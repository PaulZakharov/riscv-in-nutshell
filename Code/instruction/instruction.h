#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "infra/common.h"
#include "rf/register.h"

class Instruction {
public:
    // RISCV encoding format
    enum class Format {
        R, I, S, B, U, J,
        UNKNOWN
    };

    // internal types
    enum class Type {
        LOADU, LOAD, STORE,
        ARITHM, NOP,
        JUMP, BRANCH,
        UNKNOWN
    };

    // executor function type
    using Executor = void (Instruction::*)(void);

private:
    // PC
    const Addr PC = NO_VAL32;
    Addr new_PC = NO_VAL32;

    // main info
    bool complete = false;
    std::string name = "unknown";
    Format format = Format::UNKNOWN;
    Type type = Type::UNKNOWN;

    // registers
    Register rs1 = Register::zero();
    Register rs2 = Register::zero();
    Register rd  = Register::zero();

    // registers values
    uint32 rs1_v = NO_VAL32;
    uint32 rs2_v = NO_VAL32;
    uint32 rd_v  = NO_VAL32;
    
    // immediate
    int32 imm_v = NO_VAL32;

    // for loads/stores
    Addr memory_addr = NO_VAL32;
    Size memory_size = NO_VAL32;

public:
    // constructors
    explicit Instruction(uint32 bytes, Addr PC);
    Instruction() = delete;

    // dummy getters
    Register get_rs1 () const { return rs1; }
    Register get_rs2 () const { return rs2; }
    Register get_rd  () const { return rd; }

    bool is_load_sign_extended () const { return type == Type::LOAD; }
    bool is_load_zero_extended () const { return type == Type::LOADU; }
    bool is_store () const { return type == Type::STORE; }
    bool is_nop   () const { return type == Type::NOP; }

    void set_rs1_v (uint32 value) { rs1_v = value; }
    void set_rs2_v (uint32 value) { rs2_v = value; }
    void set_rd_v  (uint32 value) {  rd_v = value; }

    uint32 get_rs1_v () const { return rs1_v; }
    uint32 get_rs2_v () const { return rs2_v; }
    uint32 get_rd_v  () const { return  rd_v; }
    int32  get_imm_v () const { return imm_v; }

    Addr get_PC      () const { return PC;     }
    Addr get_new_PC  () const { return new_PC; }

    Addr get_memory_addr() const { return memory_addr; }
    Size get_memory_size() const { return memory_size; }

    // representation
    std::string get_disasm() const;

    // executors
    void execute();
    void execute_unknown();

#define DECLARE_INSN(name, match, mask) \
void execute_ ## name ();
#include "opcodes.gen.h"
#undef DECLARE_INSN

    Executor function = &Instruction::execute_unknown;
};

#endif
