#include <iostream>
#include <random>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Необходимо ввести значение k (k >= 2)" << endl;
        return 1;
    }

    int k = stoi(argv[1]);
    if (k < 2) {
        cout << "Необходимо хотя бы 2 броска!" << endl;
        return 1;
    }

    mt19937 gen(time(nullptr));    
    bernoulli_distribution dist(0.5);

    int exps = 1000000;
    bool coin, prev_coin;
    int a = 0, b = 0;

    for (int i = 0; i < exps; ++i) {
        coin = dist(gen);

        prev_coin = coin;
        coin = dist(gen);

        int tossings = 2;
        
        while (coin != prev_coin) {
            prev_coin = coin;
            coin = dist(gen);
            ++tossings;
        }

        if (tossings <= k)
            ++a;
        if (tossings % 2 == 0)
            ++b;
    }

    double prob1 = static_cast<double>(a) / exps;
    double prob2 = static_cast<double>(b) / exps;

    double anal_prob1 = 1.0 - pow(0.5, k-1);
    double anal_prob2 = 0.5 / (1 - 0.25);

    cout << "Вероятность: " << endl;
    cout << "a) конца опыта до " << k << " бросания монеты" << endl;
    cout << " - экспериментально - " << prob1 << endl;
    cout << " - аналитически - " << anal_prob1 << endl;
    cout << "б) четного числа бросаний монеты для завешения эксперимента - " << endl;
    cout << " - экспериментально - " << prob2 << endl;
    cout << " - аналитически - " << anal_prob2 << endl;

    return 0;
}