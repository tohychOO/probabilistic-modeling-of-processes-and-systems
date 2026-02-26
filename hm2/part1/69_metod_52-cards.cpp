#include <ctime>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));

    int exps = 1000000;
    int a = 0, b = 0, c = 0, d = 0;
    int idx;

    for (int i = 0; i < exps; ++i) {
        vector<int> cards(52);

        for (int i = 2; i <= 14; ++i) {
            for (int j = 0; j < 4; ++j)
                cards[(i-2) * 4 + j] = i;
        }
        
        vector<int> chosen_cards(4);
        for (int i = 0; i < 4; ++i) {
            uniform_int_distribution<int> dist(0, cards.size() - 1);
            idx = dist(gen);
            chosen_cards[i] = cards[idx];
            cards.erase(cards.begin() + idx);
        }

        int card1 = chosen_cards[0];
        int card2 = chosen_cards[1];
        int card3 = chosen_cards[2];
        int card4 = chosen_cards[3];

        if (card1 == card2 && card2 == card3 && card1 != card4 || card1 == card2 && card2 == card4 && card1 != card3 || card1 == card3 && card4 == card3 && card1 != card2 || card4 == card2 && card2 == card3 && card1 != card4)
            ++a;
        else if (card1 == card2 && card3 == card4 && card1 != card3 || card1 == card3 && card2 == card4 && card1 != card2 || card1 == card4 && card3 == card2 && card1 != card3)
            ++b;
        else if (card1 != card2 && card1 != card3 && card1 != card4 && card2 != card3 && card2 != card4 && card3 != card4)
            ++c;
        else if (card1 == card2 && card1 == card3 && card1 == card4)
            ++d;
    }

    double prob1 = static_cast<double>(a) / exps;
    double prob2 = static_cast<double>(b) / exps;
    double prob3 = static_cast<double>(c) / exps;
    double prob4 = static_cast<double>(d) / exps;

    cout << "Вероятность выбора" << endl;
    cout << "1) трех карт одного значения и одной – другого: " << prob1 << endl;
    cout << "2) двух карт одного значения и двух – другого: " << prob2 << endl;
    cout << "3) карт разного значения: " << prob3 << endl;
    cout << "4) карт одного значения: " << prob4 << endl;

    return 0;
}