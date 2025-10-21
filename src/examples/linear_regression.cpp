#include <iostream>
#include <random>

#include <ad/differentiation.h>
#include <ad/dual.h>

auto main() -> int
{
    auto f = []<typename T>(T x) { return 1.4 * x - .7; };

    std::random_device rd {};
    std::mt19937 gen { rd() };
    std::uniform_real_distribution<double> x_dist { 0., 1. };
    std::uniform_real_distribution<double> noise_dist { -1., 1. };

    constexpr std::size_t N = 500;
    constexpr std::size_t training_iterations = 10000;

    std::vector<double> xs(N);
    std::vector<double> ys(N);

    for (std::size_t i {}; i < N; ++i) {
        xs[i] = x_dist(gen);
        ys[i] = f(xs[i]) + noise_dist(gen);
    }

    auto loss = [&xs, &ys]<typename T>(std::vector<T> const& vs) {
        T l = 0.;
        for (std::size_t i {}; i < N; ++i)
            l += ad::pow(ys[i] - (vs[0] * xs[i] + vs[1]), 2.);
        return .002 * l;
    };

    auto a = .1;
    auto b = 0.;
    auto learning_rate = .1;

    for (std::size_t i {}; i < training_iterations; ++i) {
        auto grad = ad::gradient(loss, std::vector { a, b });

        a -= learning_rate * grad[0];
        b -= learning_rate * grad[1];
    }

    std::cout << "y = " << a << " * x + " << b << std::endl;
    return 0;
}