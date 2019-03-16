#ifndef TYPES_H
#define TYPES_H

#include <cstdint>

using int8    = int8_t;
using int16   = int16_t;
using int32   = int32_t;
using int64   = int64_t;
using uint8   = uint8_t;
using uint16  = uint16_t;
using uint32  = uint32_t;
using uint64  = uint64_t;
using float32 = float;
using float64 = double;

// semantics
using Addr   = uint32;
using Cycles = uint32;
using Size   = uint32;

static const uint8  NO_VAL8  = 0xba;
static const uint16 NO_VAL16 = 0xf00d;
static const uint32 NO_VAL32 = 0xdeadbeeful;
static const uint64 NO_VAL64 = 0xfeedfacecafebeafull;

static const uint8  MAX_VAL8  = UINT8_MAX;
static const uint16 MAX_VAL16 = UINT16_MAX;
static const uint32 MAX_VAL32 = UINT32_MAX;
static const uint64 MAX_VAL64 = UINT64_MAX;

#endif
