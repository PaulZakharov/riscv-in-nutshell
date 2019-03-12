#ifndef RF_H
#define RF_H

#include "register.h"
#include "instruction/instruction.h"
#include "infra/common.h"


class RF {
private:
    uint32 read(Register num) const;
    void write(Register num, uint32 value);

    struct RegisterState {
        uint32 value = 0;
        bool is_valid = true; 
    };
    
    std::array<RegisterState, Register::MAX_NUMBER> register_table;
    
    void invalidate(Register num);
    void validate(Register num);
    bool is_valid(Register num) const;
public:
    RF() { };
    
    void read_sources(Instruction &instr) const;
    void writeback(const Instruction &instr);
};

#endif
