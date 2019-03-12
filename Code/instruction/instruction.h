#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "common.h"
#include "decoder.h"

class Instruction {
private:
    enum class Format {
        R, I, S, B, U, J,
        UNKNOWN
    } format = Format::UNKNOWN;

    // main
    const Addr PC = NO_VAL32;
    bool complete = false;
    std::string name = "unknown";

    // registers
    Register rs1 = Register::zero;
    Register rs2 = Register::zero;
    Register rd  = Register::zero;

    // registers values
    uint32 rs1_v = NO_VAL32;
    uint32 rs2_v = NO_VAL32;
    uint32 rd_v  = NO_VAL32;
    
    // immediate
    int32 imm_v = NO_VAL32;

    // for loads/stores
    uint32 memory_addr = NO_VAL32;
    size_t memory_size = 0;

    // for branches/jumps
    Addr new_PC   = NO_VAL32;

    // representation
    std::string disasm = "INVALID";
    
    // executors
    void execute_unknown();

#define DECLARE_INSN(name, match, mask) \
void execute_ ## name ();

#include "opcodes.gen.h"

#undef DECLARE_INSN

    using Executor = void (Instruction::*)(void);
    Executor function = &Instruction::execute_unknown;

public:
    explicit Instruction(uint32 bytes, Addr PC);
    Instruction(Instruction& instr);

    const std::string& get_disasm() const { return disasm; }

    Register get_rs1 () const { return rs1; }
    Register get_rs2 () const { return rs2; }
    Register get_rd  () const { return rd;  }

    bool is_jump  () const;
    bool is_load  () const;
    bool is_store () const;
    bool is_nop   () const;

    void set_rs1_v (uint32 value);
    void set_rs2_v (uint32 value);
    void set_rd_v  (uint32 value);

    uint32 get_rs1_v () const { return rs1_v; }
    uint32 get_rs2_v () const { return rs2_v; }
    uint32 get_rd_v  () const { return rd_v;  }
    int32  get_imm_v () const { return imm_v; }

    Addr get_PC      () const { return PC;     }
    Addr get_new_PC  () const { return new_PC; }

    Addr   get_memory_addr() const { return memory_addr; }
    size_t get_memory_size() const { return memory_size; }

    void execute();
};

#endif
