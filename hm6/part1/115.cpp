#include <iostream>
#include <random>
#include <ctime>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));
    uniform_real_distribution<> dis(0.0, 1.0);
    
    int trials = 1000000;
    double p_boy = 0.51;
    
    int exactly_two = 0;
    int at_most_two = 0;
    int more_than_two = 0;
    int between_two_and_three = 0;
    
    for (int t = 0; t < trials; t++) {
        int boys = 0;
        for (int i = 0; i < 5; i++) {
            if (dis(gen) < p_boy)
                boys++;
        }
        if (boys == 2)
            exactly_two++;
        if (boys <= 2)
            at_most_two++;
        if (boys > 2)
            more_than_two++;
        if (boys >= 2 && boys <= 3)
            between_two_and_three++;
    }
    
    cout << "а) Ровно два мальчика: " << (double)exactly_two / trials << endl;
    cout << "б) Не более двух мальчиков: " << (double)at_most_two / trials << endl;
    cout << "в) Более двух мальчиков: " << (double)more_than_two / trials << endl;
    cout << "г) От двух до трех мальчиков: " << (double)between_two_and_three / trials << endl;
    
    return 0;
}