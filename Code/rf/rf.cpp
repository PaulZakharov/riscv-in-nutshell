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

void RF::read_sources(Instruction &instr) const {
    Register rs1 = instr.get_rs1();
    if (is_valid(rs1)) {
        instr.set_rs1_v(read(rs1));
    } else {
        //trap?
    }
    Register rs2 = instr.get_rs2();
    if (is_valid(rs2)) {
        instr.set_rs2_v(read(rs2));
    } else {
        //trap?
    }
    return;
}

void writeback(const Instruction &instr) {
    Register rd = instr.get_rd();
    write(rd, get_rd_v());
    return;
}