
#pragma once

#include <ad/detail/graph/constant.h>
#include <ad/detail/graph/node.h>
#include <utility>

namespace ad {

template <node L, node R> class division {
public:
    using lhs_type = L;
    using rhs_type = R;
    using lhs_reference = lhs_type&;
    using rhs_reference = rhs_type&;
    using value_type
        = decltype(std::declval<typename lhs_type::value_type>() / std::declval<typename rhs_type::value_type>());

    auto value() const -> value_type { return m_value; }

    void gradient(value_type const adjoint)
    {
        m_lhs.gradient(adjoint / m_rhs.value());
        m_rhs.gradient((-adjoint * m_lhs.value()) / (m_rhs.value() * m_rhs.value()));
    }

    explicit division(lhs_reference& lhs, rhs_reference& rhs)
        : m_lhs { lhs }
        , m_rhs { rhs }
        , m_value { m_lhs.value() / m_rhs.value() }
    {
    }

private:
    lhs_reference m_lhs {};
    rhs_reference m_rhs {};

    value_type m_value {};
};

template <node L, node R> auto operator/(L& lhs, R& rhs) -> division<L, R> { return division { lhs, rhs }; }
template <node L>
auto operator/(L& lhs, typename L::value_type const rhs) -> division<L, constant<typename L::value_type>>
{
    constant c { rhs };
    return division { lhs, c };
}
template <node R>
auto operator/(typename R::value_type const lhs, R& rhs) -> division<constant<typename R::value_type>, R>
{
    constant c { lhs };
    return division { c, rhs };
}

}