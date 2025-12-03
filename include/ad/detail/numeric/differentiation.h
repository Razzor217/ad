#pragma once

#include <array>
#include <concepts>

namespace ad {

template <typename F, typename T>
concept function = requires(F f, T x) {
    { f(x) } -> std::convertible_to<T>;
};

template <typename F, typename T, std::size_t N>
concept function_nd = requires(F f, std::array<T, N> const& xs) {
    { f(xs) } -> std::convertible_to<T>;
};

template <typename F, typename T>
    requires function<F, T>
auto numeric_derivative(F f, T x, T h = { 1e-7 }) -> T
{
    return (f(x + h) - f(x)) / h;
}

template <typename F, typename T, std::size_t N>
    requires function_nd<F, T, N>
auto numeric_partial(F f, std::array<T, N> const& xs, std::size_t const wrt, T h = { 1e-7 }) -> T
{
    std::array dxs { xs };
    dxs[wrt] += h;
    return (f(dxs) - f(xs)) / h;
}

}