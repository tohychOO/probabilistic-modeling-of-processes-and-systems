#include <ctime>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));

    int exps = 1000000;
    int both_unworn = 0;
    int idx;

    for (int exp = 0; exp < exps; ++exp) {
        vector<bool> elems;

        for (int i = 0; i < 2; ++i)
            elems.push_back(true);

        for (int i = 0; i < 3; ++i)
            elems.push_back(false);

        bool is_unworn = true;
        
        for (int i = 0; i < 2; ++i) {
            uniform_int_distribution<int> dist(0, elems.size() - 1);
            idx = dist(gen);
            if (elems[idx]) {
                is_unworn = false;
                break;
            } else
                elems.erase(elems.begin() + idx);
        }

        if (is_unworn)
            both_unworn++;
    }

    double prob = static_cast<double>(both_unworn) / exps;

    cout << "Вероятность включения неизношенных элементов: " << prob << endl;
}