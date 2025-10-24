#pragma once

#include <ad/detail/dual/dual.h>
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
concept dual_function = requires {
    requires function<F, T>;
    requires function<F, Dual<T>>;
};

template <typename F, typename T, std::size_t N>
concept dual_function_nd = requires {
    requires function_nd<F, T, N>;
    requires function_nd<F, Dual<T>, N>;
};

template <typename F, typename T>
    requires dual_function<F, T>
auto derivative(F f, T x) -> T
{
    return f(Dual { x, 1. }).dual;
}

template <typename F, typename T, std::size_t N>
    requires dual_function_nd<F, T, N>
auto partial(F f, std::array<T, N> const& xs, std::size_t const wrt) -> T
{
    std::array<Dual<T>, N> dxs {};
    for (std::size_t i {}; i < N; ++i)
        dxs[i] = Dual { xs[i], i == wrt ? T { 1. } : T { 0. } };

    return f(dxs).dual;
}

template <typename F, typename T, std::size_t N>
    requires dual_function_nd<F, T, N>
auto gradient(F f, std::array<T, N> const& xs) -> std::array<T, N>
{
    std::array<T, N> grad {};
    for (std::size_t i {}; i < N; ++i)
        grad[i] = partial(f, xs, i);

    return grad;
}

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