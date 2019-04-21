#ifndef PERFSIM_H
#define PERFSIM_H

#include "infra/common.hpp"
#include "rf/rf.hpp"
#include "memory/memory.hpp"
#include "stage_register/stage_register.hpp"
#include "infra/elf/elf.hpp"

class PerfSim {
private:
    ElfLoader loader;
    PerfMemory memory;
    RF rf;

    struct StageRegisterStore {
        StageRegister<Addr> PC;
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

        // i-th bit means that stage (i-1)th RF register is used
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
