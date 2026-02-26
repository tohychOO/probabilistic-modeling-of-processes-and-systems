#include <ctime>
#include <iomanip>
#include <iostream>
#include <random>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));
    bernoulli_distribution dist(0.8);

    int exps = 1000000;
    double prob = 1;
    int n = 1;

    cout << " n   P(нет промахов) <0.4?" << endl;
    while (prob >= 0.4) {
        int success = 0;

        for (int exp = 0; exp < exps; ++exp) {
            bool no_miss = true;
            for (int shot = 0; shot < n; ++shot) {
                if (!dist(gen)) {
                    no_miss = false;
                    break;
                }
            }
            if (no_miss) 
                success++;
        }

        prob = static_cast<double>(success) / exps;
        cout << setw(2) << n << "   " << prob << "        " << (prob < 0.4 ? "ДА" : "НЕТ") << endl;
        ++n;
    }

    return 0;
}