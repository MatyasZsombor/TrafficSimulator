#include <fstream>
#include <iostream>
#include <random>

#include "Network.h"
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

std::vector<double> randomInitialDensity(const int cellCount, double const maxDensity) {
    std::vector<double> density(cellCount);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution dis(0.01 * maxDensity, 0.05 * maxDensity);

    for (int i = 0; i < cellCount; i++) {
        density[i] = dis(gen);
    }

    return density;
}

int main() {
    std::ofstream outFile("densities.txt");

    if (!outFile) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    constexpr auto length = 1000;
    constexpr auto cellCount = 100;
    constexpr auto maxDensity = 0.12;

    auto road = Road(1, length, cellCount, maxDensity, 30, 1.0);
    const auto density = randomInitialDensity(cellCount, maxDensity);

    road.init(density);

    std::vector<Road> roads;
    roads.push_back(road);

    Network network(roads);

    for (auto i = 0; i < 1000; i++) {
        network.update();
        outFile << network;
    }

    outFile.close();
}