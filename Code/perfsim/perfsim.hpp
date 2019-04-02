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
        Latch<PreF> pref_port;
        Latch<InstrPort> fd_port;
        Latch<InstrPort> de_port;
        Latch<InstrPort> em_port;
        Latch<InstrPort> mwb_port;
        // these used for feedback from later stages to fetch
        //pipeline flushing in case of jumps & branches  
        struct WireStore{
            bool FD_latch_flush = false;
            bool DE_latch_flush = false;
            bool PreF_latch_stall = false;
            bool FD_latch_stall = false;
            bool target_mispredict = false;
            Addr target = NO_VAL32;
            //i-th bit means that stage (i-1)th reg is used
            //zero excluded
            uint32 Execute_stage_regs = 0;
            uint32 Memory_stage_regs = 0;
        } wires;
    public:
        PerfSim(std::string executable_filename);
        void run(uint32 n);
        
        void step();
        
        void fetch_stage();
        void decode_stage();
        void execute_stage();
        void memory_stage();
        void writeback_stage();
};

#endif
