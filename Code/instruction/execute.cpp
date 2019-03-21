#include "instruction.hpp"

void Instruction::execute_unknown() {
    std::cerr << "Incorrect instruction: " << this->get_disasm() << std::endl;
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
    if (rs1_v == rs2_v) {
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
    rd_v = rs1_v >> (imm_v & 0b00000000'00000000'00000000'00011111);
}


void Instruction::execute_add() {
    rd_v = rs1_v + rs2_v;
}

void Instruction::execute_sub() {
    rd_v = rs1_v - rs2_v;
}

void Instruction::execute_sll() {
    rd_v = rs1_v << (rs2_v & 0b00000000'00000000'00000000'00011111);
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
    rd_v = rs1_v >> (rs2_v & 0b00000000'00000000'00000000'00011111);
}

void Instruction::execute_ld() { assert(0); }
void Instruction::execute_sd() { assert(0); }
void Instruction::execute_div() { assert(0); }
void Instruction::execute_fld() { assert(0); }
void Instruction::execute_flq() { assert(0); }
void Instruction::execute_flw() { assert(0); }
void Instruction::execute_fsd() { assert(0); }
void Instruction::execute_fsq() { assert(0); }
void Instruction::execute_fsw() { assert(0); }
void Instruction::execute_lwu() { assert(0); }
void Instruction::execute_mul() { assert(0); }
void Instruction::execute_rem() { assert(0); }
void Instruction::execute_wfi() { assert(0); }
void Instruction::execute_addw() { assert(0); }
void Instruction::execute_divu() { assert(0); }
void Instruction::execute_divw() { assert(0); }
void Instruction::execute_dret() { assert(0); }
void Instruction::execute_lr_d() { assert(0); }
void Instruction::execute_lr_w() { assert(0); }
void Instruction::execute_mret() { assert(0); }
void Instruction::execute_mulh() { assert(0); }
void Instruction::execute_mulw() { assert(0); }
void Instruction::execute_remu() { assert(0); }
void Instruction::execute_remw() { assert(0); }
void Instruction::execute_sc_d() { assert(0); }
void Instruction::execute_sc_w() { assert(0); }
void Instruction::execute_sllw() { assert(0); }
void Instruction::execute_sraw() { assert(0); }
void Instruction::execute_sret() { assert(0); }
void Instruction::execute_srlw() { assert(0); }
void Instruction::execute_subw() { assert(0); }
void Instruction::execute_uret() { assert(0); }
void Instruction::execute_addiw() { assert(0); }
void Instruction::execute_csrrc() { assert(0); }
void Instruction::execute_csrrs() { assert(0); }
void Instruction::execute_csrrw() { assert(0); }
void Instruction::execute_divuw() { assert(0); }
void Instruction::execute_ecall() { assert(0); }
void Instruction::execute_fence() { assert(0); }
void Instruction::execute_feq_d() { assert(0); }
void Instruction::execute_feq_q() { assert(0); }
void Instruction::execute_feq_s() { assert(0); }
void Instruction::execute_fle_d() { assert(0); }
void Instruction::execute_fle_q() { assert(0); }
void Instruction::execute_fle_s() { assert(0); }
void Instruction::execute_flt_d() { assert(0); }
void Instruction::execute_flt_q() { assert(0); }
void Instruction::execute_flt_s() { assert(0); }
void Instruction::execute_mulhu() { assert(0); }
void Instruction::execute_remuw() { assert(0); }
void Instruction::execute_slliw() { assert(0); }
void Instruction::execute_sraiw() { assert(0); }
void Instruction::execute_srliw() { assert(0); }
void Instruction::execute_csrrci() { assert(0); }
void Instruction::execute_csrrsi() { assert(0); }
void Instruction::execute_csrrwi() { assert(0); }
void Instruction::execute_ebreak() { assert(0); }
void Instruction::execute_fadd_d() { assert(0); }
void Instruction::execute_fadd_q() { assert(0); }
void Instruction::execute_fadd_s() { assert(0); }
void Instruction::execute_fdiv_d() { assert(0); }
void Instruction::execute_fdiv_q() { assert(0); }
void Instruction::execute_fdiv_s() { assert(0); }
void Instruction::execute_fmax_d() { assert(0); }
void Instruction::execute_fmax_q() { assert(0); }
void Instruction::execute_fmax_s() { assert(0); }
void Instruction::execute_fmin_d() { assert(0); }
void Instruction::execute_fmin_q() { assert(0); }
void Instruction::execute_fmin_s() { assert(0); }
void Instruction::execute_fmul_d() { assert(0); }
void Instruction::execute_fmul_q() { assert(0); }
void Instruction::execute_fmul_s() { assert(0); }
void Instruction::execute_fsub_d() { assert(0); }
void Instruction::execute_fsub_q() { assert(0); }
void Instruction::execute_fsub_s() { assert(0); }
void Instruction::execute_mulhsu() { assert(0); }
void Instruction::execute_amoor_d() { assert(0); }
void Instruction::execute_amoor_w() { assert(0); }
void Instruction::execute_fence_i() { assert(0); }
void Instruction::execute_fmadd_d() { assert(0); }
void Instruction::execute_fmadd_q() { assert(0); }
void Instruction::execute_fmadd_s() { assert(0); }
void Instruction::execute_fmsub_d() { assert(0); }
void Instruction::execute_fmsub_q() { assert(0); }
void Instruction::execute_fmsub_s() { assert(0); }
void Instruction::execute_fmv_d_x() { assert(0); }
void Instruction::execute_fmv_q_x() { assert(0); }
void Instruction::execute_fmv_w_x() { assert(0); }
void Instruction::execute_fmv_x_d() { assert(0); }
void Instruction::execute_fmv_x_q() { assert(0); }
void Instruction::execute_fmv_x_w() { assert(0); }
void Instruction::execute_fsgnj_d() { assert(0); }
void Instruction::execute_fsgnj_q() { assert(0); }
void Instruction::execute_fsgnj_s() { assert(0); }
void Instruction::execute_fsqrt_d() { assert(0); }
void Instruction::execute_fsqrt_q() { assert(0); }
void Instruction::execute_fsqrt_s() { assert(0); }
void Instruction::execute_amoadd_d() { assert(0); }
void Instruction::execute_amoadd_w() { assert(0); }
void Instruction::execute_amoand_d() { assert(0); }
void Instruction::execute_amoand_w() { assert(0); }
void Instruction::execute_amomax_d() { assert(0); }
void Instruction::execute_amomax_w() { assert(0); }
void Instruction::execute_amomin_d() { assert(0); }
void Instruction::execute_amomin_w() { assert(0); }
void Instruction::execute_amoxor_d() { assert(0); }
void Instruction::execute_amoxor_w() { assert(0); }
void Instruction::execute_fclass_d() { assert(0); }
void Instruction::execute_fclass_q() { assert(0); }
void Instruction::execute_fclass_s() { assert(0); }
void Instruction::execute_fcvt_d_l() { assert(0); }
void Instruction::execute_fcvt_d_q() { assert(0); }
void Instruction::execute_fcvt_d_s() { assert(0); }
void Instruction::execute_fcvt_d_w() { assert(0); }
void Instruction::execute_fcvt_l_d() { assert(0); }
void Instruction::execute_fcvt_l_q() { assert(0); }
void Instruction::execute_fcvt_l_s() { assert(0); }
void Instruction::execute_fcvt_q_d() { assert(0); }
void Instruction::execute_fcvt_q_l() { assert(0); }
void Instruction::execute_fcvt_q_s() { assert(0); }
void Instruction::execute_fcvt_q_w() { assert(0); }
void Instruction::execute_fcvt_s_d() { assert(0); }
void Instruction::execute_fcvt_s_l() { assert(0); }
void Instruction::execute_fcvt_s_q() { assert(0); }
void Instruction::execute_fcvt_s_w() { assert(0); }
void Instruction::execute_fcvt_w_d() { assert(0); }
void Instruction::execute_fcvt_w_q() { assert(0); }
void Instruction::execute_fcvt_w_s() { assert(0); }
void Instruction::execute_fnmadd_d() { assert(0); }
void Instruction::execute_fnmadd_q() { assert(0); }
void Instruction::execute_fnmadd_s() { assert(0); }
void Instruction::execute_fnmsub_d() { assert(0); }
void Instruction::execute_fnmsub_q() { assert(0); }
void Instruction::execute_fnmsub_s() { assert(0); }
void Instruction::execute_fsgnjn_d() { assert(0); }
void Instruction::execute_fsgnjn_q() { assert(0); }
void Instruction::execute_fsgnjn_s() { assert(0); }
void Instruction::execute_fsgnjx_d() { assert(0); }
void Instruction::execute_fsgnjx_q() { assert(0); }
void Instruction::execute_fsgnjx_s() { assert(0); }
void Instruction::execute_amomaxu_d() { assert(0); }
void Instruction::execute_amomaxu_w() { assert(0); }
void Instruction::execute_amominu_d() { assert(0); }
void Instruction::execute_amominu_w() { assert(0); }
void Instruction::execute_amoswap_d() { assert(0); }
void Instruction::execute_amoswap_w() { assert(0); }
void Instruction::execute_fcvt_d_lu() { assert(0); }
void Instruction::execute_fcvt_d_wu() { assert(0); }
void Instruction::execute_fcvt_lu_d() { assert(0); }
void Instruction::execute_fcvt_lu_q() { assert(0); }
void Instruction::execute_fcvt_lu_s() { assert(0); }
void Instruction::execute_fcvt_q_lu() { assert(0); }
void Instruction::execute_fcvt_q_wu() { assert(0); }
void Instruction::execute_fcvt_s_lu() { assert(0); }
void Instruction::execute_fcvt_s_wu() { assert(0); }
void Instruction::execute_fcvt_wu_d() { assert(0); }
void Instruction::execute_fcvt_wu_q() { assert(0); }
void Instruction::execute_fcvt_wu_s() { assert(0); }
void Instruction::execute_sfence_vma() { assert(0); }
void Instruction::execute_hfence_bvma() { assert(0); }
void Instruction::execute_hfence_gvma() { assert(0); }

