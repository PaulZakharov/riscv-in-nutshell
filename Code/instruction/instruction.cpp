#include "instruction.h"
#include "../rf/rf.h"

Instruction::Instruction(uint32 bytes, Addr PC):
    instr_raw(bytes),
    PC(PC),
    decoder(bytes)
{
    init_format();
    switch (this->format) {
        case Format::R:
            init_R();
            break;
        case Format::I:
            init_I();
            break;
        case Format::S:
            init_S();
            break;
        case Format::B:
            init_B();
            break;
        case Format::U;
            init_U();
            break;
        case Format::J;
            init_J();
            break;
        case Format::UNKNOWN:
            init_UNKNOWN();
            break;
    }
    new_PC = PC + 4; // to be modified in execute() for jumps
}


void Instruction::init_format() {
    for ( const auto& entry : isa_table) {
        if (Instruction:check_match(entry, instr_raw)) { 
            format = entry.format;
            name = entry.name;
            function = entry.function;
            break;
        }
    }
    
    return;
}


void Instruction::init_R() {
    rs1 = static_cast<Register>(decoder.rs1);
    rs2 = static_cast<Register>(decoder.rs2);
    rd = static_cast<Register>(decoder.rd);
    disasm = name + " " + \
        std::to_string(rd) + ", " + \
        std::to_string(rs1) + ", " + \
        std::to_string(rs2);
    return;
}

void Instruction::init_I() {
    rs1 = static_cast<Register>(decoder.rs1);
    imm_v = decoder.get_I_immediate();
    rd = static_cast<Register>(decoder.rd);
    /* Depending on a certain instruction, its 
    correct disasm may vary. I leave a simplified
    version for ADDI-like instr */
    disasm = name + " " + \
        std::to_string(rd) + ", " + \
        std::to_string(rs1) + ", " + \
        std::to_string(imm_v);
    return;
}

void Instruction::init_S() {
    rs1 = static_cast<Register>(decoder.rs1);
    rs2 = static_cast<Register>(decoder.rs2);
    imm_v = decoder.get_S_immediate();
    disasm = name + " " + \
        std::to_string(rs2) + ", " + \
        std::to_string(imm_v) + "(" + \
        std::to_string(rs1) + ")";
    return;
}

void Instruction::init_B() {
    rs1 = static_cast<Register>(decoder.rs1);
    rs2 = static_cast<Register>(decoder.rs2);
    imm_v = decoder.get_B_immediate();
    disasm = name + " " + \
        std::to_string(rs1) + ", " + \
        std::to_string(rs2) + ", " + \
        std::to_string(imm_v);
    return;
}

void Instruction::init_U() {
    imm_v = decoder.get_I_immediate();
    rd = static_cast<Register>(decoder.rd);
    disasm = name + " " + \
        std::to_string(rd) + ", " + \
        std::to_string(imm_v);
    return;
}

void Instruction::init_J() {
    imm_v = decoder.get_J_immediate();
    rd = static_cast<Register>(decoder.rd);
    disasm = name + " " + \
        std::to_string(rd) + ", " + \
        std::to_string(imm_v);
    return;
}


void Instruction::init_UNKNOWN() {
    //this function doesn't have to do anything:
    // default values are for UNKNOWN format!
    return;
}

void Instruction::execute() {
    (this->*function)();
    complete = true;
    // TODO: modify disasm
}

bool Instruction::check_match(const Instruction::ISAEntry& entry, uint32 raw) const  {
    uint8 opcode = (raw & 0b00000000'00000000'00000000'01111111);
    uint8 funct3 = (raw & 0b00000000'00000000'01110000'00000000) >> 12;
    uint8 funct7 = (raw & 0b11111110'00000000'00000000'00000000) >> 25;
    return (opcode == entry.opcode) && (funct3 == entry.funct3) && (funct7 == entry.funct7);
}

bool Instruction::is_jump() const{
    uint8 opcode = instr_raw & 0b00000000'00000000'00000000'01111111;
    return (opcode == 0b1101111) | (opcode == 0b1100111);
}

bool Instruction::is_load() const{
    uint8 opcode = instr_raw & 0b00000000'00000000'00000000'01111111;
    return opcode == 0b0000011;
}

bool Instruction::is_store() const{
    uint8 opcode = instr_raw & 0b00000000'00000000'00000000'01111111;
    return opcode == 0b0100011;
}

bool Instruction::is_nop() const{
    return instr_raw == 0b00000000'00000000'00000000'00010011; //ADDI x0, x0, 0
}
