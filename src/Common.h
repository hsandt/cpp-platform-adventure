#pragma once

// std
#include <stdint.h>

// PPK_ASSERT
// Enable assertions even in Release (mostly for FATAL aborts)
#undef PPK_ASSERT_ENABLED
#define PPK_ASSERT_ENABLED 1
#include <ppk_assert.h>

// Common game headers
#include "Memory/Box.hpp"
#include "Memory/SafePtr.hpp"

// Numeric type aliases

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

// Numeric constants

constexpr i8 MAX_I8 = std::numeric_limits<i8>::max();
constexpr i16 MAX_I16 = std::numeric_limits<i16>::max();
constexpr i32 MAX_I32 = std::numeric_limits<i32>::max();
constexpr i64 MAX_I64 = std::numeric_limits<i64>::max();

constexpr u8 MAX_U8 = std::numeric_limits<u8>::max();
constexpr u16 MAX_U16 = std::numeric_limits<u16>::max();
constexpr u32 MAX_U32 = std::numeric_limits<u32>::max();
constexpr u64 MAX_U64 = std::numeric_limits<u64>::max();

/// Handle alias is used for big container index or key
/// For small containers, just use u8
using Handle = u16;

/// DataID is used for data entries and do not need be contiguous
using DataID = u16;
