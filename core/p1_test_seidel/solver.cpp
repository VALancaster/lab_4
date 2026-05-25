#define _USE_MATH_DEFINES
#include "solver.h"
#include <cmath>
#include <algorithm>
#include <iostream>

// ПЕРЕИМЕНОВАНО: PoissonSolver -> PoissonSolver_p1
// ЗАМЕНЕНО: TaskConfig -> TaskParams
PoissonSolver_p1::PoissonSolver_p1(const TaskParams& config,
                            std::function<double(double, double)> f_star,
                            std::function<double(double, double)> u_exact)
    : cfg(config), f_star(f_star), u_exact(u_exact), iterations_done(0), last_diff(0.0) 
{
    hx = (cfg.b - cfg.a) / cfg.n;
    hy = (cfg.d - cfg.c) / cfg.m;
    
    int total_nodes = (cfg.n + 1) * (cfg.m + 1);
    v.assign(total_nodes, 0.0);
    f.assign(total_nodes, 0.0);

    for (int i = 0; i <= cfg.n; ++i) {
        for (int j = 0; j <= cfg.m; ++j) {
            f[idx(i, j)] = f_star(cfg.a + i * hx, cfg.c + j * hy);
        }
    }
}

// ПЕРЕИМЕНОВАНО: PoissonSolver -> PoissonSolver_p1
void PoissonSolver_p1::init_grid() {
    for (int i = 0; i <= cfg.n; ++i) {
        double x = cfg.a + i * hx;
        v[idx(i, 0)] = u_exact(x, cfg.c);
        v[idx(i, cfg.m)] = u_exact(x, cfg.d);
    }
    for (int j = 0; j <= cfg.m; ++j) {
        double y = cfg.c + j * hy;
        v[idx(0, j)] = u_exact(cfg.a, y);
        v[idx(cfg.n, j)] = u_exact(cfg.b, y);
    }
}

// ПЕРЕИМЕНОВАНО: PoissonSolver -> PoissonSolver_p1
void PoissonSolver_p1::solve() {
    double hx2inv = 1.0 / (hx * hx);
    double hy2inv = 1.0 / (hy * hy);
    double denom = 2.0 * (hx2inv + hy2inv);

    // ЗАМЕНЕНО: max_iter -> Nmax (так называется поле в общей TaskParams)
    for (iterations_done = 0; iterations_done < cfg.Nmax; ++iterations_done) {
        double current_max_diff = 0.0;

        for (int phase = 0; phase < 2; ++phase) {
            for (int i = 1; i < cfg.n; ++i) {
                for (int j = 1; j < cfg.m; ++j) {
                    if ((i + j) % 2 == phase) {
                        int c_idx = idx(i, j);
                        double v_old = v[c_idx];
                        
                        double sum = (v[idx(i - 1, j)] + v[idx(i + 1, j)]) * hx2inv +
                                    (v[idx(i, j - 1)] + v[idx(i, j + 1)]) * hy2inv;
                                    
                        double v_new = (sum + f[c_idx]) / denom;
                        
                        v[c_idx] = v_old + cfg.omega * (v_new - v_old);
                        
                        current_max_diff = std::max(current_max_diff, std::abs(v[c_idx] - v_old));
                    }
                }
            }
        }

        last_diff = current_max_diff;
        // ЗАМЕНЕНО: epsilon -> eps_met (так называется поле в общей TaskParams)
        if (current_max_diff < cfg.eps_met) {
            iterations_done++;
            break;
        }
    }
}

// ПЕРЕИМЕНОВАНО: PoissonSolver -> PoissonSolver_p1
double PoissonSolver_p1::calculate_max_error() const {
    double max_err = 0.0;
    for (int i = 0; i <= cfg.n; ++i) {
        for (int j = 0; j <= cfg.m; ++j) {
            double exact = u_exact(cfg.a + i * hx, cfg.c + j * hy);
            max_err = std::max(max_err, std::abs(v[idx(i, j)] - exact));
        }
    }
    return max_err;
}