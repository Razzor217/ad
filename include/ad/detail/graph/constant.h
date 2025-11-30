#pragma once

namespace ad {

template <typename T> class constant {
public:
    using value_type = T;

    auto value() const -> value_type { return m_value; }
    void gradient(value_type const) { }
    auto gradient() const -> value_type { return 0; }

    explicit constant(value_type const v)
        : m_value { v }
    {
    }

private:
    value_type m_value {};
};

}
