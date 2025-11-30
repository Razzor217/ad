#pragma once

namespace ad {

template <typename T> class variable {
public:
    using value_type = T;

    auto value() const -> value_type { return m_value; }
    void gradient(value_type const adjoint) { m_gradient += adjoint; }
    auto gradient() const -> value_type { return m_gradient; }

    explicit variable(value_type const v)
        : m_value { v }
    {
    }

private:
    value_type m_value {};
    value_type m_gradient {};
};

}
