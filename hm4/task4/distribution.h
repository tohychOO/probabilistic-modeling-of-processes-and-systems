#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include <random>
#include <vector>
#include <cmath>

class Distribution {
public:
    virtual ~Distribution() = default;
    virtual int sample(std::mt19937& rng) = 0;
    virtual QString name() const = 0;
};

class UniformIntDistribution : public Distribution {
    int a, b;
    std::uniform_int_distribution<int> dist;
public:
    UniformIntDistribution(int a, int b) : a(a), b(b), dist(a, b) {}
    int sample(std::mt19937& rng) override { return dist(rng); }
    QString name() const override { return "Равномерное"; }
};

class BinomialDistribution : public Distribution {
    int trials;
    double p;
    std::binomial_distribution<int> dist;
public:
    BinomialDistribution(int trials, double p) : trials(trials), p(p), dist(trials, p) {}
    int sample(std::mt19937& rng) override { return dist(rng); }
    QString name() const override { return "Биномиальное"; }
};

class GeometricDistribution : public Distribution {
    double p;
    int maxVal;
    std::geometric_distribution<int> dist;
public:
    GeometricDistribution(double p, int maxVal) : p(p), maxVal(maxVal), dist(p) {}
    int sample(std::mt19937& rng) override {
        int val;
        do {
            val = dist(rng) + 1;
        } while (val > maxVal);
        return val;
    }
    QString name() const override { return "Геометрическое"; }
};

class TriangularDistribution : public Distribution {
    int a, b, c;
    std::discrete_distribution<int> dist;
public:
    TriangularDistribution(int a, int b, int c) : a(a), b(b), c(c) {
        std::vector<double> weights(b - a + 1);
        for (int i = 0; i <= b - a; ++i) {
            int val = a + i;
            if (val <= c) {
                weights[i] = val - a + 1;
            } else {
                weights[i] = b - val + 1;
            }
        }
        dist = std::discrete_distribution<int>(weights.begin(), weights.end());
    }
    int sample(std::mt19937& rng) override { return a + dist(rng); }
    QString name() const override { return "Треугольное"; }
};

#endif // DISTRIBUTION_H