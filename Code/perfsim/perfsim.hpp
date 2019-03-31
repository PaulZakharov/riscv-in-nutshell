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
        Port<PreF> pref_port;
        Port<InstrPort> fd_port;
        Port<InstrPort> de_port;
        Port<InstrPort> em_port;
        Port<InstrPort> mwb_port;
        // these used for feedback from later stages to fetch
        //pipeline flushing in case of jumps & branches  
        bool jump;
        Addr jumpPC;
        //pipeline stalls in case of memory dependencies
        uint32 E_regs;
        uint32 M_regs;

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
