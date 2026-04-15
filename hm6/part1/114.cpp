#include <iostream>
#include <random>
#include <ctime>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));
    uniform_real_distribution<> dis(0.0, 1.0);
    
    int trials = 1000000;
    double p_fail = 0.1;
    
    int working_a = 0;
    int working_b = 0;
    int working_c = 0;
    
    for (int t = 0; t < trials; t++) {
        int failed_a = 0;
        for (int i = 0; i < 3; i++) {
            if (dis(gen) < p_fail)
                failed_a++;
        }
        if (failed_a == 0)
            working_a++;
        
        int failed_b = 0;
        for (int i = 0; i < 4; i++) {
            if (dis(gen) < p_fail)
                failed_b++;
        }
        if (failed_b <= 1)
            working_b++;
        
        int failed_c = 0;
        for (int i = 0; i < 5; i++) {
            if (dis(gen) < p_fail)
                failed_c++;
        }
        if (failed_c <= 2)
            working_c++;
    }
    
    cout << "а) Без резерва: " << (double)working_a / trials << endl;
    cout << "б) Один резерв: " << (double)working_b / trials << endl;
    cout << "в) Два резерва: " << (double)working_c / trials << endl;
    
    return 0;
}