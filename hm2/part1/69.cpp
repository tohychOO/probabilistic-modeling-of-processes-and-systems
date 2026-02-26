#include <ctime>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));

    int exps = 1000000;
    int successes = 0;
    int idx;

    for (int i = 0; i < exps; ++i) {
        vector<bool> questions(25);
        bool all_known = true;

        for (int i = 0; i < 25; ++i) {
            if (i < 20)
                questions[i] = true;
            else
                questions[i] = false;
        }
        
        for (int i = 0; i < 3; ++i) {
            uniform_int_distribution<int> dist(0, questions.size() - 1);
            idx = dist(gen);
            if (!questions[idx]) {
                all_known = false;
                break;
            }
            questions.erase(questions.begin() + idx);
        }

        if (all_known)
            ++successes;
    }

    double prob = static_cast<double>(successes) / exps;

    cout << "Вероятность знания студентом всех трех вопросов: " << prob << endl;
    
    return 0;
}