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
    // PC
}

void Instruction::execute_bne() {}

void Instruction::execute_blt() {}

void Instruction::execute_bge() {}

void Instruction::execute_bltu() {}

void Instruction::execute_bgeu() {}

void Instruction::execute_lb() {
    // memory access   
}

void Instruction::execute_lh() {

}

void Instruction::execute_lw() {

}

void Instruction::execute_lbu() {

}

void Instruction::execute_lhu() {

}

void Instruction::execute_sb() {

}

void Instruction::execute_sh() {

}

void Instruction::execute_sw() {

}

void Instruction::execute_addi() {
    rd_v = static_cast<uint32>(static_cast<int32_t>rs1_v + static_cast<int32_t>imm_v);
}

void Instruction::execute_slti() {
    rd_v =  static_cast<uint32>(static_cast<int32_t>(rs1_v) < \
                            static_cast<int32_t>(imm_v));
}

void Instruction::execute_sltiu() {
    rd_v = static_cast<uint32>(rs1_v < imm_v);
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
    
}
void Instruction::execute_or() {
    this->rd_v = ... // TODO
}














// TODO: make a huge list of functions implementations
