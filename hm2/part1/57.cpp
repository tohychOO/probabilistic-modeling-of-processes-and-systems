#include <ctime>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    vector<double> boxes = {0.6, 0.7, 0.8, 0.9};

    mt19937 gen(time(nullptr));

    int a = 0, b = 0;
    int exps = 1000000;

    for (int i = 0; i < exps; ++i) {
        int details = 0;
        for (double p : boxes) {
            bernoulli_distribution dist(p);
            if (dist(gen))
                ++details;
        }

        if (details <= 3)
            ++a;
        if (details >= 2)
            ++b;
    }

    double prob1 = static_cast<double>(a) / exps;
    double prob2 = static_cast<double>(b) / exps;

    cout << "Вероятность содержания детали:" << endl;
    cout << "а) не более чем в 3 ящиках - " << prob1 << endl;
    cout << "б) не менее чем в 2 ящиках - " << prob2 << endl;
    
    return 0;
}