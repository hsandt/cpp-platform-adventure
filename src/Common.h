#pragma once

// std
#include <stdint.h>

// PPK_ASSERT
// Enable assertions even in Release (mostly for FATAL aborts)
#undef PPK_ASSERT_ENABLED
#define PPK_ASSERT_ENABLED 1
#include <ppk_assert.h>

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
