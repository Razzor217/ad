#pragma once

#include <ad/detail/graph/node.h>
#include <memory>

namespace ad {

template <arithmetic T> class constant_impl {
public:
    using value_type = T;

    auto value() const -> value_type { return m_value; }
    void gradient(value_type const) { }

    explicit constant_impl(value_type const v)
        : m_value { v }
    {
    }

private:
    value_type m_value {};
};

template <arithmetic T> using constant_ptr = std::shared_ptr<constant_impl<T>>;
template <arithmetic T> auto constant(T const value) -> constant_ptr<T>
{
    return std::make_shared<constant_impl<T>>(value);
}

}
