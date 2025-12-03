#include <iostream>

#include <ad/graph.h>

auto main() -> int
{
    auto f = [](auto x) { return ad::pow(ad::sin(x), ad::sin(x)); };
    auto x = ad::variable(M_PI_4);
    f(x)->gradient(1.);
    std::cout << x->gradient() << std::endl;

    auto g = [](auto x) { return ad::pow(x, 2.) * ad::pow(2., x); };
    x = ad::variable(.5);
    g(x)->gradient(1.);
    std::cout << x->gradient() << std::endl;

    auto h = []<typename T>(std::array<T, 3> const& xs) {
        auto [x, y, z] = xs;
        return ad::sin(ad::pow(x, y + z)) - 3. * z * ad::log(ad::pow(x, 2.) * ad::pow(y, 3.));
    };

    std::array xs { ad::variable(.5), ad::variable(4.), ad::variable(-2.3) };
    auto [a, b, c] = xs;
    h(xs)->gradient(1.);
    std::cout << b->gradient() << std::endl;

    return 0;
}
