#ifndef DISCRETE_RANDOM_VARIABLE_H
#define DISCRETE_RANDOM_VARIABLE_H

#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

class DiscreteRandomVariable {
private:
    vector<pair<double, double>> m_table;
    
    void validateAndSort() {
        if (m_table.empty()) return;
        
        sort(m_table.begin(), m_table.end());
        
        for (size_t i = 1; i < m_table.size(); ++i) {
            if (abs(m_table[i].first - m_table[i-1].first) < 1e-9)
                throw invalid_argument("Values must be distinct");
        }
        
        double sum = 0.0;
        for (const auto& [val, prob] : m_table) {
            if (prob < -1e-9 || prob > 1.0 + 1e-9)
                throw invalid_argument("Probability must be in [0,1]");
            sum += prob;
        }
        if (abs(sum - 1.0) > 1e-8)
            throw invalid_argument("Sum of probabilities must be 1");
    }
    
public:
    DiscreteRandomVariable() = default;
    
    DiscreteRandomVariable(const vector<pair<double, double>>& table) : m_table(table) {
        validateAndSort();
    }

    const vector<pair<double, double>>& getTable() const { 
        return m_table; 
    }
    
    DiscreteRandomVariable operator*(double scalar) const {
        if (m_table.empty()) 
            return *this;
        vector<pair<double, double>> newTable;
        for (const auto& [val, prob] : m_table)
            newTable.emplace_back(val * scalar, prob);
        return DiscreteRandomVariable(newTable);
    }
    
    DiscreteRandomVariable operator+(const DiscreteRandomVariable& other) const {
        if (m_table.empty() || other.m_table.empty())
            throw logic_error("Cannot operate on empty variables");
        
        map<double, double> resultMap;
        for (const auto& [x, px] : m_table) {
            for (const auto& [y, py] : other.m_table) {
                resultMap[x + y] += px * py;
            }
        }
        
        vector<pair<double, double>> result;
        for (const auto& [val, prob] : resultMap)
            result.emplace_back(val, prob);
        
        return DiscreteRandomVariable(result);
    }
    
    DiscreteRandomVariable operator*(const DiscreteRandomVariable& other) const {
        if (m_table.empty() || other.m_table.empty())
            throw logic_error("Cannot operate on empty variables");
        
        map<double, double> resultMap;
        for (const auto& [x, px] : m_table) {
            for (const auto& [y, py] : other.m_table) {
                resultMap[x * y] += px * py;
            }
        }
        
        vector<pair<double, double>> result;
        for (const auto& [val, prob] : resultMap)
            result.emplace_back(val, prob);
        
        return DiscreteRandomVariable(result);
    }
    
    double expectation() const {
        double ex = 0.0;
        for (const auto& [val, prob] : m_table)
            ex += val * prob;
        return ex;
    }
    
    double variance() const {
        double ex = expectation();
        double ex2 = 0.0;
        for (const auto& [val, prob] : m_table)
            ex2 += val * val * prob;
        return ex2 - ex * ex;
    }
    
    double skewness() const {
        double mu = expectation();
        double sigma = sqrt(variance());
        if (sigma < 1e-12) 
            return 0.0;
        
        double mu3 = 0.0;
        for (const auto& [val, prob] : m_table) {
            double diff = val - mu;
            mu3 += diff * diff * diff * prob;
        }
        return mu3 / (sigma * sigma * sigma);
    }
    
    double kurtosis() const {
        double mu = expectation();
        double sigma2 = variance();
        if (sigma2 < 1e-12) 
            return 0.0;
        
        double mu4 = 0.0;
        for (const auto& [val, prob] : m_table) {
            double diff = val - mu;
            mu4 += diff * diff * diff * diff * prob;
        }
        return mu4 / (sigma2 * sigma2) - 3.0;
    }
    
    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open())
            throw runtime_error("Cannot open file: " + filename);
        
        file << m_table.size() << '\n';
        file << fixed << setprecision(15);
        for (const auto& [val, prob] : m_table)
            file << val << ' ' << prob << '\n';
    }
    
    static DiscreteRandomVariable loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open())
            throw runtime_error("Cannot open file: " + filename);
        
        size_t size;
        file >> size;
        vector<pair<double, double>> table;
        for (size_t i = 0; i < size; ++i) {
            double val, prob;
            file >> val >> prob;
            table.emplace_back(val, prob);
        }
        return DiscreteRandomVariable(table);
    }
    
    string distributionTable() const {
        stringstream ss;
        ss << fixed << setprecision(6);
        ss << "┌─────────────┬─────────────┐\n";
        ss << "│     x       │   P(X=x)    │\n";
        ss << "├─────────────┼─────────────┤\n";
        for (const auto& [val, prob] : m_table) {
            ss << "│ " << setw(11) << val << " │ " 
               << setw(11) << prob << " │\n";
        }
        ss << "└─────────────┴─────────────┘\n";
        return ss.str();
    }
    
    bool isEmpty() const { return m_table.empty(); }
};

#endif // DISCRETE_RANDOM_VARIABLE_H