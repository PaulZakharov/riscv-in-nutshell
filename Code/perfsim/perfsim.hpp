#ifndef PERFSIM_H
#define PERFSIM_H

#include "infra/common.hpp"
#include "rf/rf.hpp"
#include "memory/memory.hpp"
#include "cache/cache.hpp"
#include "stage_register/stage_register.hpp"
#include "infra/elf/elf.hpp"

class PerfSim {
private:
    ElfLoader loader;
    PerfMemory memory;
    Cache icache;
    Cache dcache;
    RF rf;
    Addr PC;
    uint32 clocks;
    uint32 ops;
    uint32 branch_penalties = 0;
    uint32 data_stalls = 0;
    uint32 memory_stalls = 0;
    uint32 multiple_stalls = 0;

    bool branch_mispredict = false;
    bool fetch_stall = false;
    bool memory_stall = false;
    bool data_stall = false;
    bool multiple_stall = false;

    struct StageRegisterStore {
        StageRegister<Instruction> FETCH_DECODE;
        StageRegister<Instruction> DECODE_EXE;
        StageRegister<Instruction> EXE_MEM;
        StageRegister<Instruction> MEM_WB;
    } stage_registers;

    // used for feedback from later stages to earlier stages 
    struct WireStore {
        // branch misprediction flush
        Addr memory_to_fetch_target = NO_VAL32;
        bool memory_to_all_flush = false;

        bool PC_stage_reg_stall = false;
        bool FD_stage_reg_stall = false;
        bool DE_stage_reg_stall = false;
        bool EM_stage_reg_stall = false;

        // masks of RF registers used at EXE/MEM stages
        uint32 execute_stage_regs = 0;
        uint32 memory_stage_regs = 0;
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
