#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    vector<double> pyramid = {0.95, 0.95, 0.95, 0.7, 0.7};

    mt19937 gen(time(nullptr));
    uniform_int_distribution<int> dist1(0, 4);
    int exps = 1000000;
    int hits = 0;

    for (int i = 0; i < exps; ++i) {
        bernoulli_distribution dist2(pyramid[dist1(gen)]);
        if (dist2(gen))
            ++hits;
    }

    double prob = static_cast<double>(hits) / exps;

    cout << "Вероятность поражения мишени: " << prob << endl;
}