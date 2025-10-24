#pragma once

#include <ad/detail/dual/dual.h>

#include <cmath>

namespace ad {

template <typename T>
    requires arithmetic<T>
auto pow(T const base, T const exp) -> T
{
    return std::pow(base, exp);
}

template <typename T>
    requires arithmetic<T>
auto pow(Dual<T> const& base, Dual<T> const& exp) -> Dual<T>
{
    if (base.dual == T {} && exp.dual == T {})
        return { std::pow(base.real, exp.real) };

    if (base.dual == T {})
        return { std::pow(base.real, exp.real), std::pow(base.real, exp.real) * std::log(base.real) * exp.dual };

    if (exp.dual == T {})
        return { std::pow(base.real, exp.real), std::pow(base.real, exp.real - 1.) * exp.real * base.dual };

    return { std::pow(base.real, exp.real),
        std::pow(base.real, exp.real - 1.) * (base.real * exp.dual * std::log(base.real) / base.dual + exp.real)
            * base.dual };
}

template <typename T>
    requires arithmetic<T>
auto pow(Dual<T> const& base, T const exp) -> Dual<T>
{
    return { std::pow(base.real, exp), std::pow(base.real, exp - 1.) * exp * base.dual };
}

template <typename T>
    requires arithmetic<T>
auto pow(T const base, Dual<T> const& exp) -> Dual<T>
{
    return { std::pow(base, exp.real), std::pow(base, exp.real) * std::log(base) * exp.dual };
}

template <typename T>
    requires arithmetic<T>
auto sqrt(T const x) -> T
{
    return std::sqrt(x);
}

template <typename T>
    requires arithmetic<T>
auto sqrt(Dual<T> const& x) -> Dual<T>
{
    return { std::sqrt(x.real), x.dual / (2. * std::sqrt(x.real)) };
}

template <typename T>
    requires arithmetic<T>
auto exp(T const x) -> T
{
    return std::exp(x);
}

template <typename T>
    requires arithmetic<T>
auto exp(Dual<T> const& x) -> Dual<T>
{
    return { std::exp(x.real), std::exp(x.real) * x.dual };
}

template <typename T>
    requires arithmetic<T>
auto log(T const x) -> T
{
    return std::log(x);
}

template <typename T>
    requires arithmetic<T>
auto log(Dual<T> const& x) -> Dual<T>
{
    assert(x.real != T {});
    return { std::log(x.real), x.dual / x.real };
}

template <typename T>
    requires arithmetic<T>
auto sin(T const x) -> T
{
    return std::sin(x);
}

template <typename T>
    requires arithmetic<T>
auto sin(Dual<T> const& x) -> Dual<T>
{
    return { std::sin(x.real), std::cos(x.real) * x.dual };
}

template <typename T>
    requires arithmetic<T>
auto cos(T const x) -> T
{
    return std::cos(x);
}

template <typename T>
    requires arithmetic<T>
auto cos(Dual<T> const& x) -> Dual<T>
{
    return { std::cos(x.real), -std::sin(x.real) * x.dual };
}

template <typename T>
    requires arithmetic<T>
auto tan(T const x) -> T
{
    return std::tan(x);
}

template <typename T>
    requires arithmetic<T>
auto tan(Dual<T> const& x) -> Dual<T>
{
    return { std::tan(x.real), x.dual / (std::cos(x.real) * std::cos(x.real)) };
}

template <typename T>
    requires arithmetic<T>
auto sgn(T const x) -> T
{
    return (T {} < x) - (x < T {});
}

template <typename T>
    requires arithmetic<T>
auto abs(T const x) -> T
{
    return std::abs(x);
}

template <typename T>
    requires arithmetic<T>
auto abs(Dual<T> const& x) -> T
{
    return abs(x.real);
}

template <typename T>
    requires arithmetic<T>
auto abs2(Dual<T> const& x) -> T
{
    return x.real * x.real;
}

template <typename T>
    requires arithmetic<T>
auto real(Dual<T> const& x) -> T
{
    return x.real;
}

template <typename T>
    requires arithmetic<T>
auto imag(Dual<T> const& x) -> T
{
    return x.dual;
}

template <typename T>
    requires arithmetic<T>
auto conj(Dual<T> const& x) -> Dual<T>
{
    return { x.real, -x.dual };
}

}
