#ifndef _COMMON_H_
#define _COMMON_H_

#include <iostream>
#include <vector>
#include <string>

#include "types.h"

int32 sign_extend(const int bits, uint32 x) {
    uint32 m = 1;
    m <<= bits - 1;
    return static_cast<int32>((x ^ m) - m);
}

#endif
