#pragma once

#include <ad/detail/graph/constant.h>
#include <ad/detail/graph/node.h>

#include <cmath>
#include <utility>

namespace ad {

template <node B, node E> class pow_impl {
public:
    using base_type = B;
    using exponent_type = E;
    using base_reference = base_type&;
    using exponent_reference = exponent_type&;
    using value_type = decltype(std::exp(
        std::declval<typename base_type::value_type>(), std::declval<typename exponent_type::value_type>()));

    auto value() const -> value_type { return m_value; }

    void gradient(value_type const adjoint)
    {
        // to-do: add handling of edge cases (base == 0, base < 0)
        m_base.gradient(adjoint * m_value * m_exponent.value() / m_base.value());
        m_exponent.gradient(adjoint * m_value * std::log(m_base.value()));
    }

    explicit pow_impl(base_reference& base, exponent_reference& exponent)
        : m_base { base }
        , m_exponent { exponent }
        , m_value { std::exp(m_base.value(), m_exponent.value()) }
    {
    }

private:
    base_reference m_base {};
    exponent_reference m_exponent {};

    value_type m_value {};
};

template <node B, node E> auto pow(B& base, E& exponent) -> pow_impl<B, E> { return pow_impl { base, exponent }; }
template <node B>
auto pow(B& base, typename B::value_type const exponent) -> pow_impl<B, constant<typename B::value_type>>
{
    constant c { exponent };
    return pow_impl { base, c };
}
template <node E>
auto pow(typename E::value_type const base, E& exponent) -> pow_impl<constant<typename E::value_type>, E>
{
    constant c { base };
    return pow_impl { c, exponent };
}
template <typename T> auto pow(T const base, T const exponent) -> constant<T>
{
    return constant { std::exp(base, exponent) };
}

}
