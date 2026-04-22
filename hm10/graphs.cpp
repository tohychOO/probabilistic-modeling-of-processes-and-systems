#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <functional>
#include <set>
#include <map>
#include <cmath>
#include <iomanip>

using namespace std;

mt19937 rng(12345);
uniform_real_distribution<> prob(0.0, 1.0);
uniform_int_distribution<> weight(1, 10);

int n;
double edgeProb = 0.5;

struct Edge {
    int u, v, w;
};

vector<Edge> edges;
vector<vector<pair<int,int>>> adj;

void generateGraph() {
    edges.clear();
    adj.assign(n, {});
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            if (prob(rng) < edgeProb) {
                int w = weight(rng);
                edges.push_back({i, j, w});
                adj[i].push_back({j, w});
                adj[j].push_back({i, w});
            }
        }
    }
}

// a) MST вес (Крускал)
int mstWeight() {
    sort(edges.begin(), edges.end(), [](Edge a, Edge b) { return a.w < b.w; });
    vector<int> parent(n);
    iota(parent.begin(), parent.end(), 0);
    function<int(int)> find = [&](int x) {
        return parent[x] == x ? x : parent[x] = find(parent[x]);
    };
    int total = 0, cnt = 0;
    for (auto& e : edges) {
        int ru = find(e.u), rv = find(e.v);
        if (ru != rv) {
            parent[ru] = rv;
            total += e.w;
            cnt++;
            if (cnt == n-1) break;
        }
    }
    return total;
}

// b) длина самого длинного цикла (по числу ребер)
int maxCycleLength() {
    int best = 0;
    vector<bool> used(n, false);
    vector<int> path;
    function<void(int, int)> dfs = [&](int v, int start) {
        used[v] = true;
        path.push_back(v);
        for (auto& [to, w] : adj[v]) {
            if (to == start && path.size() > 2) {
                best = max(best, (int)path.size());
            }
            if (!used[to]) dfs(to, start);
        }
        used[v] = false;
        path.pop_back();
    };
    for (int i = 0; i < n; i++) {
        dfs(i, i);
    }
    return best;
}

// d) сумма весов самого тяжелого цикла
int maxWeightCycle() {
    int best = 0;
    vector<bool> used(n, false);
    vector<int> path;
    vector<int> pathWeight;
    function<void(int, int, int)> dfs = [&](int v, int start, int curWeight) {
        used[v] = true;
        path.push_back(v);
        for (auto& [to, w] : adj[v]) {
            if (to == start && path.size() > 2) {
                best = max(best, curWeight + w);
            }
            if (!used[to]) dfs(to, start, curWeight + w);
        }
        used[v] = false;
        path.pop_back();
    };
    for (int i = 0; i < n; i++) {
        dfs(i, i, 0);
    }
    return best;
}

// e) количество изолированных вершин
int isolatedVertices() {
    int cnt = 0;
    for (int i = 0; i < n; i++) {
        if (adj[i].empty()) cnt++;
    }
    return cnt;
}

// f) количество остовных деревьев (теорема Кирхгофа)
int determinant(vector<vector<int>> mat) {
    int m = mat.size();
    int det = 1;
    for (int i = 0; i < m; i++) {
        int pivot = -1;
        for (int j = i; j < m; j++) {
            if (mat[j][i] != 0) {
                pivot = j;
                break;
            }
        }
        if (pivot == -1) return 0;
        if (pivot != i) {
            swap(mat[i], mat[pivot]);
            det = -det;
        }
        det *= mat[i][i];
        for (int j = i+1; j < m; j++) {
            int factor = mat[j][i] / mat[i][i];
            for (int k = i; k < m; k++) {
                mat[j][k] -= factor * mat[i][k];
            }
        }
    }
    return det;
}

int spanningTrees() {
    vector<vector<int>> L(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        L[i][i] = adj[i].size();
        for (auto& [j, w] : adj[i]) {
            L[i][j]--;
        }
    }
    
    vector<vector<int>> M(n-1, vector<int>(n-1));
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-1; j++) {
            M[i][j] = L[i][j];
        }
    }
    return determinant(M);
}

// g) количество компонент связности
int components() {
    vector<bool> vis(n, false);
    int comp = 0;
    function<void(int)> dfs = [&](int v) {
        vis[v] = true;
        for (auto& [to, w] : adj[v]) if (!vis[to]) dfs(to);
    };
    for (int i = 0; i < n; i++) {
        if (!vis[i]) {
            dfs(i);
            comp++;
        }
    }
    return comp;
}

// h) количество компонент, являющихся полными подграфами
int completeComponents() {
    vector<bool> vis(n, false);
    int cnt = 0;
    for (int i = 0; i < n; i++) {
        if (!vis[i]) {
            vector<int> comp;
            function<void(int)> dfs = [&](int v) {
                vis[v] = true;
                comp.push_back(v);
                for (auto& [to, w] : adj[v]) if (!vis[to]) dfs(to);
            };
            dfs(i);
            int k = comp.size();
            bool complete = true;
            for (int a : comp) {
                set<int> neigh;
                for (auto& [to, w] : adj[a]) neigh.insert(to);
                for (int b : comp) {
                    if (a != b && neigh.find(b) == neigh.end()) {
                        complete = false;
                        break;
                    }
                }
                if (!complete) break;
            }
            if (complete) cnt++;
        }
    }
    return cnt;
}

int main() {
    cout << "Введите количестов вершин: ";
    cin >> n;

    int experiments = 10000;

    double sumA = 0, sumA2 = 0;
    double sumB = 0, sumB2 = 0;
    double sumD = 0, sumD2 = 0;
    double sumE = 0, sumE2 = 0;
    double sumF = 0, sumF2 = 0;
    double sumG = 0, sumG2 = 0;
    double sumH = 0, sumH2 = 0;

    cout << "Выполняются расчеты..." << endl;

    for (int exp = 0; exp < experiments; exp++) {
        generateGraph();

        int a = mstWeight();
        int b = maxCycleLength();
        int d = maxWeightCycle();
        int e = isolatedVertices();
        int f = spanningTrees();
        int g = components();
        int h = completeComponents();

        sumA += a; sumA2 += a*a;
        sumB += b; sumB2 += b*b;
        sumD += d; sumD2 += d*d;
        sumE += e; sumE2 += e*e;
        sumF += f; sumF2 += f*f;
        sumG += g; sumG2 += g*g;
        sumH += h; sumH2 += h*h;
    }
    
    cout << fixed << setprecision(4);

    cout << "A (сумма весов рёбер минимального остовного дерева):                 E = " 
        << setw(10) << sumA/experiments << " D = " 
        << setw(12) << (sumA2/experiments - (sumA/experiments)*(sumA/experiments)) << endl;

    cout << "B (длина самого большого (по количеству рёбер) цикла):               E = " 
        << setw(10) << sumB/experiments << " D = " 
        << setw(12) << (sumB2/experiments - (sumB/experiments)*(sumB/experiments)) << endl;

    cout << "D (количество рёбер самого большого (по сумме весов) цикла):         E = " 
        << setw(10) << sumD/experiments << " D = " 
        << setw(12) << (sumD2/experiments - (sumD/experiments)*(sumD/experiments)) << endl;

    cout << "E (количество изолированных вершин):                                 E = " 
        << setw(10) << sumE/experiments << " D = " 
        << setw(12) << (sumE2/experiments - (sumE/experiments)*(sumE/experiments)) << endl;

    cout << "F (количество остовных деревьев):                                    E = " 
        << setw(10) << sumF/experiments << " D = " 
        << setw(12) << (sumF2/experiments - (sumF/experiments)*(sumF/experiments)) << endl;

    cout << "G (количество компонентов связности):                                E = " 
        << setw(10) << sumG/experiments << " D = " 
        << setw(12) << (sumG2/experiments - (sumG/experiments)*(sumG/experiments)) << endl;

    cout << "H (количество компонентов связности, являющихся полными подграфами): E = " 
        << setw(10) << sumH/experiments << " D = " 
        << setw(12) << (sumH2/experiments - (sumH/experiments)*(sumH/experiments)) << endl;
        
    return 0;
}