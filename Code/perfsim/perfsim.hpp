#ifndef PERFSIM_H
#define PERFSIM_H

#include "infra/common.hpp"
#include "rf/rf.hpp"
#include "new_memory/new_memory.hpp"
#include "latch/latch.hpp"

class PerfSim {
    private:
        Memory memory;
        RF rf;
        Latch<Addr> pc_latch;
        Latch<Instruction> fd_latch;
        Latch<Instruction> de_latch;
        Latch<Instruction> em_latch;
        Latch<Instruction> mwb_latch;
        // these used for feedback from later stages to fetch
        //pipeline flushing in case of jumps & branches  
        struct WireStore{
            bool FD_latch_flush = false;
            bool DE_latch_flush = false;
            bool PC_latch_flush = false;
            bool PC_latch_stall = false;
            bool FD_latch_stall = false;
            bool DE_latch_stall = false;
            bool EM_latch_stall = false;
            Addr target = NO_VAL32;
            //i-th bit means that stage (i-1)th reg is used
            //zero excluded
            uint32 Execute_stage_regs = 0;
            uint32 Memory_stage_regs = 0;
            //memory usage wires
            int memory_stage_iter = 0;
            int fetch_stage_iter = 0;
            uint32 fetch_bytes = NO_VAL32;
            bool memory_stage_usage = false;
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
