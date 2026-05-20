#include <iostream>
#include <cmath>
#include "solver.h"

double u_exact_func(double x, double y) {
    double s = std::sin(M_PI * x * y);
    return std::exp(s * s);
}

double f_star_func(double x, double y) {
    double h = 1e-4; // Шаг для производной
    // Используем вторую производную: f = - ( (u(x+h) - 2u(x) + u(x-h))/h^2 + (u(y+h) - 2u(y) + u(y-h))/h^2 )
    double d2ux = (u_exact_func(x + h, y) - 2.0 * u_exact_func(x, y) + u_exact_func(x - h, y)) / (h * h);
    double d2uy = (u_exact_func(x, y + h) - 2.0 * u_exact_func(x, y) + u_exact_func(x, y - h)) / (h * h);
    
    return -(d2ux + d2uy); 
}

int main() {
    TaskParams p;
    p.a = 0.0; p.b = 2.0; p.c = 0.0; p.d = 1.0;
    p.n = 500; p.m = 250;
    p.eps_met = 1e-7;
    p.Nmax = 500000;
    p.omega = 1.95; // Хорошее начальное приближение для SOR

    std::cout << "[CHECK] Exact at (0,0): " << u_exact_func(p.a, p.c) << std::endl;
    std::cout << "[CHECK] Exact at (b,d): " << u_exact_func(p.b, p.d) << std::endl;

    PoissonSolver solver(p, f_star_func, u_exact_func);
    
    solver.init_grid();
    solver.solve();
    

    std::cout << "Iterations: " << solver.get_iterations() << std::endl;
    std::cout << "Epsilon 1: " << solver.calculate_max_error() << std::endl;

    return 0;
}