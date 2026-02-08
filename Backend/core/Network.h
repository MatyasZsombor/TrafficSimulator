//
// Created by Zsombor Matyas on 2/8/26.
//

#ifndef BACKEND_NETWORK_H
#define BACKEND_NETWORK_H
#include <vector>

#include "Road.h"

class Network {
    std::vector<Road> roads_;

    [[nodiscard]] double minDx() const;
    [[nodiscard]] double maxSpeed() const;
    static double cfl(double minDx, double maxSpeed);

public:
    explicit Network(const std::vector<Road>& roads);

    void update();

    friend std::ostream& operator<<(std::ostream& os, const Network& r) {
        for (const Road& road : r.roads_) {
            os << road << std::endl;
        }
        return os;
    }
};


#endif //BACKEND_NETWORK_H