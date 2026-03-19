#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include <random>
#include <vector>
#include <QString>
#include <QJsonObject>

using namespace std;

class Distribution {
public:
    virtual ~Distribution() = default;
    virtual double sample(mt19937& rng) = 0;
    virtual QString name() const = 0;
    virtual QJsonObject toJson() const = 0;
};

class UniformDistribution : public Distribution {
    double a, b;
    uniform_real_distribution<double> dist;
public:
    UniformDistribution(double a, double b) : a(a), b(b), dist(a, b) {}
    
    double sample(mt19937& rng) override { return dist(rng); }
    
    QString name() const override { return "Равномерное"; }
    
    QJsonObject toJson() const override {
        QJsonObject obj;
        obj["type"] = "uniform";
        obj["a"] = a;
        obj["b"] = b;
        return obj;
    }
};

class BinomialDistribution : public Distribution {
    double p;
    binomial_distribution<int> dist;
public:
    BinomialDistribution(double p) : p(p), dist(1, p) {}
    
    double sample(mt19937& rng) override { return dist(rng); }
    
    QString name() const override { return "Биномиальное (0 или 1)"; }
    
    QJsonObject toJson() const override {
        QJsonObject obj;
        obj["type"] = "binomial";
        obj["p"] = p;
        return obj;
    }
};

class GeometricDistribution : public Distribution {
    int m;
    double prob;
    discrete_distribution<int> dist;
public:
    GeometricDistribution(int m, double prob) : m(m), prob(prob) {
        vector<double> weights(m);
        double sum = 0;
        for (int i = 0; i < m; ++i) {
            weights[i] = prob * pow(1 - prob, i);
            sum += weights[i];
        }
        for (double& w : weights) w /= sum;
        dist = discrete_distribution<int>(weights.begin(), weights.end());
    }
    
    double sample(mt19937& rng) override { return dist(rng) + 1; }
    
    QString name() const override { return "Геометрическое (конечное)"; }
    
    QJsonObject toJson() const override {
        QJsonObject obj;
        obj["type"] = "geometric";
        obj["m"] = m;
        obj["prob"] = prob;
        return obj;
    }
};

class TriangularDistribution : public Distribution {
    int a, b, mode;
    discrete_distribution<int> dist;
public:
    TriangularDistribution(int a, int b, int mode) : a(a), b(b), mode(mode) {
        vector<double> weights(b - a + 1);
        for (int i = 0; i <= b - a; ++i) {
            int val = a + i;
            if (val <= mode) {
                weights[i] = val - a + 1;
            } else {
                weights[i] = b - val + 1;
            }
        }
        dist = discrete_distribution<int>(weights.begin(), weights.end());
    }
    
    double sample(mt19937& rng) override { return a + dist(rng); }
    
    QString name() const override { return "Треугольное"; }
    
    QJsonObject toJson() const override {
        QJsonObject obj;
        obj["type"] = "triangular";
        obj["a"] = a;
        obj["b"] = b;
        obj["mode"] = mode;
        return obj;
    }
};

#endif // DISTRIBUTION_H