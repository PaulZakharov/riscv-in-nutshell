#include "common.hpp"

int32 sign_extend(const int bits, uint32 x) {
    uint32 m = 1;
    m <<= bits - 1;
    return static_cast<int32>((x ^ m) - m);
}
