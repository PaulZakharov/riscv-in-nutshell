#include "infra/test/catch.hpp"
#include "instruction/instruction.hpp"

TEST_CASE("Dummy test showing that testing works", "[multi-file:2]") { }
TEST_CASE("Instrcution class test", "[multi-file:2]") {
    Instruction i(0b0000000'00010'00001'000'10000'0110011, 13u);
    REQUIRE(i.get_name() == "add");
}
