#include <iostream>
#include <random>
#include <ctime>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));
    uniform_real_distribution<> dis(0.0, 1.0);
    
    double a, x;
    cout << "Введите длину отрезка a: ";
    cin >> a;
    cout << "Введите расстояние x от точки A (0 < x < a): ";
    cin >> x;
    
    if (a <= 0 || x <= 0 || x >= a) {
        cout << "Ошибка: x должен быть в интервале (0, a)" << endl;
        return 1;
    }
    
    double p_left = x / a;
    
    int trials = 1000000;
    int success = 0;
    
    for (int t = 0; t < trials; t++) {
        int left_count = 0;
        int right_count = 0;
        for (int i = 0; i < 5; i++) {
            double point = dis(gen);
            if (point < p_left)
                left_count++;
            else
                right_count++;
        }
        if (left_count == 2 && right_count == 3)
            success++;
    }
    
    cout << "Вероятность (a = " << a << ", x = " << x << "): " << (double)success / trials << endl;
    
    return 0;
}