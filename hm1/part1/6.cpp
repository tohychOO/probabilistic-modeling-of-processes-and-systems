#include <ctime>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    vector<int> cubes;

    for (int i = 0; i < 8; ++i)
        cubes.push_back(3);

    for (int i = 0; i < 96; ++i)
        cubes.push_back(2);

    for (int i = 0; i < 384; ++i)
        cubes.push_back(1);

    for (int i = 0; i < 512; ++i)
        cubes.push_back(0);

    mt19937 gen(time(nullptr));
    uniform_int_distribution<int> dist(0, cubes.size() - 1);

    int one = 0, two = 0, three = 0;
    int exps = 1000000;

    for (int i = 0; i < exps; ++i) {
        int idx = dist(gen);
        if (cubes[idx] == 3)
            ++three;
        else if (cubes[idx] == 2)
            ++two;
        else if (cubes[idx] == 1)
            ++one;
    }

    double prob1 = static_cast<double>(one) / exps;
    double prob2 = static_cast<double>(two) / exps;
    double prob3 = static_cast<double>(three) / exps;

    cout << "Вероятность извлечения кубика с:" << endl;
    cout << "а) одной окрашенной гранью - " << prob1 << endl;
    cout << "б) двумя окрашенными гранями - " << prob2 << endl;
    cout << "в) тремя окрашенными гранями - " << prob3 << endl;
}