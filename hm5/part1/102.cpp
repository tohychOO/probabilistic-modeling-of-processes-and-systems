#include <iostream>
#include <random>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));
    bernoulli_distribution dist1(0.55);
    int exps = 1000000;
    int standards = 0, second = 0;

    for (int i = 0; i < exps; ++i) {
        if (dist1(gen)) {
            bernoulli_distribution dist2(0.9);
            if (dist2(gen))
                ++standards;
        } else {
            bernoulli_distribution dist2(0.98);
            if (dist2(gen)) {
                ++standards;
                ++second;
            }
        }
    }

    double prob = static_cast<double>(second) / standards;

    cout << "Вероятность, что изделие проверил 2 товаровед: " << prob << endl;
}