#include <ctime>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));
    uniform_real_distribution<double> dist(0, 1);

    int exps = 1000000;
    int suitable_segments = 0;
    double dot;

    double L = 1;

    for (int exp = 0; exp < exps; ++exp) {
        dot = dist(gen);
        double O_B = dot;
        double B_A = 1 - dot;
        double smaller = min(O_B, B_A);
        if (smaller > L / 3)
            ++suitable_segments;
    }

    double prob = static_cast<double>(suitable_segments) / exps;

    cout << "Вероятность, что длина меньшего отрезка больше L/3: " << prob << endl;
}