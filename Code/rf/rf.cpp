#include "rf.h"

uint32 RF::read(Register num) const {
    return (register_table[num]).value;
}

void RF::write(Register num, uint32 value) {
    (register_table[num]).value = value;
    (register_table[num]).is_valid = true;
    return;
}

void RF::invalidate(Register num) {
    (register_table[num]).is_valid = false;
    return;
}

void RF::validate(Register num) {
    (register_table[num]).is_valid = true;
    return;
}

bool RF::is_valid(Register num) const {
    return (register_table[num]).is_valid;
}

void read_sources(Instruction &instr) const;

void writeback(Instruction &instr);