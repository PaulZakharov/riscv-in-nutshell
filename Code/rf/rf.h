#ifndef RF_H
#define RF_H

#include "register.h"
#include "infra/common.h"


// TODO: implement register names table
// aka std::array<std::string, Register::MAX>
//
// One way to do it might be using more serious Register
// class containing registers name table as a static member
// and implemeting friend string stream << operator for
// convenient printing


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
