#include "rf.h"

RF::Reg & RF::get(Register num) {
    return (array[static_cast<uint16>(num)]);
}

const RF::Reg & RF::get_const(Register num) const {
    return const_cast<const RF::Reg &>(array[static_cast<uint32>(num)]);
}

void RF::invalidate(Register num) {
    get(num).is_valid = false;
    return;
}

void RF::validate(Register num) {
    get(num).is_valid = true;
    return;
}

bool RF::check(Register num) const {
    return get_const(num).is_valid;

}

uint32 RF::read(Register num) const {
    return get_const(num).value;
}

void RF::write (Register num, uint32 val) {
    Reg& cur_reg = get(num);
    cur_reg.value = val;
    cur_reg.is_valid = true;
    return;
}