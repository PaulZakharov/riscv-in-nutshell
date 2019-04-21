#ifndef PERFSIM_H
#define PERFSIM_H

#include "infra/common.hpp"
#include "rf/rf.hpp"
#include "memory/memory.hpp"
#include "stage_reg/stage_reg.hpp"
#include "infra/elf/elf.hpp"

class PerfSim {
private:
    ElfLoader loader;
    PerfMemory memory;
    RF rf;

    // inter-stage regsiters
    Stage_Reg<Addr> pc_stage_reg;
    Stage_Reg<Instruction> fd_stage_reg;
    Stage_Reg<Instruction> de_stage_reg;
    Stage_Reg<Instruction> em_stage_reg;
    Stage_Reg<Instruction> mwb_stage_reg;

    // used for feedback from later stages to fetch
    // pipeline flushing in case of jumps & branches  
    struct WireStore {
        Addr memory_to_fetch_target = NO_VAL32;
        bool memory_to_all_flush = false;
        
        bool PC_stage_reg_stall = false;
        bool FD_stage_reg_stall = false;
        bool DE_stage_reg_stall = false;
        bool EM_stage_reg_stall = false;

        // i-th bit means that stage (i-1)th reg is used
        // zero excluded
        uint32 execute_stage_regs = 0;
        uint32 memory_stage_regs = 0;

        // memory usage wires
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
