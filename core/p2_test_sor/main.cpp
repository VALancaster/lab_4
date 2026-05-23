#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include "solver.h"

// Функции f* и u* ... (оставь их как есть)
double u_exact_func(double x, double y) {
    return std::exp(std::pow(std::sin(M_PI * x * y), 2));
}

double f_star_func(double x, double y) {
    double pi_xy = M_PI * x * y;
    double sin2 = std::sin(2.0 * pi_xy);
    double cos2 = std::cos(2.0 * pi_xy);
    double term = sin2 * sin2 + 2.0 * cos2;
    double u_val = u_exact_func(x, y);
    return - M_PI * M_PI * (x * x + y * y) * u_val * term;
}

int main() {
    TaskParams cfg;
    cfg.a = 0.0; cfg.b = 2.0; cfg.c = 0.0; cfg.d = 1.0;
    cfg.n = 100; cfg.m = 100;
    cfg.eps_met = 1e-6;
    cfg.Nmax = 10000;
    cfg.omega = 1.5;

    // ПЕРЕИМЕНОВАНО: PoissonSolver -> PoissonSolver_p2
    PoissonSolver_p2 solver(cfg, f_star_func, u_exact_func);
    solver.init_grid();
    solver.solve();

    std::cout << "Done! Error: " << solver.calculate_max_error() << std::endl;
    return 0;
}