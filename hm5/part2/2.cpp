#include <iostream>
#include <vector>
#include <random>
#include <unordered_map>

using namespace std;

int main() {
    const int N = 65536;
    const int k = 213;
    
    mt19937 gen(random_device{}());
    uniform_int_distribution<int> dist(0, N-1);
    
    int key = dist(gen);
    
    vector<int> C1(k), C2(k);
    
    for (int i = 0; i < k; ++i) {
        int x1 = dist(gen);
        int x2 = dist(gen);
        C1[i] = x1 ^ key;
        C2[i] = x2 ^ key;
    }
    
    unordered_map<int, vector<int>> c1_indices;
    for (int i = 0; i < k; ++i) {
        c1_indices[C1[i]].push_back(i);
    }
    
    bool found = false;
    cout << "Найденные коллизии:\n";
    
    for (int j = 0; j < k; ++j) {
        if (c1_indices.count(C2[j])) {
            for (int i : c1_indices[C2[j]]) {
                cout << "  C1[" << i << "] = " << C1[i] 
                     << " и C2[" << j << "] = " << C2[j] 
                     << " совпадают" << endl;
                found = true;
            }
        }
    }
    
    if (!found) {
        cout << "Коллизий не найдено.\n";
    }
    
    return 0;
}