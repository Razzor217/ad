#pragma once

#include <ad/detail/differentiation/forward.h>
#include <ad/detail/dual/eigen.h>

namespace ad::detail {

template <typename F, typename T, int R, int C> struct eigen_function_traits_nd {
    using argument_type = Eigen::Matrix<T, R, C>;
    using return_type = decltype(std::declval<F>()(std::declval<argument_type>()));

    constexpr static int out_rows = return_type::RowsAtCompileTime;
    constexpr static int out_cols = return_type::ColsAtCompileTime;
};

template <typename F, typename T, int X, int U> struct eigen_function_traits_xu {
    using x_type = Eigen::Matrix<T, X, 1>;
    using u_type = Eigen::Matrix<T, U, 1>;
    using return_type = decltype(std::declval<F>()(std::declval<x_type>(), std::declval<u_type>(), std::declval<T>()));

    constexpr static int out_rows = return_type::RowsAtCompileTime;
    constexpr static int out_cols = return_type::ColsAtCompileTime;
};

}
namespace ad {

template <typename F, typename T, int R, int C>
concept eigen_function = requires(F f, Eigen::Matrix<T, R, C> X) {
    { f(X) } -> std::convertible_to<T>;
};

template <typename F, typename T, int R, int C>
concept eigen_function_nd = R != Eigen::Dynamic && C != Eigen::Dynamic && requires(F f, Eigen::Matrix<T, R, C> X) {
    requires detail::eigen_function_traits_nd<F, T, R, C>::out_rows != Eigen::Dynamic;
    requires detail::eigen_function_traits_nd<F, T, R, C>::out_cols != Eigen::Dynamic;
    requires std::is_same_v<typename decltype(f(X))::Scalar, T>;
};

template <typename F, typename T, int X, int U>
concept eigen_function_xu = X != Eigen::Dynamic && U != Eigen::Dynamic
    && requires(F f, Eigen::Matrix<T, X, 1> x, Eigen::Matrix<T, U, 1> u, T dt) {
           requires detail::eigen_function_traits_xu<F, T, X, U>::out_rows != Eigen::Dynamic;
           requires detail::eigen_function_traits_xu<F, T, X, U>::out_cols != Eigen::Dynamic;
           requires std::is_same_v<typename decltype(f(x, u, dt))::Scalar, T>;
       };

template <typename F, typename T, int M>
    requires eigen_function<F, T, M, 1>
auto partial(F f, Eigen::Matrix<T, M, 1> const& x, std::size_t const wrt) -> Dual<T>
{
    Eigen::Matrix<Dual<T>, M, 1> dX { Eigen::Matrix<Dual<T>, M, 1>::Constant(Dual<T> {}) };
    for (int i {}; i < M; ++i)
        dX(i) = { x(i), (i == wrt) ? T { 1. } : T { 0. } };

    return f(dX).dual;
}

template <typename F, typename T, int M>
    requires eigen_function<F, T, M, 1>
auto gradient(F f, Eigen::Matrix<T, M, 1> const& x) -> Eigen::Matrix<T, 1, M>
{
    Eigen::Matrix<T, 1, M> grad { Eigen::Matrix<T, 1, M>::Zero() };
    for (int i {}; i < M; ++i)
        grad(i) = partial(f, x, i);

    return grad;
}

template <typename F, typename T, int M>
    requires eigen_function_nd<F, T, M, 1>
auto jacobian(F f, Eigen::Matrix<T, M, 1> const& X)
{
    constexpr int N = detail::eigen_function_traits_nd<F, T, M, 1>::out_rows;
    Eigen::Matrix<T, N, M> J { Eigen::Matrix<T, N, M>::Zero() };

    for (int m {}; m < M; ++m) {
        Eigen::Matrix<Dual<T>, M, 1> dX { Eigen::Matrix<Dual<T>, M, 1>::Constant(Dual<T> {}) };
        for (int i {}; i < M; ++i)
            dX(i) = { X(i), (i == m) ? T { 1. } : T { 0. } };

        auto dY = f(dX);

        for (int n {}; n < N; ++n)
            J(n, m) = dY(n).dual;
    }

    return J;
}

template <typename F, typename T, int X, int U>
    requires eigen_function_xu<F, T, X, U>
auto jacobian(F f, Eigen::Matrix<T, X, 1> const& x, Eigen::Matrix<T, U, 1> const& u, T const dt)
{
    constexpr int N = detail::eigen_function_traits_xu<F, T, X, U>::out_rows;
    Eigen::Matrix<T, N, X> J { Eigen::Matrix<T, N, X>::Zero() };

    Eigen::Matrix<Dual<T>, U, 1> du { Eigen::Matrix<Dual<T>, U, 1>::Constant(Dual<T> {}) };
    for (int i {}; i < U; ++i)
        du(i) = { u(i) };

    Dual<T> const ddt { dt };

    for (int ix {}; ix < X; ++ix) {
        Eigen::Matrix<Dual<T>, X, 1> dx { Eigen::Matrix<Dual<T>, X, 1>::Constant(Dual<T> {}) };
        for (int i {}; i < X; ++i)
            dx(i) = { x(i), (i == ix) ? T { 1. } : T { 0. } };

        auto df = f(dx, du, ddt);

        for (int n {}; n < N; ++n)
            J(n, ix) = df(n).dual;
    }

    return J;
}
}
