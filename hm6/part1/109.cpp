#include <iostream>
#include <random>
#include <ctime>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));
    uniform_real_distribution<> dis(0.0, 1.0);

    double p[4] = {0.1, 0.2, 0.3, 0.4};
    int trials = 1000000;
    int count_first_two_failed = 0;
    int count_two_failed_total = 0;

    for (int t = 0; t < trials; t++) {
        bool failed[4];
        int failed_count = 0;
        for (int i = 0; i < 4; i++) {
            failed[i] = (dis(gen) < p[i]);
            if (failed[i])
                failed_count++;
        }

        if (failed_count == 2) {
            count_two_failed_total++;
            if (failed[0] && failed[1] && !failed[2] && !failed[3])
                count_first_two_failed++;
        }
    }

    double probability = (double)count_first_two_failed / count_two_failed_total;
    cout << "Вероятность того, что отказали первая и вторая: " << probability << endl;

    return 0;
}