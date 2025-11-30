#include <iostream>

#include <ad/detail/graph/addition.h>
#include <ad/detail/graph/constant.h>
#include <ad/detail/graph/variable.h>

auto main() -> int
{
    ad::constant c1 { 1. };
    ad::variable v2 { 2. };
    ad::constant c3 { 3. };

    auto a1 = c1 + v2;
    auto a2 = a1 + c3;

    std::cout << a1.value() << std::endl;
    std::cout << a2.value() << std::endl;

    a2.gradient(1);

    std::cout << v2.gradient() << std::endl;

    auto a3 = a1 + 4.;

    std::cout << a3.value() << std::endl;

    auto a4 = 5. + a3;

    std::cout << a4.value() << std::endl;

    return 0;
}
