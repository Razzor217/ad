#include <iostream>

#include <ad/differentiation.h>
#include <ad/dual.h>

auto main() -> int
{
    auto f = [](auto x) { return ad::pow(ad::sin(x), ad::sin(x)); };
    std::cout << ad::derivative(f, M_PI_4) << std::endl;
    std::cout << ad::numeric_derivative(f, M_PI_4) << std::endl;

    auto g = [](auto x) { return ad::pow(x, 2.) * ad::pow(2., x); };
    std::cout << ad::derivative(g, .5) << std::endl;
    std::cout << ad::numeric_derivative(g, .5) << std::endl;

    auto h = []<typename T>(std::array<T, 3> const& xs) {
        auto [x, y, z] = xs;
        return ad::sin(ad::pow(x, y + z)) - 3. * z * ad::log(ad::pow(x, 2.) * ad::pow(y, 3.));
    };

    std::array xs { .5, 4., -2.3 };
    std::cout << ad::partial(h, xs, 1) << std::endl;
    std::cout << ad::numeric_partial(h, xs, 1) << std::endl;

    return 0;
}
