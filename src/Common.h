#pragma once

#include <stdint.h>

// Type aliases

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

// Handle alias is used for container index or key
using Handle = u16;

// DataID is used for data entries and do not need be contiguous
using DataID = u16;
