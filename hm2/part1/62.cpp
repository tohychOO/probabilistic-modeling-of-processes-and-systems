#include <cmath>
#include <ctime>
#include <iostream>
#include <random>

using namespace std;

bool in_triangle(double x, double y, double R) {
    double Ax = 0, Ay = R;
    double Bx = sqrt(3)/2 * R, By = -R/2;
    double Cx = -sqrt(3)/2 * R, Cy = -R/2;

    double v0x = Cx - Ax, v0y = Cy - Ay;
    double v1x = Bx - Ax, v1y = By - Ay;
    double v2x = x - Ax, v2y = y - Ay;

    double dot00 = v0x*v0x + v0y*v0y;
    double dot01 = v0x*v1x + v0y*v1y;
    double dot02 = v0x*v2x + v0y*v2y;
    double dot11 = v1x*v1x + v1y*v1y;
    double dot12 = v1x*v2x + v1y*v2y;

    double invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    double v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    return (u >= 0) && (v >= 0) && (u + v <= 1);
}

int main() {
    double R;
    cout << "Введите радиус R окружности: ";
    cin >> R;
    if (R <= 0) {
        cout << "Радиус - величина положительная!";
        return 1;
    }
    
    mt19937 gen(time(nullptr));
    uniform_real_distribution<double> dist(-R, R);

    int exps = 1000000;
    int a = 0, b = 0;
    double x, y;

    for (int exp = 0; exp < exps; ++exp) {
        int triangle = 0, left_segment = 0, right_segment = 0, down_segment = 0;
        for (int i = 0; i < 4; ++i) {
            x = dist(gen);
            y = dist(gen);
            if (x*x + y*y <= R*R) {
                if (in_triangle(x, y, R))
                    ++triangle;
                else if (y < - R / 2)
                    ++down_segment;
                else if (x < 0)
                    ++left_segment;
                else if (x > 0)
                    ++right_segment;
            } else
                --i;
        }

        if (triangle == 4)
            ++a;
        else if (triangle == 1 && left_segment == 1 && right_segment == 1 && down_segment == 1)
            ++b;
    }

    double prob1 = static_cast<double>(a) / exps;
    double prob2 = static_cast<double>(b) / exps;

    cout << "Вероятность:" << endl;
    cout << "а) что все 4 точки попадут внутрь треугольника - " << prob1 << endl;
    cout << "б) что одна точка попадет внутрь треугольника и по одной точке на каждый сегмент - " << prob2 << endl;

    return 0;
}