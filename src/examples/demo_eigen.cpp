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

    auto g = []<typename T>(Eigen::Matrix<T, 6, 1> const& x, Eigen::Matrix<T, 3, 1> const& u, T const dt) {
        Eigen::Matrix<T, 6, 1> f { x };
        f.template segment<3>(3) += dt * u;
        f.template segment<3>(0) += dt * f.template segment<3>(3);
        return f;
    };

    std::cout << ad::jacobian(
        g, Eigen::Matrix<double, 6, 1> { Eigen::Matrix<double, 6, 1>::Zero() }, Eigen::Vector3d { 2., 2., 2. }, .01)
              << std::endl;

    return 0;
}
