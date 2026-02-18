#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void print_permutation(const vector<int>& perm) {
    cout << "[";
    for (int i = 0; i < perm.size(); ++i) {
        cout << perm[i];
        if (i < perm.size() - 1) cout << ",";
    }
    cout << "]";
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Необходимо ввести количсетво элементов множества (N >= 2)" << endl;
        return 1;
    }

    int N = stoi(argv[1]);
    if (N < 2) {
        cout << "Слишком мало элементов!" << endl;
        return 1;
    }

    int i = 1, j = 2;

    vector<int> perm(N);
    for (int k = 0; k < N; ++k) 
        perm[k] = k + 1;

    int total = 0;
    int count_i = 0, count_j = 0, count_ij = 0, count_union = 0;
    
    vector<vector<int>> fav_i, fav_j, fav_ij, fav_union;

    do {
        total++;
        
        bool A_i = (perm[i-1] == i);
        bool A_j = (perm[j-1] == j);
        
        if (A_i) {
            count_i++;
            fav_i.push_back(perm);
        }
        if (A_j) {
            count_j++;
            fav_j.push_back(perm);
        }
        if (A_i && A_j) {
            count_ij++;
            fav_ij.push_back(perm);
        }
        if (A_i || A_j) {
            count_union++;
            fav_union.push_back(perm);
        }
        
    } while (next_permutation(perm.begin(), perm.end()));

    double p_i = static_cast<double>(count_i) / total;
    double p_j = static_cast<double>(count_j) / total;
    double p_ij = static_cast<double>(count_ij) / total;
    double p_union = static_cast<double>(count_union) / total;

    double anal_p_i = 1.0 / N;
    double anal_p_j = 1.0 / N;
    double anal_p_ij = 1.0 / (N * (N - 1));
    double anal_p_union = anal_p_i + anal_p_j - anal_p_ij;

    cout << "Аналитические значения:" << endl;
    cout << "P(A" << i << ") = " << anal_p_i << endl;
    cout << "P(A" << j << ") = " << anal_p_j << endl;
    cout << "P(A" << i << "A" << j << ") = " << anal_p_ij << endl;
    cout << "P(A" << i << "+A" << j << ") = " << anal_p_union << endl;

    cout << "\nЭкcпериментальные значения:" << endl;
    cout << "P(A" << i << ") = " << p_i << endl;
    cout << "P(A" << j << ") = " << p_j << endl;
    cout << "P(A" << i << "A" << j << ") = " << p_ij << endl;
    cout << "P(A" << i << "+A" << j << ") = " << p_union << endl;
    
    cout << "\nПроверка теоремы:" << endl;
    cout << "P(Ai)+P(Aj)-P(AiAj) = " << p_i + p_j - p_ij << endl;
    cout << "P(Ai+Aj) = " << p_union << endl;
    cout << "Равенство выполняется: " << (abs(p_i + p_j - p_ij - p_union) < 1e-10 ? "ДА" : "НЕТ") << endl;

    cout << "\nБлагоприятствующие перестановки:" << endl;
    
    cout << "A_" << i << ":" << endl;
    for (const auto& p : fav_i) {
        print_permutation(p);
        cout << " ";
    }
    
    cout << "\n\nA_" << j << ":" << endl;
    for (const auto& p : fav_j) {
        print_permutation(p);
        cout << " ";
    }
    
    cout << "\n\nA_" << i << "A_" << j << ":" << endl;
    for (const auto& p : fav_ij) {
        print_permutation(p);
        cout << " ";
    }
    
    cout << "\n\nA_" << i << " + A_" << j << ":" << endl;
    for (const auto& p : fav_union) {
        print_permutation(p);
        cout << " ";
    }
    cout << endl;

    return 0;
}