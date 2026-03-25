#include <iostream>
#include <random>
#include <ctime>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));
    uniform_real_distribution<> dis(0.0, 1.0);
    
    int trials = 1000000;
    int success = 0;
    
    for (int t = 0; t < trials; t++) {
        int count[4] = {0, 0, 0, 0};
        for (int i = 0; i < 8; i++) {
            double point = dis(gen);
            int part = (int)(point * 4);
            if (part == 4)
                part = 3;
            count[part]++;
        }
        if (count[0] == 2 && count[1] == 2 && count[2] == 2 && count[3] == 2)
            success++;
    }
    
    cout << "Вероятность: " << (double)success / trials << endl;
    
    return 0;
}