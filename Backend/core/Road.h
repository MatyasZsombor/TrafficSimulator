//
// Created by Zsombor Matyas on 2/8/26.
//

#ifndef BACKEND_ROAD_H
#define BACKEND_ROAD_H
#include <vector>


class Road {
    int _id;

    double _length;
    int _cellCount;
    double _dx;
    std::vector<double> _density;
    double _maxDensity;
    double _maxSpeed;
    double _maxFlow;

public:
    Road(int id, double length, int cellCount, double maxDensity, double maxSpeed);

    void init(const std::vector<double> &density);

    [[nodiscard]] double averageDensity() const;
    [[nodiscard]] double totalSpeed(double density) const;
    [[nodiscard]] std::vector<double> flux() const;

    void print() const;
};


#endif //BACKEND_ROAD_H