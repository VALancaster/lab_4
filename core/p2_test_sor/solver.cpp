#include "solver.h"
#include <cmath>
#include <algorithm>
#include <iostream>

PoissonSolver::PoissonSolver(const TaskParams& config,
                             std::function<double(double, double)> f_star,
                             std::function<double(double, double)> u_exact)
    : cfg(config), f_star(f_star), u_exact(u_exact), iterations_done(0), last_diff(0.0) 
{
    v.assign((cfg.n + 1) * (cfg.m + 1), 0.0);
    f.assign((cfg.n + 1) * (cfg.m + 1), 0.0);

    double hx = (cfg.b - cfg.a) / cfg.n;
    double hy = (cfg.d - cfg.c) / cfg.m;

    std::cout << "[DEBUG] Solver init. Grid: " << cfg.n << "x" << cfg.m << std::endl;
    std::cout << "[DEBUG] hx=" << hx << ", hy=" << hy << std::endl;

    for (int i = 0; i <= cfg.n; ++i) {
        for (int j = 0; j <= cfg.m; ++j) {
            f[idx(i, j)] = f_star(cfg.a + i * hx, cfg.c + j * hy);
        }
    }

    std::cout << "[DEBUG] f[center] = " << f[idx(cfg.n/2, cfg.m/2)] << std::endl;
}

void PoissonSolver::init_grid() {
    // Граничные условия
    for (int i = 0; i <= cfg.n; ++i) {
        double x = cfg.a + i * ((cfg.b - cfg.a) / cfg.n);
        v[idx(i, 0)] = u_exact(x, cfg.c);
        v[idx(i, cfg.m)] = u_exact(x, cfg.d);
    }
    for (int j = 0; j <= cfg.m; ++j) {
        double y = cfg.c + j * ((cfg.d - cfg.c) / cfg.m);
        v[idx(0, j)] = u_exact(cfg.a, y);
        v[idx(cfg.n, j)] = u_exact(cfg.b, y);
    }
}

void PoissonSolver::solve() {
    double hx = (cfg.b - cfg.a) / cfg.n;
    double hy = (cfg.d - cfg.c) / cfg.m;
    double hx2 = hx * hx;
    double hy2 = hy * hy;
    double denom = 2.0 * (1.0 / hx2 + 1.0 / hy2);

    std::cout << "[DEBUG] Denom = " << denom << std::endl;

    for (iterations_done = 0; iterations_done < cfg.Nmax; ++iterations_done) {
        double current_max_diff = 0.0;

        for (int i = 1; i < cfg.n; ++i) {
            for (int j = 1; j < cfg.m; ++j) {
                double v_old = v[idx(i, j)];
                
                double sum = (v[idx(i - 1, j)] + v[idx(i + 1, j)]) / hx2 +
                             (v[idx(i, j - 1)] + v[idx(i, j + 1)]) / hy2;
                             
                double v_new = (sum + f[idx(i, j)]) / denom;
                
                // Формула SOR: v_new_sor = (1-w)*v_old + w*v_new_gs
                v[idx(i, j)] = (1.0 - cfg.omega) * v_old + cfg.omega * v_new;
                
                
                current_max_diff = std::max(current_max_diff, std::abs(v[idx(i, j)] - v_old));
            }
        }
        last_diff = current_max_diff;

        if (iterations_done % 500 == 0) {
            std::cout << "[DEBUG] Iter " << iterations_done << " MaxDiff " << current_max_diff << std::endl;
            if (std::isnan(current_max_diff) || std::isinf(current_max_diff)) {
                std::cerr << "[ERROR] Method diverged! NaN/Inf detected." << std::endl;
                exit(1);
            }
        }
        
        if (current_max_diff < cfg.eps_met) break;
    }
}

double PoissonSolver::calculate_max_error() const {
    double max_err = 0.0;
    double hx = (cfg.b - cfg.a) / cfg.n;
    double hy = (cfg.d - cfg.c) / cfg.m;
    for (int i = 0; i <= cfg.n; ++i) {
        for (int j = 0; j <= cfg.m; ++j) {
            max_err = std::max(max_err, std::abs(v[idx(i, j)] - u_exact(cfg.a + i * hx, cfg.c + j * hy)));
        }
    }
    return max_err;
}