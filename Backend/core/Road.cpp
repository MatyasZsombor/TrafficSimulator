//
// Created by Zsombor Matyas on 2/8/26.
//

#include "Road.h"

#include <iostream>
#include <numeric>

Road::Road(const int id, const double length, const int cellCount, const double maxDensity, const double maxSpeed) {
    _id = id;

    _length = length;
    _cellCount = cellCount;
    _dx = _length / _cellCount;

    _maxDensity = maxDensity;
    _maxSpeed = maxSpeed;
    _maxFlow = maxSpeed * maxDensity / 4;

    _density = std::vector(_cellCount, 0.0);
}

void Road::init(const std::vector<double> &density) {
    for (int i = 0; i < _cellCount; i++) {
        _density[i] = density[i];
    }
}

double Road::averageDensity() const {
    return std::reduce(_density.begin(), _density.end()) / _cellCount;
}

double Road::totalSpeed(const double density) const {
    return _maxSpeed * (1 - density / _maxDensity);
}

std::vector<double> Road::flux() const {
    std::vector<double> flux;
    flux.reserve(_cellCount);

    for (int i = 0; i < _cellCount; i++) {
        flux.push_back(totalSpeed(_density[i]) * _density[i]);
    }

    return flux;
}

void Road::print() const {
    std::cout << "Road " << _id << ":" << std::endl;
    std::cout << "Cell Count: " << _cellCount << std::endl;
    std::cout << "Max Density: " << _maxDensity << std::endl;
    std::cout << "Max Speed: " << _maxSpeed << std::endl;
    std::cout << "Max Flow: " << _maxFlow << std::endl << std::endl;

    std::cout << "Density: " << std::endl;
    for (int i = 0; i < _cellCount; i++) {
        std::cout << _density[i] << std::endl;
    }
}