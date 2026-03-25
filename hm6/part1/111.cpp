#include <iostream>
#include <random>
#include <ctime>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));
    uniform_real_distribution<> dis(0.0, 1.0);

    int trials = 1000000;
    
    int win_one_of_two = 0;
    int win_two_of_four = 0;
    
    for (int t = 0; t < trials; t++) {
        int wins2 = 0;
        for (int i = 0; i < 2; i++) {
            if (dis(gen) < 0.5)
                wins2++;
        }
        if (wins2 == 1)
            win_one_of_two++;
        
        int wins4 = 0;
        for (int i = 0; i < 4; i++) {
            if (dis(gen) < 0.5)
                wins4++;
        }
        if (wins4 == 2)
            win_two_of_four++;
    }
    
    double p1 = (double)win_one_of_two / trials;
    double p2 = (double)win_two_of_four / trials;
    cout << "а) Выиграть 1 из 2: " << p1 << ", выиграть 2 из 4: " << p2 << endl;
    if (p1 > p2)
        cout << "Вероятнее выиграть 1 из 2\n";
    else
        cout << "Вероятнее выиграть 2 из 4\n";
    
    int at_least_two_of_four = 0;
    int at_least_three_of_five = 0;
    
    for (int t = 0; t < trials; t++) {
        int wins4 = 0;
        for (int i = 0; i < 4; i++) {
            if (dis(gen) < 0.5)
                wins4++;
        }
        if (wins4 >= 2)
            at_least_two_of_four++;
        
        int wins5 = 0;
        for (int i = 0; i < 5; i++) {
            if (dis(gen) < 0.5)
                wins5++;
        }
        if (wins5 >= 3)
            at_least_three_of_five++;
    }
    
    p1 = (double)at_least_two_of_four / trials;
    p2 = (double)at_least_three_of_five / trials;
    cout << "б) Не менее 2 из 4: " << p1 << ", не менее 3 из 5: " << p2 << endl;
    if (p1 > p2)
        cout << "Вероятнее не менее 2 из 4\n";
    else
        cout << "Вероятнее не менее 3 из 5\n";
    
    return 0;
}