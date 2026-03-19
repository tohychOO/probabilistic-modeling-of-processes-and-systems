#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    vector<int> urn1(10);
    vector<int> urn2(11);
    vector<int> urn3(11);

    for (int i = 0; i < 10; ++i) {
        if (i < 6) {
            urn1[i] = 0;
            urn2[i] = 0;
            urn3[i] = 0;
        } else {
            urn1[i] = 1;
            urn2[i] = 1;
            urn3[i] = 1;
        }
    }

    mt19937 gen(time(nullptr));
    int exps = 1000000;
    int whites = 0;

    for (int i = 0; i < exps; ++i) {
        uniform_int_distribution<int> dist1(0, 9);
        if (urn1[dist1(gen)])
            urn2[10] = 1;
        else
            urn2[10] = 0;

        uniform_int_distribution<int> dist2(0, 10);
        if (urn2[dist1(gen)])
            urn3[10] = 1;
        else
            urn3[10] = 0;

        if (urn3[dist1(gen)])
            ++whites;
    }

    double prob = static_cast<double>(whites) / exps;

    cout << "Вероятность извлечения из 3 урны белого шара: " << prob << endl;
}