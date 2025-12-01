#pragma once

#include <ad/detail/graph/constant.h>
#include <ad/detail/graph/node.h>

#include <cmath>

namespace ad {

template <node A> class exp_impl {
public:
    using argument_type = A;
    using argument_reference = argument_type&;
    using value_type = typename argument_type::value_type;

    auto value() const -> value_type { return m_value; }

    void gradient(value_type const adjoint) { m_arg.gradient(adjoint * m_value); }

    explicit exp_impl(argument_reference& arg)
        : m_arg { arg }
        , m_value { std::exp(m_arg.value()) }
    {
    }

private:
    argument_reference m_arg {};
    value_type m_value {};
};

template <node A> auto exp(A& arg) -> exp_impl<A> { return exp_impl { arg }; }
template <typename T> auto exp(T const arg) -> constant<T> { return constant { std::exp(arg) }; }

}
