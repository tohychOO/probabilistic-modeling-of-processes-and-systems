#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    vector<double> box(50);

    for (int i = 0; i < 50; ++i) {
        if (i < 12 || i > 31)
            box[i] = 0.9;
        else
            box[i] = 0.6;
    }

    mt19937 gen(time(nullptr));
    uniform_int_distribution<int> dist1(0, 49);
    int exps = 1000000;
    int excellents = 0;

    for (int i = 0; i < exps; ++i) {
        bernoulli_distribution dist2(box[dist1(gen)]);
        if (dist2(gen))
            ++excellents;
    }

    double prob = static_cast<double>(excellents) / exps;

    cout << "Вероятность извлечения детали отличного качества: " << prob << endl;
}