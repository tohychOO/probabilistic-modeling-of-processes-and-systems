#include <ctime>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));

    double R, r;
    cout << "Введите радиус R большого круга: ";
    cin >> R;
    cout << "Введите радиус r малого круга r (r <= R): ";
    cin >> r;
    
    if (R < 0 || r < 0) {
        cout << "Раудиус - неотрицательная величина!";
        return 1;
    }

    if (r > R) {
        cout << "Ошибка: малый круг не может быть больше большого!" << endl;
        return 1;
    }

    uniform_real_distribution<double> dist(-R, R);

    int exps = 1000000;
    int dots_in_small = 0;
    double x, y;

    for (int exp = 0; exp < exps; ++exp) {
        x = dist(gen);
        y = dist(gen);
        if (x*x + y*y <= R*R) {
            if (x*x + y*y <= r*r)
                ++dots_in_small;
        } else
            --exp;
    }

    double prob = static_cast<double>(dots_in_small) / exps;

    cout << "Вероятность попадания точки в малый круг: " << prob << endl;
}