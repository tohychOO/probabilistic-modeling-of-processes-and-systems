#include <iostream>

int main() {
    double Pa_B1 = 0.6, Pa_B2 = 0.3;
    double Pa_B3 = 1 - (Pa_B1 + Pa_B2);

    std::cout << "Условная вероятность гипотезы B3 при наступлении события A: " << Pa_B3 << std::endl;
}