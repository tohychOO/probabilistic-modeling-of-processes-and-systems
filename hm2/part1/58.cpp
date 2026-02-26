#include <ctime>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));
    uniform_int_distribution<int> dist(1, 6);

    int a = 0, b = 0;
    int exps = 1000000;

    for (int i = 0; i < exps; ++i) {
        vector<int> points(3);

        for (int i = 0; i < 3; ++i)
            points[i] = dist(gen);

        if (points[0] == points[1] == points[2]) {
            ++b;
            if (points[0] == 5)
                ++a;
        }
    }

    double prob1 = static_cast<double>(a) / exps;
    double prob2 = static_cast<double>(b) / exps;

    cout << "Вероятность:" << endl;
    cout << "а) что на каждой из выпавших граней появится 5 очков - " << prob1 << endl;
    cout << "б) что на всех выпавших гранях появится одинаковое число очков - " << prob2 << endl;
    
    return 0;
}