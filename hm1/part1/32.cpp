#include <ctime>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));

    double R = 10, r = 5;
    int exps = 1000000;
    int dots_in_ring = 0;
    double x, y;

    uniform_real_distribution<double> dist(-R, R);

    for (int exp = 0; exp < exps; ++exp) {
        x = dist(gen);
        y = dist(gen);
        if (x*x + y*y <= R*R) {
            if (x*x + y*y >= r*r)
                ++dots_in_ring;
        } else
            --exp;
    }

    double prob = static_cast<double>(dots_in_ring) / exps;

    cout << "Вероятность попадания точки в кольцо: " << prob << endl;
}