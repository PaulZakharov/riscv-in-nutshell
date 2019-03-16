#include "infra/test/catch.hpp"
#include "instruction/instruction.hpp"

TEST_CASE("Dummy test showing that testing works", "[multi-file:2]") { }
TEST_CASE("ADD", "[multi-file:2]") {
    Instruction i(0b0000000'00010'00001'000'10000'0110011, 13u);
    REQUIRE(i.get_name() == "add");
    REQUIRE(i.get_rd() == 16);
    REQUIRE(i.get_rs2() == 2);
    REQUIRE(i.get_rs1() == 1);
    REQUIRE(i.get_disasm() == "add ra sp a6 -559038737");
}
