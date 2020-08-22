#pragma once

#include <array>
#include <bit>
#include <cstdint>

namespace sjsu
{
template <typename T>
constexpr auto ToByteArray(std::endian endian, T value)
{
  std::array<uint8_t, sizeof(T)> array = {};
  static_assert(std::is_integral_v<T>,
                "Type T (the return type) must be intergal type.");
  if constexpr (std::is_integral_v<T>)
  {
    if (endian == std::endian::little)
    {
      for (size_t i = 0; i < array.size(); i++)
      {
        auto next_byte   = value >> ((CHAR_BIT * i) & 0xFF);
        array.begin()[i] = static_cast<uint8_t>(next_byte);
      }
    }
    else
    {
      for (size_t i = 0; i < array.size(); i++)
      {
        auto next_byte    = value >> ((CHAR_BIT * i) & 0xFF);
        array.rbegin()[i] = static_cast<uint8_t>(next_byte);
      }
    }
  }
  return array;
}

template <typename T>
constexpr auto ToInteger(std::endian endian, std::span<const uint8_t> array)
{
  static_assert(std::is_integral_v<T>,
                "Type T (the return type) must be intergal type.");
  T value;
  if constexpr (std::is_integral_v<T>)
  {
    // Clear out value before performing the copy. The copy may not copy over
    // all of the bytes of T and thus initalizing this value to zero is
    // required.
    value = 0;

    size_t end = std::min(sizeof(T), array.size());

    if (endian == std::endian::little)
    {
      for (size_t i = 0; i < end; i++)
      {
        auto or_value = static_cast<T>(array.begin()[i]) << (CHAR_BIT * i);
        value         = static_cast<T>(value | or_value);
      }
    }
    else
    {
      for (size_t i = 0; i < end; i++)
      {
        auto or_value = static_cast<T>(array.rbegin()[i]) << (CHAR_BIT * i);
        value         = static_cast<T>(value | or_value);
      }
    }
  }

  return value;
}

template <typename T, size_t N>
constexpr auto ToIntegerArray(std::endian endian,
                              std::span<const uint8_t> bytes)
{
  static_assert(std::is_integral_v<T>,
                "Type T (the return type) must be intergal type.");
  std::array<T, N> value = { 0 };
  if constexpr (std::is_integral_v<T>)
  {
    for (size_t i = 0; i < bytes.size() / sizeof(T); i++)
    {
      std::span<const uint8_t> span_of_bytes(bytes.begin() + (i * sizeof(T)),
                                             sizeof(T));
      value[i] = ToInteger<T>(endian, span_of_bytes);
    }
  }
  return value;
}
}  // namespace sjsu
