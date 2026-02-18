#include <ctime>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));

    int exps = 1000000;
    int five_excellents = 0;
    int idx;

    for (int exp = 0; exp < exps; ++exp) {
        vector<bool> students;

        for (int i = 0; i < 8; ++i)
            students.push_back(true);

        for (int i = 0; i < 4; ++i)
            students.push_back(false);

        int exellents = 0;
        
        for (int i = 0; i < 9; ++i) {
            uniform_int_distribution<int> dist(0, students.size() - 1);
            idx = dist(gen);
            if (students[idx]) 
                ++exellents;
            students.erase(students.begin() + idx);
        }

        if (exellents == 5)
            five_excellents++;
    }

    double prob = static_cast<double>(five_excellents) / exps;

    cout << "Вероятность нахождения 5 отличников среди отобранных: " << prob << endl;
}