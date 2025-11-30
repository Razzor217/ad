#pragma once

#include <ad/detail/graph/constant.h>
#include <ad/detail/graph/node.h>

#include <cmath>

namespace ad {

template <node A> class cos_impl {
public:
    using argument_type = A;
    using argument_reference = argument_type&;
    using value_type = typename argument_type::value_type;

    auto value() const -> value_type { return m_value; }

    void gradient(value_type const adjoint) { m_arg.gradient(adjoint * std::sin(m_arg.value)); }

    explicit cos_impl(argument_reference& arg)
        : m_arg { arg }
        , m_value { std::cos(m_arg.value()) }
    {
    }

private:
    argument_reference m_arg {};
    value_type m_value {};
};

template <node A> auto cos(A& arg) -> cos_impl<A> { return cos_impl { arg }; }
template <typename T> auto cos(T const arg) -> cos_impl<constant<T>>
{
    constant c { arg };
    return cos_impl { c };
}

}
