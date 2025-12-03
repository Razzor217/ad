#pragma once

#include <ad/detail/graph/constant.h>
#include <ad/detail/graph/node.h>

#include <cmath>
#include <memory>

namespace ad {

template <node A> class exp_impl {
public:
    using argument_type = A;
    using argument_pointer = std::shared_ptr<argument_type>;
    using value_type = typename argument_type::value_type;

    auto value() const -> value_type { return m_value; }

    void gradient(value_type const adjoint) { m_arg->gradient(adjoint * m_value); }

    explicit exp_impl(argument_pointer& arg)
        : m_arg { std::move(arg) }
        , m_value { std::exp(m_arg->value()) }
    {
    }

private:
    argument_pointer m_arg {};
    value_type m_value {};
};

template <node A> using exp_ptr = std::shared_ptr<exp_impl<A>>;
template <node A> auto exp(std::shared_ptr<A> const& arg) -> exp_ptr<A> { return std::make_shared<exp_impl<A>>(arg); }
template <arithmetic T> auto exp(T const arg) -> constant_ptr<T>
{
    return std::make_shared<constant_impl<T>>(std::exp(arg));
}

}
