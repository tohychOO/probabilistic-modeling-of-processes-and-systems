#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <cmath>
#include <iomanip>
#include <cctype>

using namespace std;

class Recipe {
public:
    vector<int> requirements;
    Recipe(const vector<int>& req) : requirements(req) {}
    int getComponentCount() { return requirements.size(); }
    int getNeed(int compIdx) { return requirements[compIdx]; }
};

class Storage {
public:
    vector<vector<int>> items;
    
    Storage(int compCount) {
        items.resize(compCount, vector<int>(6, 0));
        for (int i = 0; i < compCount; i++)
            items[i][1] = 1000000;
    }

    bool hasEnough(Recipe& recipe) {
        for (int i = 0; i < items.size(); i++) {
            int total = 0;
            for (int q = 1; q <= 5; q++) 
                total += items[i][q];
            if (total < recipe.getNeed(i)) 
                return false;
        }
        return true;
    }

    void takeComponents(Recipe& recipe) {
        for (int i = 0; i < items.size(); i++) {
            int need = recipe.getNeed(i);
            for (int q = 1; q <= 5 && need > 0; q++) {
                int take = min(need, items[i][q]);
                items[i][q] -= take;
                need -= take;
            }
        }
    }

    void addComponent(int compIdx, int quality, int count) {
        if (compIdx >= 0 && compIdx < items.size() && quality >= 1 && quality <= 5) {
            items[compIdx][quality] += count;
        }
    }

    void addFromRecycling(Recipe& recipe, int quality) {
        for (int i = 0; i < items.size(); i++)
            items[i][max(1, quality - 1)] += recipe.getNeed(i) * 0.25;
    }
};

class Machine {
protected:
    double baseTime;
    double timeLeft;
    int modules;
    mt19937& rng;

public:
    Machine(double time, int mods, mt19937& gen) 
        : baseTime(time), modules(mods), rng(gen), timeLeft(0) {}

    double getCycleTime() { return baseTime * (1 + 0.1 * modules); }
    bool isFree() { return timeLeft <= 0; }
    void tick(double dt) { if (timeLeft > 0) timeLeft -= dt; }
    void start() { timeLeft = getCycleTime(); }
};

class Assembler : public Machine {
    vector<vector<double>> prob;

public:
    Assembler(double time, int mods, mt19937& gen) : Machine(time, mods, gen) {
        prob.resize(6, vector<double>(6, 0.0));
        for (int L = 1; L <= 5; L++) {
            double sum = 0;
            for (int k = 1; k <= 5 - L; k++) {
                prob[L][L + k] = 52 * pow(10, -2 - k);
                sum += prob[L][L + k];
            }
            prob[L][L] = 1 - sum;
        }
    }

    int getProductQuality(int inputQuality) {
        double r = uniform_real_distribution<double>(0, 1)(rng);
        double cum = 0;
        for (int q = 1; q <= 5; q++) {
            cum += prob[inputQuality][q];
            if (r < cum) return q;
        }
        return 1;
    }
};

class Reprocessor : public Machine {
public:
    Reprocessor(double time, int mods, mt19937& gen) : Machine(time, mods, gen) {}
};

class Factory {
    vector<Assembler> assemblers;
    vector<Reprocessor> reprocessors;
    Storage storage;
    Recipe recipe;
    vector<vector<double>> products;
    double currentTime;
    mt19937 rng;

public:
    Factory(int A, int R, Recipe& rec, double tAss, double tRep, vector<double>& speeds)
        : storage(rec.getComponentCount()), recipe(rec), currentTime(0), rng(time(nullptr)) {
        
        for (int i = 0; i < A; i++) 
            assemblers.emplace_back(tAss, 0, rng);
        for (int i = 0; i < R; i++) 
            reprocessors.emplace_back(tRep, 0, rng);
        products.resize(6);
    }

    void step(double dt, vector<double>& speeds) {
        for (int i = 0; i < speeds.size(); i++)
            storage.addComponent(i, 1, speeds[i]);

        for (int j = 0; j < assemblers.size(); j++) {
            if (!assemblers[j].isFree()) {
                assemblers[j].tick(dt);
                continue;
            }

            if (!storage.hasEnough(recipe)) continue;

            int minQuality = 5;
            for (int i = 0; i < recipe.getComponentCount(); i++) {
                for (int q = 1; q <= 5; q++) {
                    if (storage.items[i][q] > 0) {
                        if (q < minQuality) minQuality = q;
                        break;
                    }
                }
            }

            storage.takeComponents(recipe);
            int quality = assemblers[j].getProductQuality(minQuality);
            products[quality].push_back(currentTime);
            assemblers[j].start();
        }

        for (int j = 0; j < reprocessors.size(); j++) {
            if (!reprocessors[j].isFree()) {
                reprocessors[j].tick(dt);
                continue;
            }

            for (int q = 5; q >= 2; q--) {
                if (!products[q].empty()) {
                    products[q].pop_back();
                    storage.addFromRecycling(recipe, max(1, q - 1));
                    break;
                }
            }
            reprocessors[j].start();
        }

        currentTime += dt;
    }

    bool hasEnoughLegendary(int target) {
        return products[5].size() >= target;
    }

    void printResults() {
        for (int q = 3; q <= 5; q++)
            cout << "\nКачество " << q << " (" << products[q].size() << " шт.):\n";
        cout << endl;
    }

    int getLegendaryCount() {
        return products[5].size();
    }
};

bool isNumber(const string& s) {
    if (s.empty()) return false;
    int dots = 0;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '.') {
            dots++;
            if (dots > 1) return false;
        } else if (!isdigit(s[i]) && s[i] != '-')
            return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 6) {
        cout << "ОШИБКА: недостаточно аргументов\n";
        cout << "Использование: " << argv[0] << " A_M R_M t_sb t_pr s1 s2 ...\n";
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (!isNumber(arg)) {
            cout << "ОШИБКА: аргумент '" << arg << "' не является числом\n";
            return 1;
        }
    }

    int A_M = stoi(argv[1]);
    int R_M = stoi(argv[2]);
    double t_sb = stod(argv[3]);
    double t_pr = stod(argv[4]);

    if (A_M < 0) {
        cout << "ОШИБКА: число сборщиков не может быть отрицательным\n";
        return 1;
    }
    if (A_M > 100) {
        cout << "ОШИБКА: слишком много сборщиков (макс 100)\n";
        return 1;
    }

    if (R_M < 0) {
        cout << "ОШИБКА: число переработчиков не может быть отрицательным\n";
        return 1;
    }
    if (R_M > 100) {
        cout << "ОШИБКА: слишком много переработчиков (макс 100)\n";
        return 1;
    }

    if (t_sb <= 0) {
        cout << "ОШИБКА: время сборки должно быть положительным\n";
        return 1;
    }
    if (t_pr <= 0) {
        cout << "ОШИБКА: время переработки должно быть положительным\n";
        return 1;
    }

    vector<double> speeds;
    for (int i = 5; i < argc; i++) {
        double s = stod(argv[i]);
        if (s < 0) {
            cout << "ОШИБКА: скорость поступления не может быть отрицательной\n";
            return 1;
        }
        speeds.push_back(s);
    }

    if (speeds.empty()) {
        cout << "ОШИБКА: должно быть указано хотя бы одно значение скорости\n";
        return 1;
    }

    int n = speeds.size();

    vector<int> req;
    for (int i = 0; i < n; i++) 
        req.push_back(2);
    Recipe recipe(req);

    if (recipe.getComponentCount() == 0) {
        cout << "ОШИБКА: рецепт пуст\n";
        return 1;
    }

    cout << "Сборщиков: " << A_M << ", переработчиков: " << R_M << "\n";
    cout << "Время сборки: " << t_sb << ", время переработки: " << t_pr << "\n";
    cout << "Компонентов: " << n << "\n";

    Factory factory(A_M, R_M, recipe, t_sb, t_pr, speeds);

    int maxSteps = 1000000;
    int step = 0;

    while (!factory.hasEnoughLegendary(25) && step < maxSteps) {
        factory.step(1.0, speeds);
        step++;
    }

    factory.printResults();

    return 0;
}