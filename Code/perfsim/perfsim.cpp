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
    if (this->jump) {
        fd_port.set_flush(true);
        de_port.set_flush(true);
        em_port.set_flush(true);
        pref_port.write(PreF(jumpPC));
    } else {
        if (pref_data.get_PC() != NO_VAL32) {
            uint32 raw_bytes = this->memory.read_word(this->PC);
            res = InstrPort(Instruction(raw_bytes, pref_data.get_PC()));
        }
        pref_port.write(PreF(pref_data.get_PC()+4));
    }
    fd_port.write(res);
}

void PerfSim::decode() {
    InstrPort data = fd_port.read();
    Instruction cur_instr = data.get_instr();
    uint32 D_regs = ((1 << static_cast<uint32>(cur_instr.get_rs1().id())) | \
                (1 << static_cast<uint32>(cur_instr.get_rs2().id()))) >> 1; 
    //stall logic for data hazards
    if (((D_regs & E_regs) | (D_regs & M_regs)) != 0) {
        pref_port.set_stall(true);
        fd_port.set_stall(true);
        de_port.write(InstrPort());
    } else {
        this->rf.read_sources(cur_instr);
        de_port.write(InstrPort(cur_instr));
    }
}

void PerfSim::execute() {
    InstrPort data = de_port.read();
    Instruction cur_instr = data.get_instr();
    cur_instr.execute();
    em_port.write(InstrPort(cur_instr));
}

void PerfSim::memory() {
    InstrPort data = em_port.read();
    Instruction cur_instr = data.get_instr();
    // jump & branch logic
    if (cur_instr.is_jump_branch()) {
        this->jump = true;
        this->jumpPC = cur_instr.get_new_PC();
    }
    this->memory.load_store(cur_instr);
    mwb_port.write(InstrPort(cur_instr));
}

void PerfSim::writeback() {
    InstrPort data = mwb_port.read();
    Instruction cur_instr = data.get_instr();
    this->rf.writeback(cur_instr);
}