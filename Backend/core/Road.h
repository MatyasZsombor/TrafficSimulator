//
// Created by Zsombor Matyas on 2/8/26.
//

#ifndef BACKEND_ROAD_H
#define BACKEND_ROAD_H
#include <iostream>
#include <vector>

#include "SpeedZone.h"

class Road {
    int id_;

    double length_;
    int cellCount_;
    double dx_;

    std::vector<double> density_;

    std::vector<double> speedLimits_;
    double maxDensity_;
    std::vector<double> maxFlows_;

    double inflow_;

    static double godunovFlux(double densityL, double maxSpeedL, double densityR, double maxSpeedR, double maxDensity);
    static double densityFromFlow(double flow, double maxFlow, double maxDensity);
    static double greenShieldFlux(double density, double maxDensity, double maxSpeed) ;

public:
    Road(int id, double length, int cellCount, double maxDensity, double defaultSpeedLimit, double inflow);

    [[nodiscard]] double maxSpeed() const;
    [[nodiscard]] double dx() const;

    void init(const std::vector<double> &density);
    void init(const std::vector<SpeedZone> &speedLimits);
    void init(const std::vector<double> &density, const std::vector<SpeedZone> &speedLimits);

    [[nodiscard]] double averageDensity() const;
    [[nodiscard]] std::vector<double> flux() const;
    void update(double dt);

    friend std::ostream& operator<<(std::ostream& os, const Road& r) {
        os << "Road " << r.id_ << ":" << std::endl;
        os << "Cell Count: " << r.cellCount_ << std::endl;
        os << "Max Density: " << r.maxDensity_ << std::endl;
        //os << "Max Speed: " << r.maxSpeed_ << std::endl;
        //os << "Max Flow: " << r.maxFlow_ << std::endl << std::endl;

        os << "Density: ";

        for (int i = 0; i < r.cellCount_; i++) {
            os << r.density_[i] << (i < r.cellCount_ - 1 ? "," : "");
        }

        os << std::endl;
        return os;
    }
};


#endif //BACKEND_ROAD_H