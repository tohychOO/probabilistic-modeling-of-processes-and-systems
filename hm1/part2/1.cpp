#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    int N, C;
    
    if (argc == 3) {
        N = stoi(argv[1]);
        C = stoi(argv[2]);
    } else {
        cout << "Необходимо ввести мощность алфавита (N >= 2) и количество экспериментов" << endl;
        return 1;
    }

    if (N < 2) {
        cout << "Мощность слишком маленькая!";
        return 1;
    }
    if (C < 1) {
        cout << "Должен быть хотя бы 1 эксперимент!";
        return 1;
    }

    mt19937 gen(time(nullptr));
    
    int first_even = 0;
    int second_even_not_changed = 0;
    int second_even_changed = 0;
    int both_even_not_changed = 0;
    int both_even_changed = 0;
    int idx;

    for (int exp = 0; exp < C; ++exp) {
        vector<int> alphabet;

        for (int i = 1; i <= N; ++i)
            alphabet.push_back(i);
        
        uniform_int_distribution<int> dist(0, alphabet.size() - 1);
        idx = dist(gen);

        bool is_both_even_for_not_changed = true;
        bool is_both_even_for_changed = true;

        if ((idx + 1) % 2 == 0) 
            ++first_even;
        else {
            is_both_even_for_not_changed = false;
            is_both_even_for_changed = false;
        }

        alphabet.erase(alphabet.begin() + idx);

        dist = uniform_int_distribution<int>(0, alphabet.size() - 1);
        idx = dist(gen);

        if (alphabet[idx] % 2 == 0)
            ++second_even_not_changed;
        else
            is_both_even_for_not_changed = false;
        if ((idx + 1) % 2 == 0)
            ++second_even_changed;
        else  
            is_both_even_for_changed = false;

        if (is_both_even_for_not_changed)
            both_even_not_changed++;
        if (is_both_even_for_changed)
            both_even_changed++;
    }

    double prob1 = static_cast<double>(first_even) / C;
    double prob2_1 = static_cast<double>(second_even_not_changed) / C;
    double prob2_2 = static_cast<double>(second_even_changed) / C;
    double prob3_1 = static_cast<double>(both_even_not_changed) / C;
    double prob3_2 = static_cast<double>(both_even_changed) / C;

    int even_count = N / 2;
    int even_count_after_first = (N - 1) / 2;
    double anal_first_even = static_cast<double>(even_count) / N;
    double anal_second_even_not_changed = static_cast<double>(even_count) / N;
    double anal_second_even_changed = static_cast<double>(even_count_after_first) / (N - 1);
    double anal_both_even_not_changed = static_cast<double>(even_count) / N * (static_cast<double>(even_count - 1) / (N - 1));
    double anal_both_even_changed = anal_first_even * anal_second_even_changed;

    cout << "Вероятность того, что выбран символ с четным номером:" << endl;

    cout << "а) в первый раз:" << endl;
    cout << " Индексы оставшихся элементов:" << endl; 
    cout << " a) не изменяются" << endl;
    cout << "  Эксперимент - " << prob1 << endl;
    cout << "  Аналитика - " << anal_first_even << endl;
    cout << " б) изменяются" << endl;
    cout << "  Эксперимент - " << prob1 << endl;
    cout << "  Аналитика - " << anal_first_even << endl;

    cout << "б) во второй раз:" << endl;
    cout << " Индексы оставшихся элементов:" << endl; 
    cout << " a) не изменяются" << endl;
    cout << "  Эксперимент - " << prob2_1 << endl;
    cout << "  Аналитика - " << anal_second_even_not_changed << endl;
    cout << " б) изменяются" << endl;
    cout << "  Эксперимент - " << prob2_2 << endl;
    cout << "  Аналитика - " << anal_second_even_changed << endl;

    cout << "в) оба раза:" << endl;
    cout << " Индексы оставшихся элементов:" << endl; 
    cout << " a) не изменяются" << endl;
    cout << "  Эксперимент - " << prob3_1 << endl;
    cout << "  Аналитика - " << anal_both_even_not_changed << endl;
    cout << " б) изменяются" << endl;
    cout << "  Эксперимент - " << prob3_2 << endl;
    cout << "  Аналитика - " << anal_both_even_changed << endl;

    return 0;
}