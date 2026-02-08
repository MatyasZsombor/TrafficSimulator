//
// Created by Zsombor Matyas on 2/8/26.
//

#ifndef BACKEND_ROAD_H
#define BACKEND_ROAD_H
#include <iostream>
#include <vector>

class Road {
    int id_;

    double length_;
    int cellCount_;
    double dx_;

    std::vector<double> density_;

    double maxDensity_;
    double maxSpeed_;
    double maxFlow_;

    double inflow_;

    [[nodiscard]] double godunovFlux(double densityL, double densityR) const;
    [[nodiscard]] double densityFromFlow(double flow) const;
    [[nodiscard]] double greenShieldFlux(double density) const;

public:
    Road(int id, double length, int cellCount, double maxDensity, double maxSpeed, double inflow);
    [[nodiscard]] double maxSpeed() const;
    [[nodiscard]] double dx() const;

    void init(const std::vector<double> &density);

    [[nodiscard]] double averageDensity() const;
    [[nodiscard]] std::vector<double> flux() const;
    void update(double dt);

    friend std::ostream& operator<<(std::ostream& os, const Road& r) {
        os << "Road " << r.id_ << ":" << std::endl;
        os << "Cell Count: " << r.cellCount_ << std::endl;
        os << "Max Density: " << r.maxDensity_ << std::endl;
        os << "Max Speed: " << r.maxSpeed_ << std::endl;
        os << "Max Flow: " << r.maxFlow_ << std::endl << std::endl;

        os << "Density: ";

        for (int i = 0; i < r.cellCount_; i++) {
            os << r.density_[i] << (i < r.cellCount_ - 1 ? "," : "");
        }

        os << std::endl;
        return os;
    }
};


#endif //BACKEND_ROAD_H