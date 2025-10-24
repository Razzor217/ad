#include <ad/dual.h>
#include <iostream>

auto main() -> int
{
    ad::Dual x { 2., 1. };

    std::cout << x << std::endl;
}
