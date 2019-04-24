#ifndef RF_H
#define RF_H

#include "register.hpp"
#include "instruction/instruction.hpp"
#include "infra/common.hpp"

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
    bool is_valid(Register num) const;
public:
    RF() {
        register_table[Register::zero()].is_valid = true;
    };
    
    void read_sources(Instruction &instr) const;
    void writeback(const Instruction &instr);
    void set_stack_pointer(uint32 value);
    void validate(Register num);

    void dump() const;
};

#endif
