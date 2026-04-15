#include <iostream>
#include <random>
#include <ctime>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));
    uniform_real_distribution<> dis(0.0, 1.0);
    
    int trials = 1000000;
    int less_than_two = 0;
    int at_least_two = 0;
    
    for (int t = 0; t < trials; t++) {
        int heads = 0;
        for (int i = 0; i < 5; i++) {
            if (dis(gen) < 0.5)
                heads++;
        }
        if (heads < 2)
            less_than_two++;
        if (heads >= 2)
            at_least_two++;
    }
    
    cout << "а) Менее двух раз: " << (double)less_than_two / trials << endl;
    cout << "б) Не менее двух раз: " << (double)at_least_two / trials << endl;
    
    return 0;
}