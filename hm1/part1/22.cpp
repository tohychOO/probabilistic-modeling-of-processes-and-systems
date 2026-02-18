#include <ctime>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));
    uniform_int_distribution<int> dist(0, 4);

    int exps = 1000000;
    int password = 0;

    const int a = 1, b = 0, c = 3, d = 4;

    for (int exp = 0; exp < exps; ++exp) {
        int d1 = dist(gen);
        int d2 = dist(gen);
        int d3 = dist(gen);
        int d4 = dist(gen);
        if (d1 == a && d2 == b && d3 == c && d4 == d)
            ++password;
    }

    double prob = static_cast<double>(password) / exps;

    cout << "Вероятность открытия замка: " << prob << endl;
}