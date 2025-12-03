
#pragma once

#include <ad/detail/graph/constant.h>
#include <ad/detail/graph/node.h>
#include <utility>

namespace ad {

template <node L, node R> class division_impl {
public:
    using lhs_type = L;
    using rhs_type = R;
    using lhs_pointer = std::shared_ptr<lhs_type>;
    using rhs_pointer = std::shared_ptr<rhs_type>;
    using value_type
        = decltype(std::declval<typename lhs_type::value_type>() / std::declval<typename rhs_type::value_type>());

    auto value() const -> value_type { return m_value; }

    void gradient(value_type const adjoint)
    {
        m_lhs->gradient(adjoint / m_rhs->value());
        m_rhs->gradient((-adjoint * m_lhs->value()) / (m_rhs->value() * m_rhs->value()));
    }

    explicit division_impl(lhs_pointer const& lhs, rhs_pointer const& rhs)
        : m_lhs { std::move(lhs) }
        , m_rhs { std::move(rhs) }
        , m_value { m_lhs->value() / m_rhs->value() }
    {
    }

private:
    lhs_pointer m_lhs {};
    rhs_pointer m_rhs {};

    value_type m_value {};
};

template <node L, node R> using division_ptr = std::shared_ptr<division_impl<L, R>>;

template <node L, node R>
auto operator/(std::shared_ptr<L> const& lhs, std::shared_ptr<R> const& rhs) -> division_ptr<L, R>
{
    return std::make_shared<division_impl<L, R>>(lhs, rhs);
}
template <node L>
    requires arithmetic<typename L::value_type>
auto operator/(std::shared_ptr<L> const& lhs, typename L::value_type const rhs)
    -> division_ptr<L, constant_impl<typename L::value_type>>
{
    return std::make_shared<division_impl<L, constant_impl<typename L::value_type>>>(
        lhs, std::make_shared<constant_impl<typename L::value_type>>(rhs));
}
template <node R>
    requires arithmetic<typename R::value_type>
auto operator/(typename R::value_type const lhs, std::shared_ptr<R> const& rhs)
    -> division_ptr<constant_impl<typename R::value_type>, R>
{
    return std::make_shared<division_impl<constant_impl<typename R::value_type>, R>>(
        std::make_shared<constant_impl<typename R::value_type>>(lhs), rhs);
}

}
