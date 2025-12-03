#pragma once

#include <ad/detail/graph/constant.h>
#include <ad/detail/graph/node.h>

#include <cmath>

namespace ad {

template <node A> class tan_impl {
public:
    using argument_type = A;
    using argument_pointer = std::shared_ptr<argument_type>;
    using value_type = typename argument_type::value_type;

    auto value() const -> value_type { return m_value; }

    void gradient(value_type const adjoint)
    {
        m_arg->gradient(adjoint / (std::cos(m_arg->value()) * std::cos(m_arg->value())));
    }

    explicit tan_impl(argument_pointer const& arg)
        : m_arg { std::move(arg) }
        , m_value { std::tan(m_arg->value()) }
    {
    }

private:
    argument_pointer m_arg {};
    value_type m_value {};
};

template <node A> using tan_ptr = std::shared_ptr<tan_impl<A>>;
template <node A> auto tan(std::shared_ptr<A> const& arg) -> tan_ptr<A> { return std::make_shared<tan_impl<A>>(arg); }
template <arithmetic T> auto tan(T const arg) -> constant_ptr<T>
{
    return std::make_shared<constant_impl<T>>(std::tan(arg));
}

}
