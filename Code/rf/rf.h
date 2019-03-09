#ifndef RF_H
#define RF_H

#include "../infra/common.h"
#include "../infra/types.h"


// TODO: implement register names table
// aka std::array<std::string, Register::MAX>
//
// One way to do it might be using more serious Register
// class containing registers name table as a static member
// and implemeting friend string stream << operator for
// convenient printing

enum class Register {
    zero,
    ra,
    sp,
    gp,
    tp,
    t0,
    t1, t2,
    s0,
    s1,
    a0, a1,
    a2, a3, a4, a5, a6, a7,
    s2, s3, s4, s5, s6, s7, s8, s9, s10, s11,
    t3, t4, t5, t6,
    MAX
};

class RF {
private:
    uint32 read(Register num) const;
    void write(Register num, uint32 value);

    struct RegisterState {
        uint32 value = 0;
        bool is_valid = true; 
    };
    
    std::array<RegisterState, Register::MAX> register_table;
    
    void invalidate(Register num);
    void validate(Register num);
    bool is_valid(Register num) const;
public:
    RF() { };
    
    void read_sources(Instruction &instr) const;
    void writeback(const Instruction &instr);
};

#endif
