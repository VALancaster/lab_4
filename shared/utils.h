#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <cmath>
#include <algorithm>

namespace Utils {

    // Вычисление нормы max (бесконечная норма)
    inline double calculate_max_norm(const std::vector<double>& v1, const std::vector<double>& v2) {
        double max_diff = 0.0;
        for (size_t i = 0; i < v1.size(); ++i) {
            max_diff = std::max(max_diff, std::abs(v1[i] - v2[i]));
        }
        return max_diff;
    }

    // Вычисление евклидовой нормы (L2)
    inline double calculate_l2_norm(const std::vector<double>& v) {
        double sum = 0.0;
        for (double val : v) {
            sum += val * val;
        }
        return std::sqrt(sum);
    }

    // Функция для линейной интерполяции (использовать для начального приближения)
    // Линейная интерполяция граничных условий по направлению x
    inline double interpolate_linear(double x, double a, double b, double u_a, double u_b) {
        return u_a + (x - a) * (u_b - u_a) / (b - a);
    }
}

#endif // UTILS_H