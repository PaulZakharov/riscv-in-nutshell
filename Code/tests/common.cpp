#include "infra/test/catch.hpp"
#include "infra/common.hpp"

TEST_CASE("sign_extend") {
    CHECK(sign_extend(20, 0b11111111111111001010) == -54);
    CHECK(sign_extend(21, 0b111111111111110010100) == -108);
}