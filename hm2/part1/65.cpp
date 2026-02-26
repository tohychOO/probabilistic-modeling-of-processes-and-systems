#include <ctime>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));

    int exps = 1000000;
    int both_winning = 0;
    int idx;

    for (int i = 0; i < exps; ++i) {
        vector<bool> tickets(100);

        for (int i = 0; i < 100; ++i) {
            if (i < 5)
                tickets[i] = true;
            else
                tickets[i] = false;
        }
        
        uniform_int_distribution<int> dist(0, tickets.size() - 1);
        idx = dist(gen);
        if (!tickets[idx])
            continue;
        tickets.erase(tickets.begin() + idx);

        dist = uniform_int_distribution<int>(0, tickets.size() - 1);
        idx = dist(gen);
        if (tickets[idx])
            ++both_winning;
    }

    double prob = static_cast<double>(both_winning) / exps;

    cout << "Вероятность 2 выигрышных билетов: " << prob << endl;
    
    return 0;
}