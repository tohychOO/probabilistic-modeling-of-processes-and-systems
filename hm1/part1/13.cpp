#include <ctime>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));

    int exps = 100000;
    int found = 0;
    int idx;

    for (int exp = 0; exp < exps; ++exp) {
        vector<bool> photo_cards;

        photo_cards.push_back(true);

        for (int i = 0; i < 99; ++i)
            photo_cards.push_back(false);
        
        for (int i = 0; i < 10; ++i) {
            uniform_int_distribution<int> dist(0, photo_cards.size() - 1);
            idx = dist(gen);
            if (photo_cards[idx]) {
                found++;
                break;
            }
            photo_cards.erase(photo_cards.begin() + idx);
        }
    }

    double prob = static_cast<double>(found) / exps;

    cout << "Вероятность нахождения нужной карточки среди извлеченных: " << prob << endl;
}