#ifndef PERFSIM_H
#define PERFSIM_H

#include "infra/common.hpp"
#include "rf/rf.hpp"
#include "memory/memory.hpp"
#include "latch/latch.hpp"

class PerfSim {
    private:
        Memory memory;
        RF rf;
        Latch<Addr> pref_latch;
        Latch<Instruction> fd_latch;
        Latch<Instruction> de_latch;
        Latch<Instruction> em_latch;
        Latch<Instruction> mwb_latch;
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
