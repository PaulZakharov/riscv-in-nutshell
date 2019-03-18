#include "rf.hpp"

uint32 RF::read(Register num) const {
    return register_table[num].value;
}

void RF::write(Register num, uint32 value) {
    register_table[num].value = value;
    register_table[num].is_valid = true;
}

void RF::invalidate(Register num) {
    register_table[num].is_valid = false;
}

void RF::validate(Register num) {
    register_table[num].is_valid = true;
}

bool RF::is_valid(Register num) const {
    return register_table[num].is_valid;
}

void RF::read_sources(Instruction &instr) const {
    Register rs1 = instr.get_rs1();
    Register rs2 = instr.get_rs2();

    if (is_valid(rs1))
        instr.set_rs1_v(this->read(rs1));
    else
        assert(0); // TRAP

    if (is_valid(rs2))
        instr.set_rs2_v(this->read(rs2));
    else
        assert(0); // TRAP
}

void RF::writeback(const Instruction &instr) {
    Register rd = instr.get_rd();
    uint32 value = instr.get_rd_v();

    if (instr.is_sign_extended_load()) {
        auto bits = 8*instr.get_memory_size();
        int32 sign_extended_value = sign_extend(bits, value);
        value = static_cast<uint32>(sign_extended_value);
    }
    this->write(rd, value);
    this->write(Register::Number::zero, 0);
}

void RF::dump() const {
    for(uint8 i = 0; i < (Register::MAX_NUMBER); i++) {
        Register dumped(i);
        std::cout << dumped << " = " << register_table[i].value << std::endl;
    }
    return;
}