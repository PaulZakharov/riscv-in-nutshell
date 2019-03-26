#ifndef PERFSIM_H
#define PERFSIM_H

#include "infra/common.hpp"
#include "rf/rf.hpp"
#include "memory/memory.hpp"
#include "port/port.hpp"

class PerfSim {
    private:
        Memory memory;
        RF rf;
        Addr PC = NO_VAL32;
        PreF pref_port;
        FD fd_port;
        DE de_port;
        EM em_port;
        MWB mwb_port;
    public:
        PerfSim(std::string executable_filename);
        void run(uint32 n);
        
        void step();
        
        void fetch();
        void decode();
        void execute();
        void memory();
        void writeback();
};

#endif
