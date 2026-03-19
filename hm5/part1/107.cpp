#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    vector<double> snipers = {0.6, 0.5, 0.4}; 

    mt19937 gen(time(nullptr));
    int exps = 1000000;
    int two_hits = 0, third = 0;

    for (int i = 0; i < exps; ++i) {
        int hits = 0;
        bool third_hit = false;
        for (int j = 0; j < 3; ++j) {
            bernoulli_distribution dist(snipers[j]);
            if (dist(gen)) {
                ++hits;
                if (j == 2)
                    third_hit = true;
            }
        }

        if (hits == 2) {
            two_hits++;
            if (third_hit)
                third++;
        }
    }

    double prob = static_cast<double>(third) / two_hits;

    cout << "Вероятность, что 3 стрелок поразил мишень: " << prob << endl;
}