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

std::vector<SpeedZone> createSpeedLimits(const int cellCount) {
    std::vector<SpeedZone> zones;
    struct ZoneFraction {
        double startFrac;
        double endFrac;
        double speed;
    };

    std::vector<ZoneFraction> fractions = {
        {0.0, 0.10, 30},  // first 10% of road
        {0.10, 0.20, 20}, // slow zone
        {0.20, 0.30, 25}, // mild slowdown
        {0.30, 0.50, 30}, // normal highway
        {0.50, 0.60, 15}, // bottleneck / traffic light
        {0.60, 0.80, 30}, // highway
        {0.80, 0.90, 18}, // urban slowdown
        {0.90, 1.0, 30}   // end of road
    };

    for (const auto&[startFrac, endFrac, speed] : fractions) {
        int startIdx = static_cast<int>(std::round(startFrac * cellCount));
        int endIdx   = static_cast<int>(std::round(endFrac * cellCount)) - 1;

        if (startIdx < 0) startIdx = 0;
        if (endIdx >= cellCount) endIdx = cellCount - 1;

        zones.push_back({startIdx, endIdx, speed});
    }

    return zones;
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

    auto road = Road(1, length, cellCount, maxDensity, 30, .5);
    const auto density = randomInitialDensity(cellCount, maxDensity);
    const auto speedLimits = createSpeedLimits(cellCount);

    road.init(density, speedLimits);

    std::vector<Road> roads;
    roads.push_back(road);

    Network network(roads);

    for (auto i = 0; i < 3000; i++) {
        network.update();
        outFile << network;
    }

    outFile.close();
}
