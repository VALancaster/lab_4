#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <iomanip>
#include "solver.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double u_exact_func(double x, double y) {
    double sin_val = std::sin(M_PI * x * y);
    return std::exp(sin_val * sin_val);
}

double f_star_func(double x, double y) {
    double pi = M_PI;
    double val = pi * pi * (x * x + y * y);
    double s = std::sin(pi * x * y);
    double c = std::cos(pi * x * y);
    double u = std::exp(s * s);
    
    double delta_u = 2 * u * val * (c * c - s * s + 2 * s * s * c * c);
    return -delta_u;
}

int main() {
    // ЗАМЕНЕНО: TaskConfig -> TaskParams
    TaskParams cfg;
    cfg.a = 0.0; cfg.b = 2.0;
    cfg.c = 0.0; cfg.d = 1.0;
    cfg.n = 100; 
    cfg.m = 100;  
    cfg.eps_met = 1e-9;   // ЗАМЕНЕНО: epsilon -> eps_met
    cfg.Nmax = 100000;    // ЗАМЕНЕНО: max_iter -> Nmax
    cfg.omega = 1.0;

    std::cout << "--- Решение тестовой задачи (Sequential Red-Black) ---" << std::endl;
    
    // ПЕРЕИМЕНОВАНО: PoissonSolver -> PoissonSolver_p1
    PoissonSolver_p1 solver(cfg, f_star_func, u_exact_func);
    solver.init_grid();
    solver.solve();

    std::cout << "Итераций: " << solver.get_iterations() << std::endl;
    double eps1 = solver.calculate_max_error();
    std::cout << "Погрешность eps_1: " << std::scientific << eps1 << std::endl;

    return 0;
}