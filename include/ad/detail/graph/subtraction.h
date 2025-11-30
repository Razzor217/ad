#pragma once

#include <ad/detail/graph/constant.h>
#include <ad/detail/graph/node.h>
#include <utility>

namespace ad {

template <typename L, typename R> class subtraction {
public:
    using lhs_type = L;
    using rhs_type = R;
    using value_type
        = decltype(std::declval<typename lhs_type::value_type>() - std::declval<typename rhs_type::value_type>());

    auto value() const -> value_type { return m_value; }

    void gradient(value_type const adjoint)
    {
        m_lhs.gradient(adjoint);
        m_rhs.gradient(-adjoint);
    }

    explicit subtraction(lhs_type const& lhs, rhs_type const& rhs)
        : m_lhs { lhs }
        , m_rhs { rhs }
        , m_value { m_lhs.value() - m_rhs.value() }
    {
    }

private:
    lhs_type m_lhs {};
    rhs_type m_rhs {};

    value_type m_value {};
};

template <node L, node R> auto operator-(L& lhs, R& rhs) -> subtraction<L, R> { return subtraction { lhs, rhs }; }
template <node L>
auto operator-(L& lhs, typename L::value_type const rhs) -> subtraction<L, constant<typename L::value_type>>
{
    constant c { rhs };
    return subtraction { lhs, c };
}
template <node R>
auto operator-(typename R::value_type const lhs, R& rhs) -> subtraction<constant<typename R::value_type>, R>
{
    constant c { lhs };
    return subtraction { c, rhs };
}

}