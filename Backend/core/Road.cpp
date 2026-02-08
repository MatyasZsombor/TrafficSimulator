//
// Created by Zsombor Matyas on 2/8/26.
//

#include "Road.h"

#include <complex>
#include <numeric>

double Road::godunovFlux(const double densityL, const double densityR) const {
    const double fluxL = greenShieldFlux(densityL);
    const double fluxR = greenShieldFlux(densityR);

    return densityL <= densityR ? std::min(fluxL, fluxR) : std::max(fluxL, fluxR);
}

double Road::densityFromFlow(const double flow) const {
    const double discriminator = 1.0 - flow / maxFlow_;

    if (discriminator < 0) {
        return maxDensity_ / 2.0;
    }

    return 0.5 * maxDensity_ * (1.0 - std::sqrt(discriminator));
}

double Road::greenShieldFlux(const double density) const {
    return density * maxSpeed_ * (1.0 - density / maxDensity_);
}

Road::Road(const int id, const double length, const int cellCount, const double maxDensity, const double maxSpeed, const double inflow) {
    id_ = id;

    length_ = length;
    cellCount_ = cellCount;
    dx_ = length_ / cellCount_;

    maxDensity_ = maxDensity;
    maxSpeed_ = maxSpeed;
    maxFlow_ = maxSpeed * maxDensity / 4;

    density_ = std::vector(cellCount_, 0.0);

    inflow_ = inflow;
}

double Road::maxSpeed() const {
    return maxSpeed_;
}

double Road::dx() const {
    return dx_;
}

void Road::init(const std::vector<double> &density) {
    for (int i = 0; i < cellCount_; i++) {
        density_[i] = density[i];
    }
}

double Road::averageDensity() const {
    return std::reduce(density_.begin(), density_.end()) / cellCount_;
}

std::vector<double> Road::flux() const {
    std::vector<double> flux(cellCount_ + 1);

    const double densityIn = densityFromFlow(inflow_);
    flux[0] = godunovFlux(densityIn, density_[0]);

    for (int i = 0; i < cellCount_ - 1; i++) {
        flux[i + 1] = godunovFlux(density_[i], density_[i + 1]);
    }

    constexpr double densityOut = 0.0;
    flux[cellCount_] = godunovFlux(density_[cellCount_ - 1], densityOut);

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