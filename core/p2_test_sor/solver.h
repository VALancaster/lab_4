#ifndef P2_SOLVER_H
#define P2_SOLVER_H

#include "poisson_base.h"
#include <functional>
#include <vector>

// ПЕРЕИМЕНОВАНО: PoissonSolver -> PoissonSolver_p2
class PoissonSolver_p2 {
public:
    PoissonSolver_p2(const TaskParams& config,
                  std::function<double(double, double)> f_star,
                  std::function<double(double, double)> u_exact);

    void init_grid();
    void solve();
    double calculate_max_error() const;

    int get_iterations() const { return iterations_done; }
    double get_last_diff() const { return last_diff; }
    
    // ДОБАВЛЕНО ДЛЯ СЕРВЕРА
    std::vector<double> get_v() const { return v; }

private:
    TaskParams cfg;
    std::vector<double> v;
    std::vector<double> f;
    
    std::function<double(double, double)> f_star;
    std::function<double(double, double)> u_exact;

    int iterations_done;
    double last_diff;

    inline int idx(int i, int j) const { return i * (cfg.m + 1) + j; }
};

#endif // P2_SOLVER_H