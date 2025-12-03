#pragma once

#include <ad/detail/graph/node.h>
#include <memory>

namespace ad {

template <arithmetic T> class variable_impl {
public:
    using value_type = T;

    auto value() const -> value_type { return m_value; }
    void gradient(value_type const adjoint) { m_gradient += adjoint; }
    auto gradient() const -> value_type { return m_gradient; }

    explicit variable_impl(value_type const v)
        : m_value { v }
    {
    }

private:
    value_type m_value {};
    value_type m_gradient {};
};

template <arithmetic T> using variable_ptr = std::shared_ptr<variable_impl<T>>;
template <arithmetic T> auto variable(T const value) -> variable_ptr<T>
{
    return std::make_shared<variable_impl<T>>(value);
}

}
