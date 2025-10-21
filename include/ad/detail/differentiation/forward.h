#pragma once

#include <ad/detail/dual/dual.h>
#include <concepts>
#include <vector>

namespace ad::detail {

template <typename T, std::size_t... Indices> constexpr auto make_n_tuple_impl(std::index_sequence<Indices...>)
{
    return std::tuple { ((void)Indices, T {})... };
}

template <typename T, std::size_t N> constexpr auto make_n_tuple()
{
    return make_n_tuple_impl<T>(std::make_index_sequence<N> {});
}

}

namespace ad {

template <typename F, typename T>
concept function = requires(F f, T x) {
    { f(x) } -> std::convertible_to<T>;
};

template <typename F, typename T>
concept function_nd = requires(F f, std::vector<T> const& xs) {
    { f(xs) } -> std::convertible_to<T>;
};

template <typename F, typename T>
concept dual_function = requires {
    requires function<F, T>;
    requires function<F, Dual<T>>;
};

template <typename F, typename T>
concept dual_function_nd = requires {
    requires function_nd<F, T>;
    requires function_nd<F, Dual<T>>;
};

template <typename F, typename T>
    requires dual_function<F, T>
auto derivative(F f, T x) -> T
{
    return f(Dual { x, 1. }).dual;
}

template <typename F, typename T>
    requires dual_function_nd<F, T>
auto partial(F f, std::vector<T> const& xs, std::size_t const wrt) -> T
{
    std::vector<Dual<T>> dxs(xs.size());
    for (std::size_t i {}; i < xs.size(); ++i)
        dxs[i] = Dual { xs[i], i == wrt ? 1. : 0. };

    return f(dxs).dual;
}

template <typename F, typename T>
    requires dual_function_nd<F, T>
auto gradient(F f, std::vector<T> const& xs) -> std::vector<T>
{
    std::vector<T> grad(xs.size());
    for (std::size_t i {}; i < xs.size(); ++i)
        grad[i] = partial(f, xs, i);

    return grad;
}

template <typename F, typename T>
    requires function<F, T>
auto numeric_derivative(F f, T x, T h = { 1e-7 }) -> T
{
    return (f(x + h) - f(x)) / h;
}

template <typename F, typename T>
    requires function_nd<F, T>
auto numeric_partial(F f, std::vector<T> const& xs, std::size_t const wrt, T h = { 1e-7 }) -> T
{
    std::vector dxs { xs };
    dxs[wrt] += h;
    return (f(dxs) - f(xs)) / h;
}

}