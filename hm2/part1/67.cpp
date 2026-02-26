#include <ctime>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));

    int exps = 1000000;
    int all_painted = 0;
    int idx;

    for (int exp = 0; exp < exps; ++exp) {
        vector<bool> box(10);

        for (int i = 0; i < 10; ++i) {
            if (i < 6)
                box[i] = true;
            else
                box[i] = false;
        }
        
        for (int i = 0; i < 4; ++i) {
            uniform_int_distribution<int> dist(0, box.size() - 1);
            idx = dist(gen);
            if (!box[idx])
                break;
            box.erase(box.begin() + idx);
            if (i == 3)
                ++all_painted;
        }
    }

    double prob = static_cast<double>(all_painted) / exps;

    cout << "Вероятность всех окрашенных деталей: " << prob << endl;
    
    return 0;
}