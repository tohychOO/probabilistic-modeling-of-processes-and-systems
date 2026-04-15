#include <iostream>
#include <random>
#include <ctime>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));
    uniform_real_distribution<> dis(0.0, 1.0);
    
    double left_len = 2.0 / 3.0;
    
    int trials = 1000000;
    int success = 0;
    
    for (int t = 0; t < trials; t++) {
        int left_count = 0;
        int right_count = 0;
        for (int i = 0; i < 4; i++) {
            double point = dis(gen);
            if (point < left_len)
                left_count++;
            else
                right_count++;
        }
        if (left_count == 2 && right_count == 2)
            success++;
    }
    
    cout << "Вероятность: " << (double)success / trials << endl;
    
    return 0;
}