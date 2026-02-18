#include <ctime>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));

    int exps = 1000000;
    int one_painted = 0;
    int two_painted = 0;
    int idx;

    for (int exp = 0; exp < exps; ++exp) {
        vector<bool> products;

        for (int i = 0; i < 3; ++i)
            products.push_back(true);

        for (int i = 0; i < 2; ++i)
            products.push_back(false);

        int painted = 0;
        
        for (int i = 0; i < 2; ++i) {
            uniform_int_distribution<int> dist(0, products.size() - 1);
            idx = dist(gen);
            if (products[idx]) 
                ++painted;
            products.erase(products.begin() + idx);
        }

        if (painted == 1)
            one_painted++;
        else if (painted == 2)
            two_painted++;
    }

    double prob1 = static_cast<double>(one_painted) / exps;
    double prob2 = static_cast<double>(two_painted) / exps;
    double prob3 = static_cast<double>(one_painted + two_painted) / exps;

    cout << "Вероятность извлечения: " << endl;
    cout << "а) одного окрашенного изделия - " << prob1 << endl;
    cout << "б) двух окрашенных изделий - " << prob2 << endl;
    cout << "в) хотя бы одного окрашенного изделия - " << prob3 << endl;
}