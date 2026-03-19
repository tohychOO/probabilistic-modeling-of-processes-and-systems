#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    vector<int> pyramid = {1, 1, 1, 1, 0, 0, 0, 0, 0, 0};

    mt19937 gen(time(nullptr));
    uniform_int_distribution<int> dist1(0, 9);
    int exps = 1000000;
    int hits = 0, with_optical_sight = 0, without_optical_sight = 0;

    for (int i = 0; i < exps; ++i) {
        if (pyramid[dist1(gen)]) {
            bernoulli_distribution dist2(0.95);
            if (dist2(gen)) {
                ++hits;
                ++with_optical_sight;
            }
        } else {
            bernoulli_distribution dist2(0.8);
            if (dist2(gen)) {
                ++hits;
                ++without_optical_sight;
            }
        }
    }

    double prob_with = static_cast<double>(with_optical_sight) / hits;
    double prob_without = static_cast<double>(without_optical_sight) / hits;

    cout << "Вероятности: " << endl;
    cout << "Cтрелок стрелял из винтовки с прицелом: " << prob_with << endl;
    cout << "Cтрелок стрелял из винтовки без прицела: " << prob_without << endl;
    cout << "Вывод: ";
    if (prob_with > prob_without)
        cout << "вероятнее, что стрелок стрелял из винтовки с прицелом" << endl;
    else
        cout << "вероятнее, что стрелок стрелял из винтовки без прицела" << endl;
}