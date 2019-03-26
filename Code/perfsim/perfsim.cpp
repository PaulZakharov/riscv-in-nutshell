#include "perfsim.hpp"

PerfSim::PerfSim(std::string executable_filename) :
    memory(executable_filename),
    rf(),
    PC(memory.get_start_PC()),
    pref_port(),
    fd_port();
    de_port();
    em_port();
    mwb_port();
{
    // setup stack
    rf.set_stack_pointer(memory.get_stack_pointer());
    rf.validate(Register::Number::s0);
    rf.validate(Register::Number::ra);
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

void PerfSim::fetch() {}

void PerfSim::decode() {}

void PerfSim::execute() {}

void PerfSim::memory() {}

void PerfSim::writeback() {}