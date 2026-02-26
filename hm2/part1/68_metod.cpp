#include <ctime>
#include <iostream>
#include <random>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));
    bernoulli_distribution dist(0.3);

    int exps = 1000000;
    double prob = 0;
    int n = 1;

    cout << "n   P(успех)  >0.9?" << endl;
    while (prob < 0.9) {
        int success = 0;

        for (int exp = 0; exp < exps; ++exp) {
            bool informed = false;

            bernoulli_distribution dist_chief(0.3);
            if (dist_chief(gen)) {
                informed = true;
            } else {
                bernoulli_distribution dist(0.5);
                for (int message = 0; message < n; ++message) {
                    if (dist(gen)) {
                        informed = true;
                        break;
                    }
                }
            }

            if (informed)
                success++;
        }

        prob = static_cast<double>(success) / exps;
        cout << n << "   " << prob << "    " << (prob > 0.9 ? "ДА" : "НЕТ") << endl;
        ++n;
    }

    return 0;
}