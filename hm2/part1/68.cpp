#include <ctime>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));

    int exps = 1000000;
    int a = 0, b = 0;
    int idx1, idx2, idx3;
    int ball1, ball2, ball3;

    for (int i = 0; i < exps; ++i) {
        vector<int> urn(5);

        for (int i = 0; i < 5; ++i)
            urn[i] = i + 1;
        
        uniform_int_distribution<int> dist(0, urn.size() - 1);
        idx1 = dist(gen);
        ball1 = urn[idx1];
        urn.erase(urn.begin() + idx1);

        dist = uniform_int_distribution<int>(0, urn.size() - 1);
        idx2 = dist(gen);
        ball2 = urn[idx2];
        urn.erase(urn.begin() + idx2);

        dist = uniform_int_distribution<int>(0, urn.size() - 1);
        idx3 = dist(gen);
        ball3 = urn[idx3];
        
        if (ball1 == 1 && ball2 == 4 && ball3 == 5) {
            ++a;
            ++b;
        } else if (ball1 * ball2 * ball3 == 20)
            ++b;
    }

    double prob1 = static_cast<double>(a) / exps;
    double prob2 = static_cast<double>(b) / exps;

    cout << "Вероятность: " << endl;
    cout << "а) что последовательно появятся шары с номерами 1, 4, 5 - " << prob1 << endl;
    cout << "б) что появятся шары с номерами 1, 4, 5 - " << prob2 << endl;
    
    return 0;
}