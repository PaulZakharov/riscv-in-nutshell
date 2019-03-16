#include "infra/test/catch.hpp"
#include "instruction/instruction.hpp"

TEST_CASE("Instruction decode") {
    Instruction i(0b0000000'00010'00001'000'10000'0110011, 13u);
    CHECK(i.get_name() == "add");
    CHECK(i.get_rd() == 16);
    CHECK(i.get_rs2() == 2);
    CHECK(i.get_rs1() == 1);
    CHECK(i.get_disasm() == "add ra sp a6 -559038737");
}

TEST_CASE("Instruction disassembly") {
	CHECK(Instruction(0x00f70463, 0).get_disasm() == "beq a4 a5 zero 8");
	CHECK(Instruction(0x00052783, 0).get_disasm() == "lw a0 zero a5 0");
    CHECK(Instruction(0xf95ff06f, 0).get_disasm() == "jal zero zero zero -108");
    CHECK_THROWS(Instruction(0x10200073, 0).get_disasm());
}

TEST_CASE("lui") {
	Instruction i(0x204002b7, 0);
    CHECK(i.get_disasm() == "lui zero zero t0 541065216");
    CHECK(i.get_imm_v() == 541065216);
    i.execute();
    CHECK(i.get_rd_v() == 541065216);
}