#include <ctime>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));
    vector<int> cards36(36);
    vector<int> cards52(52);

    for (int i = 2; i <= 14; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (i >= 6)
                cards36[(i-6) * 4 + j] = i;
            cards52[(i-2) * 4 + j] = i;
        }
    }
    
    for (int a: cards52)
        cout << a << endl;
    
    return 0;
}