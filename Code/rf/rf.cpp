#include "rf.hpp"
#include "infra/config/config.hpp"

namespace config {
    static         Value<uint64>      dump_rf     = { "dump_rf", "whether to dump RF state", false };
}

uint32 RF::read(Register num) const {
    if (this->is_valid(num))
        return this->register_table[num].value;
    
    throw std::invalid_argument("Register " + num.get_name() + " is INVALID");
}

void RF::write(Register num, uint32 value) {
    if (num == 0) return;
    this->register_table[num].value = value;
    this->validate(num);
}

void RF::invalidate(Register num) {
    if (num == 0) return;
    this->register_table[num].is_valid = false;
}

void RF::validate(Register num) {
    this->register_table[num].is_valid = true;
}

bool RF::is_valid(Register num) const {
    return this->register_table[num].is_valid;
}

void RF::read_sources(Instruction &instr) const {
    instr.set_rs1_v(this->read(instr.get_rs1()));
    instr.set_rs2_v(this->read(instr.get_rs2()));
}

void RF::writeback(const Instruction &instr) {
    uint32 value = instr.get_rd_v();

    if (instr.is_sign_extended_load()) {
        auto bits = 8*instr.get_memory_size();
        int32 sign_extended_value = sign_extend(bits, value);
        value = static_cast<uint32>(sign_extended_value);
    }

    this->write(instr.get_rd(), value);
}

void RF::set_stack_pointer(uint32 value) {
    this->write(Register::Number::sp, value);
}

void RF::dump() const {
    if (!config::dump_rf)
        return;

    std::cout << "Register file dump:" << std::endl;
    
    for(uint8 i = 0; i < (Register::MAX_NUMBER); ++i) {
        if (!this->is_valid(i))
            continue;

        std::cout << '\t' << Register(i) << " = " << std::hex
                  << this->register_table[i].value << std::endl;
    }

    std::cout << std::endl;
}
