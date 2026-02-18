#include <ctime>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));

    double a, r;
    cout << "Введите половину расстояния a между прямыми: ";
    cin >> a;
    cout << "Введите радиус r монеты: ";
    cin >> r;
    
    if (a <= 0 || r <= 0) {
        cout << "Расстояние и радиус - положительные величины!";
        return 1;
    }

    uniform_real_distribution<double> dist(0, 2 * a);

    int exps = 1000000;
    int not_intersect = 0;
    double x;

    for (int exp = 0; exp < exps; ++exp) {
        x = dist(gen);
        if (x - r >= 0 && x + r <= 2 * a)
            ++not_intersect;
    }

    double prob = static_cast<double>(not_intersect) / exps;

    cout << "Вероятность, что монета не пересечет ни одну из прямых: " << prob << endl;
}