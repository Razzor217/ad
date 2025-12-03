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
    using base_pointer = std::shared_ptr<base_type>;
    using exponent_pointer = std::shared_ptr<exponent_type>;
    using value_type = decltype(std::pow(
        std::declval<typename base_type::value_type>(), std::declval<typename exponent_type::value_type>()));

    auto value() const -> value_type { return m_value; }

    void gradient(value_type const adjoint)
    {
        // to-do: add handling of edge cases (base == 0, base < 0)
        m_base->gradient(adjoint * m_value * m_exponent->value() / m_base->value());
        m_exponent->gradient(adjoint * m_value * std::log(m_base->value()));
    }

    explicit pow_impl(base_pointer const& base, exponent_pointer const& exponent)
        : m_base { std::move(base) }
        , m_exponent { std::move(exponent) }
        , m_value { std::pow(m_base->value(), m_exponent->value()) }
    {
    }

private:
    base_pointer m_base {};
    exponent_pointer m_exponent {};

    value_type m_value {};
};

template <node B, node E> using pow_ptr = std::shared_ptr<pow_impl<B, E>>;
template <node B, node E> auto pow(std::shared_ptr<B> const& base, std::shared_ptr<E> const& exponent) -> pow_ptr<B, E>
{
    return std::make_shared<pow_impl<B, E>>(base, exponent);
}
template <node B>
    requires arithmetic<typename B::value_type>
auto pow(std::shared_ptr<B> const& base, typename B::value_type const exponent)
    -> pow_ptr<B, constant_impl<typename B::value_type>>
{
    return std::make_shared<pow_impl<B, constant_impl<typename B::value_type>>>(
        base, std::make_shared<constant_impl<typename B::value_type>>(exponent));
}
template <node E>
    requires arithmetic<typename E::value_type>
auto pow(typename E::value_type const base, std::shared_ptr<E> const& exponent)
    -> pow_ptr<constant_impl<typename E::value_type>, E>
{
    return std::make_shared<pow_impl<constant_impl<typename E::value_type>, E>>(
        std::make_shared<constant_impl<typename E::value_type>>(base), exponent);
}
template <arithmetic T> auto pow(T const base, T const exponent) -> constant_ptr<T>
{
    return std::make_shared<constant_impl>(std::exp(base, exponent));
}

}
