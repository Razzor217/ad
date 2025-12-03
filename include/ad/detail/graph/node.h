#pragma once

#include <concepts>

namespace ad {

template <typename N>
concept node = requires(N n) {
    typename N::value_type;
    { n.value() } -> std::convertible_to<typename N::value_type>;
    { n.gradient(std::declval<typename N::value_type>()) };
};

template <typename T>
concept arithmetic = std::is_arithmetic_v<T>;

}
