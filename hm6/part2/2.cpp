#include <iostream>
#include <random>
#include <ctime>

using namespace std;

int main() {
    mt19937 gen(time(nullptr));
    uniform_real_distribution<> dis(0.0, 1.0);
    
    int n, k, trials = 1000000;
    
    cout << "Введите общее количество монет n: ";
    cin >> n;
    cout << "Введите количество монет с двумя орлами k: ";
    cin >> k;
    
    if (n <= 0 || k < 0 || k > n) {
        cout << "Ошибка ввода данных" << endl;
        return 1;
    }
    
    int success = 0;
    int condition = 0;
    
    for (int t = 0; t < trials; t++) {
        bool double_heads = (dis(gen) < (double)k / n);
        
        bool first_three_heads = true;
        bool fourth_head = false;
        
        for (int i = 0; i < 4; i++) {
            bool head;
            
            if (double_heads)
                head = true;
            else
                head = (dis(gen) < 0.5);
            
            if (i < 3) {
                if (!head)
                    first_three_heads = false;
            } else
                fourth_head = head;
        }
        
        if (first_three_heads) {
            condition++;
            if (fourth_head)
                success++;
        }
    }
    
    double empirical_prob = (double)success / condition;
    
    cout << "Условная вероятность: " << empirical_prob << endl;
    
    return 0;
}