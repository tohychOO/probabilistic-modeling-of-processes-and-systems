#include <iostream>
#include <random>
#include <ctime>
#include <cmath>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));
    uniform_real_distribution<> dis(0.0, 1.0);
    
    double p, p1;
    int n, trials = 1000000;
    
    cout << "Введите вероятность попадания одной пули p (0-1): ";
    cin >> p;
    cout << "Введите вероятность взрыва при одном попадании p1 (0-1): ";
    cin >> p1;
    cout << "Введите количество выстрелов n: ";
    cin >> n;
    
    if (p < 0 || p > 1 || p1 < 0 || p1 > 1 || n <= 0) {
        cout << "Ошибка ввода данных" << endl;
        return 1;
    }
    
    int explosions = 0;
    
    for (int t = 0; t < trials; t++) {
        int hits = 0;
        
        for (int i = 0; i < n; i++) {
            if (dis(gen) < p)
                hits++;
        }
        
        bool exploded = false;
        
        if (hits >= 2)
            exploded = true;
        else if (hits == 1) {
            if (dis(gen) < p1)
                exploded = true;
        }
        
        if (exploded)
            explosions++;
    }
    
    double prob = (double)explosions / trials;

    cout << "Вероятность взрыва: " << prob << endl;
    
    return 0;
}