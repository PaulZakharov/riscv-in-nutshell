#include "funcsim.hpp"

FuncSim::FuncSim(std::string executable_filename)
    : loader(executable_filename)
    , memory(loader.load_data())
    , PC(loader.get_start_PC())
    , rf()
{
    // setup stack
    rf.set_stack_pointer(memory.get_stack_pointer());
    rf.validate(Register::Number::s0);
    rf.validate(Register::Number::ra);
}

void FuncSim::step() {
    // fetch
    uint32 raw_bytes = this->memory.read_word(this->PC);
    // decode
    Instruction instr(raw_bytes, this->PC);
    this->rf.read_sources(instr);
    // execute
    instr.execute();
    // memory
    this->memory.load_store(instr);
    // writeback
    this->rf.writeback(instr);

    // let's start with this and improve when needed
    std::cout << "0x" << std::hex << this->PC << ": "
              << instr.get_disasm() << " "
              << "(0x" << std::hex << raw_bytes << ")" << std::endl;
    this->rf.dump();

    this->PC = instr.get_new_PC();
}

void FuncSim::run(const uint32 n) {
    for (uint32 i = 0; i < n; ++i)
        this->step();
}
