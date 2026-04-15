#include <iostream>
#include <random>
#include <ctime>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));
    uniform_real_distribution<> dis(0.0, 1.0);
    
    int trials = 1000000;
    
    int at_least_three = 0;
    for (int t = 0; t < trials; t++) {
        int count = 0;
        for (int i = 0; i < 4; i++) {
            if (dis(gen) < 0.4)
                count++;
        }
        if (count >= 3)
            at_least_three++;
    }
    cout << "а) P(A >= 3 из 4) = " << (double)at_least_three / trials << endl;
    
    int event_B = 0;
    for (int t = 0; t < trials; t++) {
        int count = 0;
        for (int i = 0; i < 5; i++) {
            if (dis(gen) < 0.8)
                count++;
        }
        if (count >= 4)
            event_B++;
    }
    cout << "б) P(B) = " << (double)event_B / trials << endl;
    
    return 0;
}