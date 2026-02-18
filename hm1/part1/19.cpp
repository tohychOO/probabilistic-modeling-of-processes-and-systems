#include <ctime>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));

    int exps = 1000000;
    int three_Lviv = 0;
    int idx;

    for (int exp = 0; exp < exps; ++exp) {
        vector<bool> kinescopes;

        for (int i = 0; i < 10; ++i)
            kinescopes.push_back(true);

        for (int i = 0; i < 5; ++i)
            kinescopes.push_back(false);

        int Lviv_kinescopes = 0;
        
        for (int i = 0; i < 5; ++i) {
            uniform_int_distribution<int> dist(0, kinescopes.size() - 1);
            idx = dist(gen);
            if (kinescopes[idx]) 
                ++Lviv_kinescopes;
            kinescopes.erase(kinescopes.begin() + idx);
        }

        if (Lviv_kinescopes == 3)
            three_Lviv++;
    }

    double prob = static_cast<double>(three_Lviv) / exps;

    cout << "Вероятность нахождения 3 кинескопов среди выбранных: " << prob << endl;
}