#include "graphwalk.h"
#include <cmath>

RandomWalk::RandomWalk(double h, int n, double startY, std::unique_ptr<Distribution> dist)
    : h(h), n(n), startY(startY), dist(std::move(dist))
{}

Trajectory RandomWalk::run(std::mt19937& rng) {
    Trajectory traj;
    traj.x.reserve(n + 1);
    traj.y.reserve(n + 1);

    double x = 0;
    double y = startY;
    traj.x.push_back(x);
    traj.y.push_back(y);

    int crossings = 0;
    bool wasPositive = (y > 0);
    bool wasZero = (y == 0);

    for (int i = 0; i < n; ++i) {
        x += h;
        y += dist->sample(rng);
        traj.x.push_back(x);
        traj.y.push_back(y);

        bool nowPositive = (y > 0);
        bool nowZero = (y == 0);

        if (!wasZero && !nowZero && wasPositive != nowPositive) {
            crossings++;
        } else if (!wasZero && nowZero) {
            crossings++;
        }

        wasPositive = (y > 0);
        wasZero = (y == 0);
    }

    traj.crossings = crossings;
    return traj;
}