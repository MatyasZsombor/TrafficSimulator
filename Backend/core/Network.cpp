//
// Created by Zsombor Matyas on 2/8/26.
//

#include "Network.h"

#include <iostream>
#include <limits>

double Network::minDx() const {
    double minDx = std::numeric_limits<double>::max();

    for (const auto& road : roads_) {
        if (road.dx() < minDx) {
            minDx = road.dx();
        }
    }

    return minDx;
}

double Network::maxSpeed() const {
    double maxV = 0.0;

    for (const auto& road : roads_) {
        if (road.maxSpeed() > maxV) {
            maxV = road.maxSpeed();
        }
    }

    return maxV;
}

double Network::cfl(const double minDx, const double maxSpeed) {
    return minDx / maxSpeed;
}

Network::Network(const std::vector<Road>& roads) {
    std::vector<Road> r;
    r.reserve(roads.size());

    for (const Road& road : roads) {
        r.push_back(road);
    }

    roads_ = r;
}

void Network::update() {
    const double dt = cfl(minDx(), maxSpeed());

    for (Road& road : roads_) {
        road.update(dt);
    }
}
