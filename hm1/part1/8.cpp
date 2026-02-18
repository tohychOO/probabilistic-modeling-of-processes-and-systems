#include <ctime>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));

    int increasing_orders = 0;
    int exps = 1000000;
    int number, idx;

    for (int i = 0; i < exps; ++i) {
        vector<int> cur_cubes = {1, 2, 3, 4, 5, 6};
        bool is_increase = true;
        
        uniform_int_distribution<int> dist(0, cur_cubes.size() - 1);
        idx = dist(gen);

        number = cur_cubes[idx];
        cur_cubes.erase(cur_cubes.begin() + idx);

        while (!cur_cubes.empty()) {
            uniform_int_distribution<int> dist(0, cur_cubes.size() - 1);
            idx = dist(gen);
            if (number < cur_cubes[idx]) {
                number = cur_cubes[idx];
                cur_cubes.erase(cur_cubes.begin() + idx);
            } else {
                is_increase = false;
                break;
            }
        }

        if (is_increase)
            increasing_orders++;
    }

    double prob = static_cast<double>(increasing_orders) / exps;

    cout << "Вероятность возрастающего порядка номеров извлеченных кубиков: " << prob << endl;
}