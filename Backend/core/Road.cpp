//
// Created by Zsombor Matyas on 2/8/26.
//

#include "Road.h"

#include <algorithm>
#include <complex>
#include <numeric>

double Road::godunovFlux(const double densityL, const double maxSpeedL,  const double densityR, const double maxSpeedR, const double maxDensity) {
    const double fluxL = greenShieldFlux(densityL, maxDensity, maxSpeedL);
    const double fluxR = greenShieldFlux(densityR, maxDensity, maxSpeedR);

    return densityL <= densityR ? std::min(fluxL, fluxR) : std::max(fluxL, fluxR);
}

double Road::densityFromFlow(const double flow, const double maxFlow, const double maxDensity) {
    const double discriminator = 1.0 - flow / maxFlow;

    if (discriminator < 0) {
        return maxDensity / 2.0;
    }

    return 0.5 * maxDensity * (1.0 - std::sqrt(discriminator));
}

double Road::greenShieldFlux(const double density, const double maxDensity, const double maxSpeed) {
    return density * maxSpeed * (1.0 - density / maxDensity);
}

Road::Road(const int id, const double length, const int cellCount, const double maxDensity, const double defaultSpeedLimit, const double inflow) {
    id_ = id;

    length_ = length;
    cellCount_ = cellCount;
    dx_ = length_ / cellCount_;

    maxDensity_ = maxDensity;

    speedLimits_ = std::vector(cellCount_, defaultSpeedLimit);
    maxFlows_ = std::vector(cellCount_, defaultSpeedLimit * maxDensity / 4);

    density_ = std::vector(cellCount_, 0.0);
    inflow_ = inflow;
}

double Road::maxSpeed() const {
    return *std::ranges::max_element(speedLimits_);
}

double Road::dx() const {
    return dx_;
}

void Road::init(const std::vector<double> &density) {
    for (int i = 0; i < cellCount_; i++) {
        density_[i] = density[i];
    }
}

void Road::init(const std::vector<SpeedZone> &speedLimits) {
    for (const auto&[from, to, limit] : speedLimits) {
        for (int i = from + 1; i <= to; i++) {
            speedLimits_[i] = limit;
            maxFlows_[i] = limit * maxDensity_ / 4;
        }
    }
}

void Road::init(const std::vector<double> &density, const std::vector<SpeedZone> &speedLimits) {
    init(density);
    init(speedLimits);
}


double Road::averageDensity() const {
    return std::reduce(density_.begin(), density_.end()) / cellCount_;
}

std::vector<double> Road::flux() const {
    std::vector<double> flux(cellCount_ + 1);

    const double densityIn = densityFromFlow(inflow_, maxFlows_[0], maxDensity_);
    const double speedLimitIn = speedLimits_[0];

    flux[0] = godunovFlux(densityIn, speedLimitIn, density_[0], speedLimitIn, maxDensity_);

    for (int i = 0; i < cellCount_ - 1; i++) {
        flux[i + 1] = godunovFlux(density_[i], speedLimits_[i], density_[i + 1], speedLimits_[i + 1], maxDensity_);
    }

    constexpr double densityOut = 0.0;
    const double speedLimitOut = speedLimits_[cellCount_ - 1];
    flux[cellCount_] = godunovFlux(density_[cellCount_ - 1], speedLimitOut, densityOut, speedLimitOut, maxDensity_);

    return flux;
}

void Road::update(const double dt) {
    const auto f = flux();

    std::vector<double> newDensity(cellCount_);

    for (int i = 0; i < cellCount_; i++) {
        newDensity[i] = density_[i] - dt / dx_ * (f[i + 1] - f[i]);
    }

    density_ = newDensity;
}