#ifndef POISSON_BASE_H
#define POISSON_BASE_H

#include <vector>
#include <string>

// Параметры задачи согласно вариантам
struct TaskParams {
    double a, b, c, d;      // Границы области
    int n, m;               // Количество разбиений
    double omega;           // Параметр релаксации
    double eps_met;         // Критерий остановки (точность метода)
    int Nmax;               // Максимальное число итераций
};

// Результаты вычислений для справки и графиков
struct SolverResult {
    std::vector<double> values; // Значения функции в узлах (размер (n+1)*(m+1))
    int iterations_done;        // Затрачено итераций N
    double final_residual;      // Достигнутая невязка
    double error;               // Погрешность (eps1 или eps2)
};

#endif // POISSON_BASE_H