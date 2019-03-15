#include "instruction.h"

void Instruction::execute_unknown() {
    std::cerr << "Incorrect instruction: " << this->disasm << std::endl;
    exit(EXIT_FAILURE);
}

void Instruction::execute_lui() {
    rd_v = imm_v;
}

void Instruction::execute_auipc() {
    rd_v = PC + imm_v;
}

void Instruction::execute_jal() {
    rd_v = PC + 4;
    new_PC = PC + imm_v;
}

void Instruction::execute_jalr() {
    rd_v = PC + 4;
    new_PC = (imm_v + rs1_v) & ~1;
}

void Instruction::execute_beq() {
    if (rs1_v = rs2_v) {
        new_PC = PC + imm_v;
    } else {
        new_PC = PC + 4;
    }
}

void Instruction::execute_bne() {
    if (rs1_v != rs2_v) {
        new_PC = PC + imm_v;
    } else {
        new_PC = PC + 4;
    }
}

void Instruction::execute_blt() {
    if (static_cast<int32_t>(rs1_v) < static_cast<int32_t>(rs2_v)) {
        new_PC = PC + imm_v;
    } else {
        new_PC = PC + 4;
    }
}

void Instruction::execute_bge() {
     if (static_cast<int32_t>(rs1_v) >= static_cast<int32_t>(rs2_v)) {
        new_PC = PC + imm_v;
    } else {
        new_PC = PC + 4;
    }
}

void Instruction::execute_bltu() {
    if (rs1_v < rs2_v) {
        new_PC = PC + imm_v;
    } else {
        new_PC = PC + 4;
    }
}

void Instruction::execute_bgeu() {
    if (rs1_v >= rs2_v) {
        new_PC = PC + imm_v;
    } else {
        new_PC = PC + 4;
    }
}

void Instruction::execute_lb() {
    memory_addr = rs1_v + imm_v;
}

void Instruction::execute_lh() {
    memory_addr = rs1_v + imm_v;
}

void Instruction::execute_lw() {
    memory_addr = rs1_v + imm_v;
}

void Instruction::execute_lbu() {
    memory_addr = rs1_v + imm_v;
}

void Instruction::execute_lhu() {
    memory_addr = rs1_v + imm_v;
}

void Instruction::execute_sb() {
    memory_addr = rs1_v + imm_v;
}

void Instruction::execute_sh() {
    memory_addr = rs1_v + imm_v;
}

void Instruction::execute_sw() {
    memory_addr = rs1_v + imm_v;
}

void Instruction::execute_addi() {
    rd_v = rs1_v + imm_v;
}

void Instruction::execute_slti() {
    rd_v =  static_cast<uint32>(static_cast<int32_t>(rs1_v) < imm_v);
}

void Instruction::execute_sltiu() {
    rd_v = static_cast<uint32>(rs1_v < static_cast<uint32>(imm_v));
}

void Instruction::execute_xori() {
    rd_v = rs1_v ^ imm_v;
}

void Instruction::execute_ori() {
    rd_v = rs1_v | imm_v;
}

void Instruction::execute_andi() {
    rd_v = rs1_v & imm_v;
}

void Instruction::execute_slli() {
    rd_v = rs1_v << (imm_v & 0b00000000'00000000'00000000'00011111);
}

void Instruction::execute_srai() {
    rd_v = static_cast<uint32>(static_cast<int32_t>(rs1_v) >> \
        (imm_v & 0b00000000'00000000'00000000'00011111));
}

void Instruction::execute_srli() {
    rd_v = rs1_v >> imm_v & 0b00000000'00000000'00000000'00011111;
}


void Instruction::execute_add() {
    rd_v = rs1_v + rs2_v;
}

void Instruction::execute_sub() {
    rd_v = rs1_v - rs2_v;
}

void Instruction::execute_sll() {
    rd_v = rs1_v << rs2_v & 0b00000000'00000000'00000000'00011111;
}

void Instruction::execute_slt() {
    rd_v =  static_cast<uint32>(static_cast<int32_t>(rs1_v) < \
                            static_cast<int32_t>(rs2_v));
}

void Instruction::execute_sltu() {
    rd_v =  static_cast<uint32>(rs1_v < rs2_v);
}

void Instruction::execute_xor() {
    rd_v = rs1_v ^ rs2_v;
}

void Instruction::execute_or() {
    rd_v = rs1_v | rs2_v;
}

void Instruction::execute_and() {
    rd_v = rs1_v & rs2_v;
}

void Instruction::execute_sra() {
    rd_v = static_cast<uint32>(static_cast<int32_t>(rs1_v) >> \
        (rs2_v & 0b00000000'00000000'00000000'00011111));
}

void Instruction::execute_srl() {
    rd_v = rs1_v >> rs2_v & 0b00000000'00000000'00000000'00011111;
}

