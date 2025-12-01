#pragma once

#include <ad/detail/graph/constant.h>
#include <ad/detail/graph/node.h>

#include <cmath>

namespace ad {

template <node A> class sqrt_impl {
public:
    using argument_type = A;
    using argument_reference = argument_type&;
    using value_type = typename argument_type::value_type;

    auto value() const -> value_type { return m_value; }

    void gradient(value_type const adjoint) { m_arg.gradient(adjoint * .5 / m_value); }

    explicit sqrt_impl(argument_reference& arg)
        : m_arg { arg }
        , m_value { std::sqrt(m_arg.value()) }
    {
    }

private:
    argument_reference m_arg {};
    value_type m_value {};
};

template <node A> auto sqrt(A& arg) -> sqrt_impl<A> { return sqrt_impl { arg }; }
template <typename T> auto sqrt(T const arg) -> constant<T> { return constant { std::sqrt(arg) }; }

}
