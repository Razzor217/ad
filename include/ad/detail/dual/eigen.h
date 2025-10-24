#pragma once

#include <ad/detail/dual/dual.h>

#include <Eigen/Core>

namespace Eigen {

template <typename T> struct NumTraits<ad::Dual<T>> : GenericNumTraits<ad::Dual<T>> {
    using Real = ad::Dual<T>::value_type;
    using NonInteger = ad::Dual<typename NumTraits<Real>::NonInteger>;
    using Literal = typename NumTraits<Real>::Literal;

    enum {
        IsComplex = 0,
        IsInteger = NumTraits<Real>::IsInteger,
        IsSigned = NumTraits<Real>::IsSigned,
        RequireInitialization = 1,
        ReadCost = 2 * NumTraits<Real>::ReadCost,
        AddCost = 2 * NumTraits<Real>::AddCost,
        MulCost = 3 * NumTraits<Real>::MulCost + 1 * NumTraits<Real>::AddCost,
    };

    constexpr static auto epsilon() -> Real { return NumTraits<Real>::epsilon(); }
    constexpr static auto dummy_precision() -> Real { return NumTraits<Real>::dummy_precision(); }
    constexpr static auto highest() -> ad::Dual<Real> { return { NumTraits<Real>::highest() }; }
    constexpr static auto lowest() -> ad::Dual<Real> { return { NumTraits<Real>::lowest() }; }
    constexpr static auto digits() -> int { return NumTraits<Real>::digits(); }
    constexpr static auto digits10() -> int { return NumTraits<Real>::digits10(); }
    constexpr static auto max_digits10() -> int { return NumTraits<Real>::digits10(); }
    constexpr static auto min_exponent() -> int { return NumTraits<Real>::min_exponent(); }
    constexpr static auto max_exponent() -> int { return NumTraits<Real>::max_exponent(); }
    constexpr static auto infinity() -> ad::Dual<Real> { return { NumTraits<Real>::infinity() }; }
    constexpr static auto quiet_NaN() -> ad::Dual<Real> { return { NumTraits<Real>::quiet_NaN() }; }
};
}
