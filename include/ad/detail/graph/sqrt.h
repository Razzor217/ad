#pragma once

#include <ad/detail/graph/constant.h>
#include <ad/detail/graph/node.h>

#include <cmath>
#include <memory>

namespace ad {

template <node A> class sqrt_impl {
public:
    using argument_type = A;
    using argument_pointer = std::shared_ptr<argument_type>;
    using value_type = typename argument_type::value_type;

    auto value() const -> value_type { return m_value; }

    void gradient(value_type const adjoint) { m_arg->gradient(adjoint * .5 / m_value); }

    explicit sqrt_impl(argument_pointer const& arg)
        : m_arg { std::move(arg) }
        , m_value { std::sqrt(m_arg->value()) }
    {
    }

private:
    argument_pointer m_arg {};
    value_type m_value {};
};

template <node A> using sqrt_ptr = std::shared_ptr<sqrt_impl<A>>;
template <node A> auto sqrt(std::shared_ptr<A> const& arg) -> sqrt_ptr<A>
{
    return std::make_shared<sqrt_impl<A>>(arg);
}
template <arithmetic T> auto sqrt(T const arg) -> constant_ptr<T>
{
    return std::make_shared<constant_impl<T>>(std::sqrt(arg));
}

}
