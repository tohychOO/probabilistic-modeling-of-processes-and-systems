#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

bool validPattern(const string& s, int r) {
    for (char c : s) {
        if (c < '0' || c >= '0' + r) 
            return false;
    }
    return true;
}

string generateCluster(int n, const vector<double>& probs, mt19937& gen) {
    string cluster;
    discrete_distribution<int> dist(probs.begin(), probs.end());
    for (int i = 0; i < n; ++i)
        cluster += '0' + dist(gen);
    return cluster;
}

bool areConnectable(const string& left, const string& right, const string& pattern) {
    int n = left.size();
    int k = pattern.size();
    for (int tailLen = 1; tailLen < k; ++tailLen) {
        if (tailLen > n || (k - tailLen) > n) 
            continue;
        string tail = left.substr(n - tailLen, tailLen);
        string head = right.substr(0, k - tailLen);
        if (tail + head == pattern) 
            return true;
    }
    return false;
}

int main() {
    int M, n, r, k, D, experiments;

    cout << "Введите M (число кластеров, M >= 2): ";
    cin >> M;
    if (M < 2) {
        cout << "Ошибка: M должно быть >= 2" << endl;
        return 1;
    }

    cout << "Введите n (длина кластера, n >= 1): ";
    cin >> n;
    if (n < 1) {
        cout << "Ошибка: n должно быть >= 1" << endl;
        return 1;
    }

    cout << "Введите r (размер алфавита, r >= 2): ";
    cin >> r;
    if (r < 2) {
        cout << "Ошибка: r должно быть >= 2" << endl;
        return 1;
    }

    vector<double> probs(r);
    double sum = 0;
    cout << "Введите вероятности символов (" << r << " чисел, сумма = 1):" << endl;
    for (int i = 0; i < r; ++i) {
        cout << "p[" << i << "] = ";
        cin >> probs[i];
        if (probs[i] < 0 || probs[i] > 1) {
            cout << "Ошибка: вероятности должны быть в [0,1]" << endl;
            return 1;
        }
        sum += probs[i];
    }
    if (abs(sum - 1.0) > 1e-9) {
        cout << "Ошибка: сумма вероятностей должна быть 1 (текущая сумма = " << sum << ")" << endl;
        return 1;
    }

    cout << "Введите k (длина паттерна, 2 <= k <= 2*n): ";
    cin >> k;
    if (k < 2 || k > 2 * n) {
        cout << "Ошибка: k должно быть от 2 до 2*n" << endl;
        return 1;
    }

    cout << "Введите паттерн (строка длины " << k << " из символов 0.." << r-1 << "): ";
    string pattern;
    cin >> pattern;
    if (pattern.size() != k) {
        cout << "Ошибка: длина паттерна должна быть " << k << endl;
        return 1;
    }
    if (!validPattern(pattern, r)) {
        cout << "Ошибка: паттерн должен содержать только символы от 0 до " << r-1 << endl;
        return 1;
    }

    cout << "Введите D (для пункта b, 0 <= D <= " << M-1 << "): ";
    cin >> D;
    if (D < 0 || D > M - 1) {
        cout << "Ошибка: D должно быть от 0 до " << M-1 << endl;
        return 1;
    }

    cout << "Введите число экспериментов (>= 1): ";
    cin >> experiments;
    if (experiments < 1) {
        cout << "Ошибка: число экспериментов должно быть >= 1" << endl;
        return 1;
    }

    mt19937 gen(random_device{}());

    int count_all_connectable = 0;
    int count_exact_D = 0;
    int count_all_not_connectable = 0;

    vector<string> example_a, example_b, example_c;
    bool found_a = false, found_b = false, found_c = false;

    for (int exp = 0; exp < experiments; ++exp) {
        vector<string> clusters(M);
        for (int i = 0; i < M; ++i)
            clusters[i] = generateCluster(n, probs, gen);

        vector<bool> connectable(M - 1, false);
        for (int i = 0; i < M - 1; ++i)
            connectable[i] = areConnectable(clusters[i], clusters[i+1], pattern);

        int connectable_count = 0;
        for (bool c : connectable) {
            if (c) 
                connectable_count++;
        }

        if (connectable_count == M - 1) {
            count_all_connectable++;
            if (!found_a) {
                example_a = clusters;
                found_a = true;
            }
        }

        if (connectable_count == D) {
            count_exact_D++;
            if (!found_b) {
                example_b = clusters;
                found_b = true;
            }
        }

        if (connectable_count == 0) {
            count_all_not_connectable++;
            if (!found_c) {
                example_c = clusters;
                found_c = true;
            }
        }
    }

    cout << fixed << setprecision(6);
    cout << "\nВероятность, что:\n";
    cout << "a) все пары соединяемы: "
         << (double)count_all_connectable / experiments << endl;
    if (found_a) {
        cout << "   Пример кластеров:\n";
        for (const auto& cl : example_a) 
            cout << "     " << cl << "\n";
    }

    cout << "b) ровно " << D << " пар соединяемы: "
         << (double)count_exact_D / experiments << endl;
    if (found_b) {
        cout << "   Пример кластеров:\n";
        for (const auto& cl : example_b) 
            cout << "     " << cl << "\n";
    }

    cout << "c) все пары не соединяемы: "
         << (double)count_all_not_connectable / experiments << endl;
    if (found_c) {
        cout << "   Пример кластеров:\n";
        for (const auto& cl : example_c) 
            cout << "     " << cl << "\n";
    }

    return 0;
}