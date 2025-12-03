#pragma once

#include <ad/detail/dual/dual.h>
#include <ad/detail/numeric/differentiation.h>

namespace ad {

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

}