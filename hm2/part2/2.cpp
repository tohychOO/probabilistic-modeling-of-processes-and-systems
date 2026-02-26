#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Необходимо ввести количество экпериментов" << endl;
        return 1;
    }

    int K = stoi(argv[1]);

    if (K < 1) {
        cout << "Число экспериментов должно быть положительно!" << endl;
        return 1;
    }

    mt19937 gen(time(nullptr));

    int n;
    cout << "Введите количестов n ключей: ";
    cin >> n;

    if (n < 1) {
        cout << "Число ключей должно быть положительно!" << endl;
        return 1;
    }

    vector<int> counts(n, 0);

    for (int exp = 0; exp < K; ++exp) {
        vector<int> keys(n);
        for (int i = 0; i < n; ++i) keys[i] = i;

        int attempt = 0;
        while (true) {
            uniform_int_distribution<int> dist(0, keys.size() - 1);
            int idx = dist(gen);
            attempt++;
            if (keys[idx] == 0)
                break;
            keys.erase(keys.begin() + idx);
        }

        counts[attempt - 1]++;

        cout << "Эксперимент " << exp + 1 << ": ключ найден после " << attempt << " попытки" << endl;
    }

    cout << "\nВероятности нахождения ключа" << endl;
    for (int i = 0; i < n; ++i) {
        double p = static_cast<double>(counts[i]) / K;
        cout << "- после " << i + 1 << " попытки: " << p << endl;
    }

    return 0;
}