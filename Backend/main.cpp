#include <iostream>
#include <ostream>

#include "core/Road.h"


std::vector<double> gradient(double start, double end, int n) {
    std::vector<double> result;
    result.reserve(n);

    double step = (end - start) / (n - 1);

    for (int i = 0; i < n; ++i) {
        result.push_back(start + i * step);
    }

    return result;
}


int main() {
    constexpr auto length = 500;
    constexpr auto cellCount = 50;
    constexpr auto maxDensity = 100;

    auto road = Road(1, length, cellCount, maxDensity, 100);
    const auto density = gradient(0.2 * maxDensity, 0.8 * maxDensity, cellCount);

    road.init(density);
    road.print();
    std::cout << std::endl << "Avg density: " << road.averageDensity() << std::endl << std::endl;

    const auto flux = road.flux();

    std::cout << "Flux: " << std::endl;
    for (auto i = 0; i < cellCount; i++) {
        std::cout << flux[i] << std::endl;
    }
}