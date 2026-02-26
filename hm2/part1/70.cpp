#include <ctime>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));

    int exps = 1000000;
    int a = 0, b = 0;
    int idx, idx1, idx2, idx3;
    int cube1, cube2, cube3;

    for (int i = 0; i < exps; ++i) {
        vector<int> cubes(10);

        for (int i = 0; i < 10; ++i)
            cubes[i] = i + 1;
        
        uniform_int_distribution<int> dist(0, cubes.size() - 1);

        for (int i = 0; i < 3; ++i) {
            idx = dist(gen);
            if (cubes[idx] != i + 1)
                break;
            if (i == 2)
                ++b;
        }

        idx1 = dist(gen);
        cube1 = cubes[idx1];
        cubes.erase(cubes.begin() + idx1);

        dist = uniform_int_distribution<int>(0, cubes.size() - 1);
        idx2 = dist(gen);
        cube2 = cubes[idx2];
        cubes.erase(cubes.begin() + idx2);

        dist = uniform_int_distribution<int>(0, cubes.size() - 1);
        idx3 = dist(gen);
        cube3 = cubes[idx3];
        
        if (cube1 == 1 && cube2 == 2 && cube3 == 3)
            ++a;
    }

    double prob1 = static_cast<double>(a) / exps;
    double prob2 = static_cast<double>(b) / exps;

    cout << "Вероятность, что последовательно появятся шары с номерами 1, 2, 3:" << endl;
    cout << "а) без возвращения кубиков - " << prob1 << endl;
    cout << "б) с возвращением кубиков - " << prob2 << endl;
    
    return 0;
}