#include <iostream>
#include <random>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));
    uniform_int_distribution<int> dist1(0, 1);
    int exps = 1000000;
    int mistakes = 0, first = 0;

    for (int i = 0; i < exps; ++i) {
        if (dist1(gen)) {
            bernoulli_distribution dist2(0.1);
            if (dist2(gen))
                ++mistakes;
        } else {
            bernoulli_distribution dist2(0.05);
            if (dist2(gen)) {
                ++mistakes;
                ++first;
            }
        }
    }

    double prob = static_cast<double>(first) / mistakes;

    cout << "Вероятность, что ошиблась 1 перфораторщица: " << prob << endl;
}