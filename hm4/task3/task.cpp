#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <random>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main() {
    std::ifstream f("config.json");
    json config = json::parse(f);
    
    double p = config["p"];
    double sPlus = config["sPlus"];
    double sMinus = config["sMinus"];
    int experiments = config["experiments"];
    int maxN = config["maxN"];
    
    std::mt19937 rng(std::time(nullptr));
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    
    std::map<int, int> hits;
    
    for (int exp = 0; exp < experiments; ++exp) {
        double pos = 0.0;
        for (int step = 1; step <= maxN; ++step) {
            if (dist(rng) < p) {
                pos += sPlus;
            } else {
                pos -= sMinus;
            }
            if (pos == 0.0) {
                hits[step]++;
                break;
            }
        }
    }
    
    std::cout << "Результаты для " << experiments << " экспериментов:\n";
    for (const auto& [n, count] : hits) {
        double prob = static_cast<double>(count) / experiments;
        std::cout << "N=" << n << ": " << prob << " (" << count << ")\n";
    }
    
    json result;
    result["experiments"] = experiments;
    for (const auto& [n, count] : hits) {
        result["hits"][std::to_string(n)] = count;
    }
    
    std::ofstream out("results.json");
    out << result.dump(4);
    
    return 0;
}