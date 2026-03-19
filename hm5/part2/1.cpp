#include <iostream>
#include <cmath>
#include <random>
#include <vector>

using namespace std;

int main() {
    const double lambda = 1.94;

    int k, m;
    cout << "Введите k > 0: ";
    cin >> k;

    if (k < 1) {
        cout << "Ошибка: дети должны быть!";
        return 1;
    }

    cout << "Введите m минимальное число детей для условия (m < k): ";
    cin >> m;

    if (m < 1 || m >= k) {
        cout << "Ошибка: m должно быть от 1 до " << k - 1 << endl;
        return 1;
    }

    mt19937 gen(time(nullptr));
    poisson_distribution<int> poisson(lambda);
    bernoulli_distribution boy(0.5);

    int experiments = 1000000;
    int total_all_boys = 0;
    int total_all_boys_and_ge_m = 0;
    int total_ge_m = 0;

    for (int exp = 0; exp < experiments; ++exp) {
        int children = poisson(gen);

        bool all_boys = true;
        for (int i = 0; i < children; ++i) {
            if (!boy(gen)) {
                all_boys = false;
                break;
            }
        }

        if (all_boys) {
            total_all_boys++;
            if (children >= m) 
                total_all_boys_and_ge_m++;
        }

        if (children >= m) 
                total_ge_m++;
    }

    double p_conditional = static_cast<double>(total_all_boys_and_ge_m) / total_all_boys;
    double p_unconditional = static_cast<double>(total_ge_m) / experiments;

    cout << endl;
    cout << "Условная вероятность P(число детей >= " << m << " | все мальчики) = " << p_conditional << endl;
    cout << "Безусловная вероятность P(число детей >= " << m << ") = " << p_unconditional << endl; 

    return 0;
}