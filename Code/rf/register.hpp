#ifndef REGISTER_H
#define REGISTER_H

#include <array>
#include "infra/common.hpp"

class Register {
public:
    enum class Number {
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

    friend std::ostream& operator<<(std::ostream& out, const Register& reg) {
        return out << reg.get_name();
    }

    Register(Number number) : number(number) { };
    Register(uint8 number) : number(static_cast<Number>(number)) { };

    size_t id() const { return static_cast<size_t>(this->number); }
    static Register zero() { return Register(Number::zero); }
    operator size_t() const { return static_cast<size_t>(this->number); }

    static constexpr const size_t MAX_NUMBER = static_cast<size_t>(Number::MAX);
    static_assert(MAX_NUMBER == 32u, "Wrong amount of registers");

private:
    Number number = Number::zero;
    static const std::array<std::string, MAX_NUMBER> names_table;
    const std::string get_name() const { return this->names_table[this->id()]; }
};

#endif
