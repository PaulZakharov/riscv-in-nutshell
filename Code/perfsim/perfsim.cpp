#include "perfsim.hpp"

PerfSim::PerfSim(std::string executable_filename) :
    memory(executable_filename),
    rf(),
    pref_port(),
    fd_port(),
    de_port(),
    em_port(),
    mwb_port(),
    branch(false),
    jump(false),
    branchPC(NO_VAL32),
    jumpPC(NO_VAL32),
    E_regs(0),
    M_regs(0)
{
    // setup stack
    rf.set_stack_pointer(memory.get_stack_pointer());
    rf.validate(Register::Number::s0);
    rf.validate(Register::Number::ra);
    pref_port.write(PreF(memory.get_start_PC()));
}

void PerfSim::step() {
    
    this->writeback();
    this->memory();
    this->execute();
    this->decode();
    this->fetch();

    pref_port.clock();
    fd_port.clock();
    de_port.clock();
    em_port.clock();
    mwb_port.clock();

}

void PerfSim::run(const uint32 n) {
    for (uint32 i = 0; i < n; ++i)
        this->step();
}

void PerfSim::fetch() {
    PreF pref_data = pref_port.read();
    InstrPort res;
    if (this->branch | this->jump) {
        fd_port.set_flush(true);
        de_port.set_flush(true);
        em_port.set_flush(true);
        mwb_port.set_flush(true);
        if (this->branch) {
            pref_port.write(PreF(branchPC));
        } else {
            pref_port.write(PreF(jumpPC));
        }
        fd_port.write(res);
    } else {
        if (pref_data.get_PC() != NO_VAL32) {
            uint32 raw_bytes = this->memory.read_word(this->PC);
            res = FD(Instruction(raw_bytes, pref_data.get_PC()));
        }
        fd_port.write(res);
        pref_port.write(PreF(pref_data.get_PC()+4));
    }
}

void PerfSim::decode() {
    InstrPort data = fd_port.read();
    Instruction cur_instr = data.get_instr();
    uint32 D_regs = ((1 << static_cast<uint32>(cur_instr.get_rs1().id())) | \
                (1 << static_cast<uint32>(cur_instr.get_rs2().id()))) >> 1; 
    //stall logic
    if (((D_regs & E_regs) | (D_regs & M_regs)) == 0) {
        pref_port.set_stall(true);
        fd_port.set_stall(true);
        de_port.write(InstrPort());
    }
}

void PerfSim::execute() {}

void PerfSim::memory() {}

void PerfSim::writeback() {}