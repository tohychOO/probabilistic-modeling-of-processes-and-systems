#include <ctime>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));
    uniform_int_distribution<int> dist(1, 6);

    int a = 0, b = 0, c = 0;
    int exps = 1000000;

    for (int i = 0; i < exps; ++i) {
        vector<int> points(3);

        for (int i = 0; i < 3; ++i)
            points[i] = dist(gen);

        if (points[0] == points[1] && points[1] != points[2]) {
            ++b;
            if (points[0] == 1)
                ++a;
        } else if (points[0] == points[2] && points[1] != points[2]) {
            ++b;
            if (points[0] == 1)
                ++a;
        } else if (points[0] != points[1] && points[1] == points[2]) {
            ++b;
            if (points[1] == 1)
                ++a;
        } else if (points[0] != points[1])
            ++c;
    }

    double prob1 = static_cast<double>(a) / exps;
    double prob2 = static_cast<double>(b) / exps;
    double prob3 = static_cast<double>(c) / exps;

    cout << "Вероятность:" << endl;
    cout << "а) что на двух выпавших гранях появится 1 очко, на третьей - другое число очков - " << prob1 << endl;
    cout << "б) что на двух выпавших гранях появится одинаковое число очков, на третьей - другое число очков - " << prob2 << endl;
    cout << "в) что на всех выпавших гранях появится разное число очков - " << prob3 << endl;
    
    return 0;
}