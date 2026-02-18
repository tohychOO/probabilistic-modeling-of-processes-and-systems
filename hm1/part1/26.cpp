#include <ctime>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));
    uniform_real_distribution<double> dist(0, 20);

    int exps = 1000000;
    int dots_in_small = 0;
    double dot;

    int L = 20, l = 10;
    int start_l = 0;
    int end_l = start_l + l;

    for (int exp = 0; exp < exps; ++exp) {
        dot = dist(gen);
        if (dot >= start_l && dot <= end_l)
            ++dots_in_small;
    }

    double prob = static_cast<double>(dots_in_small) / exps;

    cout << "Вероятность попадания точки на меньший отрезок: " << prob << endl;
}