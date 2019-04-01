#include "perfsim.hpp"

PerfSim::PerfSim(std::string executable_filename) :
    memory(executable_filename),
    rf(),
    pref_port(),
    fd_port(),
    de_port(),
    em_port(),
    mwb_port(),
    jump(false),
    jumpPC(NO_VAL32),
    E_regs(0),
    M_regs(0)
{
    // setup stack
    rf.set_stack_pointer(memory.get_stack_pointer());
    rf.validate(Register::Number::s0);
    rf.validate(Register::Number::ra);
    PreF res = PreF(memory.get_start_PC());
    std::cout << "Init: " << std::hex << res.get_PC() << std::endl;
    pref_port.write(res);
    pref_port.clock();
    fd_port.clock();
    de_port.clock();
    em_port.clock();
    mwb_port.clock(); 
}

void PerfSim::step() {
    
    this->writeback_stage();
    this->memory_stage();
    this->execute_stage();
    this->decode_stage();
    this->fetch_stage();

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

void PerfSim::fetch_stage() {
    PreF data;
    pref_port.read(data);
    Addr PC = data.get_PC();

    std::cout << "PC: " << PC <<std::endl;
    if (this->jump) {
        fd_port.set_flush(true);
        de_port.set_flush(true);
        em_port.set_flush(true);
        PreF jump_data(jumpPC);
        pref_port.write(jump_data);
        InstrPort res;
        fd_port.write(res);
    } else {
        if (PC != NO_VAL32) {
            uint32 raw_bytes = this->memory.read_word(PC);
            InstrPort res(Instruction(raw_bytes, PC));
            fd_port.write(res);
        }
        PreF next_data(jumpPC);
        pref_port.write(next_data);
    }
}

void PerfSim::decode_stage() {
    InstrPort data;
    if (!fd_port.get_flush()) {
        fd_port.read(data);
    }
    Instruction cur_instr(data.get_instr());
    uint32 D_regs = ((1 << static_cast<uint32>(cur_instr.get_rs1().id())) | \
                (1 << static_cast<uint32>(cur_instr.get_rs2().id()))) >> 1; 
    //stall logic for data hazards
    if (((D_regs & E_regs) | (D_regs & M_regs)) != 0) {
        pref_port.set_stall(true);
        fd_port.set_stall(true);
        InstrPort res;
        de_port.write(res);
    } else {
        this->rf.read_sources(data.get_instr());
        de_port.write(data);
    }
}


void PerfSim::execute_stage() {
    InstrPort data;
    if (!de_port.get_flush()) {
        de_port.read(data);
    }
    data.get_instr().execute();
    em_port.write(data);
    //debugging info
    std::cout << "0x" << std::hex << data.get_instr().get_PC() << ": "
              << data.get_instr().get_disasm() << " "
              << std::endl;
}

void PerfSim::memory_stage() {
    InstrPort data;
    if (!em_port.get_flush()) {
        em_port.read(data);
    }
    Instruction cur_instr = data.get_instr();
    // jump & branch logic
    if (cur_instr.is_jump_branch()) {
        this->jump = true;
        this->jumpPC = cur_instr.get_new_PC();
    }
    this->memory.load_store(data.get_instr());
    mwb_port.write(data);
}

void PerfSim::writeback_stage() {
    InstrPort data;
    mwb_port.read(data);
    this->rf.writeback(data.get_instr());
}