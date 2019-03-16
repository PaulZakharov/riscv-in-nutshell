#ifndef DECODER_H
#define DECODER_H

#include "infra/common.hpp"
#include "instruction.hpp"
#include "rf/rf.hpp"

class Decoder {
private:
    using Format = Instruction::Format;
    const Format format;

    const uint32 rd;
    const uint32 rs1;
    const uint32 rs2;
    const uint32 I_imm;
    const uint32 S_imm4_0;
    const uint32 S_imm11_5;
    const uint32 B_imm4_1;
    const uint32 B_imm10_5;
    const uint32 B_imm11;
    const uint32 B_imm12;
    const uint32 U_imm31_12;
    const uint32 J_imm10_1;
    const uint32 J_imm11;
    const uint32 J_imm19_12;
    const uint32 J_imm20;

    uint32 get_I_immediate() const {
        return I_imm;
    }

    uint32 get_S_immediate() const {
        return S_imm4_0
            | (S_imm11_5 << 5);
    }

    uint32 get_B_immediate() const {
        return (B_imm4_1  << 1)
            |  (B_imm10_5 << 5)
            |  (B_imm11   << 11)
            |  (B_imm12   << 12);
    }

    uint32 get_U_immediate() const {
        return (U_imm31_12 << 12);
    }

    uint32 get_J_immediate() const {
        return (J_imm10_1  << 1)
            |  (J_imm11    << 11)
            |  (J_imm19_12 << 12)
            |  (J_imm20    << 20);
    }


    uint32 apply_mask(uint32 bytes, uint32 mask) const {
        // en.wikipedia.org/wiki/Find_first_set
        return (bytes & mask) >> __builtin_ctz(mask);
    }

public:
    int32 get_immediate() const {
        switch(format) {
            case Format::R: return NO_VAL32;
            case Format::I: return sign_extend(12, get_I_immediate());
            case Format::S: return sign_extend(12, get_S_immediate());
            case Format::B: return sign_extend(12, get_B_immediate());
            case Format::U: return sign_extend(32, get_U_immediate());
            case Format::J: return sign_extend(20, get_J_immediate());
            default:        assert(0);
        }
    }

    Register get_rs1() const {
        switch(format) {
            case Format::R: return Register(rs1);
            case Format::I: return Register(rs1);
            case Format::S: return Register(rs1);
            case Format::B: return Register(rs1);
            case Format::U: return Register::zero();
            case Format::J: return Register::zero();
            default:        assert(0);
        }
    }

    Register get_rs2() const {
        switch(format) {
            case Format::R: return Register(rs2);
            case Format::I: return Register::zero();
            case Format::S: return Register(rs2);
            case Format::B: return Register(rs2);
            case Format::U: return Register::zero();
            case Format::J: return Register::zero();
            default:        assert(0);
        }
    }

    Register get_rd() const {
        switch(format) {
            case Format::R: return Register(rd);
            case Format::I: return Register(rd);
            case Format::S: return Register::zero();
            case Format::B: return Register::zero();
            case Format::U: return Register(rd);
            case Format::J: return Register(rd);
            default:        assert(0);
        }
    }

    Decoder(uint32 raw, Format format) : format(format),
        rd         (apply_mask(raw, 0b00000000'00000000'00001111'10000000)),
        rs1        (apply_mask(raw, 0b00000000'00001111'10000000'00000000)),
        rs2        (apply_mask(raw, 0b00000001'11110000'00000000'00000000)),
        I_imm      (apply_mask(raw, 0b11111111'11110000'00000000'00000000)),
        S_imm4_0   (apply_mask(raw, 0b00000000'00000000'00001111'10000000)),
        S_imm11_5  (apply_mask(raw, 0b11111110'00000000'00000000'00000000)),
        B_imm4_1   (apply_mask(raw, 0b00000000'00000000'00001111'00000000)),
        B_imm10_5  (apply_mask(raw, 0b01111110'00000000'00000000'00000000)),
        B_imm11    (apply_mask(raw, 0b00000000'00000000'00000000'10000000)),
        B_imm12    (apply_mask(raw, 0b10000000'00000000'00000000'00000000)),
        U_imm31_12 (apply_mask(raw, 0b11111111'11111111'11110000'00000000)),
        J_imm10_1  (apply_mask(raw, 0b01111111'11100000'00000000'00000000)),
        J_imm11    (apply_mask(raw, 0b00000000'00010000'00000000'00000000)),
        J_imm19_12 (apply_mask(raw, 0b00000000'00001111'11110000'00000000)),
        J_imm20    (apply_mask(raw, 0b10000000'00000000'00000000'00000000))
    { }
};

#endif
