#pragma once

#include <concepts>

template <typename T>
    requires std::is_arithmetic_v<T>
constexpr auto ms_to_kmph(T s)
{
    return (s * 60.0 * 60.0) / 1000.0;
}

template <typename T>
    requires std::is_arithmetic_v<T>
constexpr auto psi_to_bar(T p)
{
    return p * 0.0689476;
}

template <typename T>
    requires std::is_arithmetic_v<T>
constexpr auto ms_to_mph(T s)
{
    return s * 2.2369362920544;
}

template <typename T>
    requires std::is_arithmetic_v<T>
constexpr T clamp(T value, T min = 0.0f, T max = 1.0f)
{
    return (value < min) ? min : ((value > max) ? max : value);
}
