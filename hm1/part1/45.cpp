#include <ctime>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));

    int exps = 1000000;
    int correct_conditions = 0;
    double x, y;

    uniform_real_distribution<double> dist(0, 1);

    for (int exp = 0; exp < exps; ++exp) {
        do {
            x = dist(gen);
        } while (x == 0);

        do {
            y = dist(gen);
        } while (y == 0);

        if (x + y <= 1 && x * y >= 0.09)
            ++correct_conditions;
    }

    double prob = static_cast<double>(correct_conditions) / exps;

    cout << "Вероятность выполенных условий: " << prob << endl;
}