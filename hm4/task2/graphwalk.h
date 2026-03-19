#ifndef RANDOMWALK_H
#define RANDOMWALK_H

#include <vector>
#include <memory>
#include <random>
#include "distribution.h"

struct Trajectory {
    std::vector<double> x;
    std::vector<double> y;
    int crossings;
};

class RandomWalk {
public:
    RandomWalk(double h, int n, double startY, std::unique_ptr<Distribution> dist);

    Trajectory run(std::mt19937& rng);

private:
    double h;
    int n;
    double startY;
    std::unique_ptr<Distribution> dist;
};

#endif // RANDOMWALK_H