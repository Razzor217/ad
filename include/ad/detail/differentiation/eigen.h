#pragma once

#include <ad/detail/differentiation/forward.h>
#include <ad/detail/dual/eigen.h>

namespace ad::detail {

template <typename F, typename T, int R, int C> struct eigen_function_traits {
    using argument_type = Eigen::Matrix<T, R, C>;
    using return_type = decltype(std::declval<F>()(std::declval<argument_type>()));

    constexpr static int out_rows = return_type::RowsAtCompileTime;
    constexpr static int out_cols = return_type::ColsAtCompileTime;
};

}
namespace ad {

template <typename F, typename T, int R, int C>
concept eigen_function = requires(F f, Eigen::Matrix<T, R, C> X) {
    { f(X) } -> std::convertible_to<T>;
};

// template <typename F, typename T, int IR, int IC, int OR, int OC>
// concept eigen_function_nd = requires(F f, Eigen::Matrix<T, IR, IC> X) {
//     { f(X) } -> std::convertible_to<Eigen::Matrix<T, OR, OC>>;
// };

template <typename F, typename T, int R, int C>
concept eigen_function_nd = requires(F f, Eigen::Matrix<T, R, C> X) {
    typename decltype(f(X))::Scalar;
    requires std::same_as<typename decltype(f(X))::Scalar, T>;
    R != Eigen::Dynamic;
    C != Eigen::Dynamic;
    detail::eigen_function_traits<F, T, R, C>::out_rows != Eigen::Dynamic;
    detail::eigen_function_traits<F, T, R, C>::out_cols != Eigen::Dynamic;
};

template <typename F, typename T, int M>
    requires eigen_function_nd<F, T, M, 1>
auto jacobian(F f, Eigen::Matrix<T, M, 1> const& X)
{
    constexpr int N = detail::eigen_function_traits<F, T, M, 1>::out_rows;
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

}
