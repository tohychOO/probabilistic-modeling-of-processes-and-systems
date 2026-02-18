#include <ctime>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));

    double a, r;
    cout << "Введите длину a стороны квадрата: ";
    cin >> a;
    cout << "Введите радиус r монеты: ";
    cin >> r;
    
    if (a <= 0 || r <= 0) {
        cout << "Длина и радиус - положительные величины!";
        return 1;
    }

    uniform_real_distribution<double> dist(0, a);

    int exps = 1000000;
    int not_intersect = 0;
    double x, y;

    for (int exp = 0; exp < exps; ++exp) {
        x = dist(gen);
        y = dist(gen);
        if (x - r >= 0 && x + r <= a && y - r >= 0 && y + r <= a)
            ++not_intersect;
    }

    double prob = static_cast<double>(not_intersect) / exps;

    cout << "Вероятность, что монета не пересечет ни одну из сторон квадрата: " << prob << endl;
}