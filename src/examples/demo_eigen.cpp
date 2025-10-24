#include <iostream>

#include <ad/differentiation.h>
#include <ad/dual.h>

auto main() -> int
{
    auto f = []<typename T>(Eigen::Matrix<T, 3, 1> const& x) {
        auto A = Eigen::Matrix<T, 3, 3>::Constant({ 3. });
        return A * x;
    };

    std::cout << ad::jacobian(f, Eigen::Vector3d { 1., 2., 3. }) << std::endl;

    return 0;
}
