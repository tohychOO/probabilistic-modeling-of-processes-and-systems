#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));
    uniform_real_distribution<double> dist(0, 3);

    int exps = 1000000;
    double dot;
    int successes = 0;

    for (int exp = 0; exp < exps; ++exp) {
        bool first = false, second = false, third = false;
        for (int i = 0; i < 3; ++i) {
            dot = dist(gen);
            if (dot < 1)
                first = true;
            else if (dot < 2)
                second = true;
            else if (dot < 3)
                third = true;
        }

        if (first && second && third)
            ++successes;
    }

    double prob = static_cast<double>(successes) / exps;

    cout << "Вероятность, что на каждую часть отрезка попадет точка: " << prob << endl;

    return 0;
}