#pragma once

#include <cassert>
#include <ostream>
#include <type_traits>

namespace ad {

template <typename T>
concept arithmetic = std::is_arithmetic_v<T>;

template <typename T>
    requires arithmetic<T>
struct Dual {
    using value_type = T;

    value_type real {};
    value_type dual {};

    Dual(value_type r = {}, value_type d = {})
        : real { r }
        , dual { d }
    {
    }

    Dual(Dual const&) = default;
    Dual(Dual&&) = default;
    auto operator=(Dual const&) -> Dual& = default;
    auto operator=(Dual&&) -> Dual& = default;
};

template <typename T>
    requires arithmetic<T>
auto operator+(Dual<T> const& left, Dual<T> const& right) -> Dual<T>
{
    return { left.real + right.real, left.dual + right.dual };
}

template <typename T>
    requires arithmetic<T>
auto operator+(Dual<T> const& left, T const right) -> Dual<T>
{
    return { left.real + right, left.dual };
}

template <typename T>
    requires arithmetic<T>
auto operator+(T const left, Dual<T> const& right) -> Dual<T>
{
    return right + left;
}

template <typename T>
    requires arithmetic<T>
auto operator+=(Dual<T>& left, Dual<T> const& right) -> Dual<T>&
{
    return left = left + right;
}

template <typename T>
    requires arithmetic<T>
auto operator+=(Dual<T>& left, T const right) -> Dual<T>&
{
    return left = left + right;
}

template <typename T>
    requires arithmetic<T>
auto operator-(Dual<T> const& left, Dual<T> const& right) -> Dual<T>
{
    return { left.real - right.real, left.dual - right.dual };
}

template <typename T>
    requires arithmetic<T>
auto operator-(Dual<T> const& left, T const right) -> Dual<T>
{
    return { left.real - right, left.dual };
}

template <typename T>
    requires arithmetic<T>
auto operator-(T const left, Dual<T> const& right) -> Dual<T>
{
    return { left - right.real, -right.dual };
}

template <typename T>
    requires arithmetic<T>
auto operator*(Dual<T> const& left, Dual<T> const& right) -> Dual<T>
{
    return { left.real * right.real, left.real * right.dual + left.dual * right.real };
}

template <typename T>
    requires arithmetic<T>
auto operator*(Dual<T> const& left, T const right) -> Dual<T>
{
    return { left.real * right, left.dual * right };
}

template <typename T>
    requires arithmetic<T>
auto operator*(T const left, Dual<T> const& right) -> Dual<T>
{
    return right * left;
}

template <typename T>
    requires arithmetic<T>
auto operator/(Dual<T> const& left, Dual<T> const& right) -> Dual<T>
{
    assert(right.real != T {});
    return { left.real / right.real, (left.dual * right.real - left.real * right.dual) / (right.real * right.real) };
}

template <typename T>
    requires arithmetic<T>
auto operator/(Dual<T> const& left, T const right) -> Dual<T>
{
    assert(right != T {});
    return { left.real / right, left.dual / right };
}

template <typename T>
    requires arithmetic<T>
auto operator/(T const left, Dual<T> const& right) -> Dual<T>
{
    assert(right.real != T {});
    return { left / right.real, (-left * right.dual) / (right.real * right.real) };
}

template <typename T>
    requires arithmetic<T>
auto operator<<(std::ostream& out, Dual<T> const& v) -> std::ostream&
{
    return out << '(' << v.real << ", " << v.dual << ')';
}

}