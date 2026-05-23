#ifndef P1_SOLVER_H
#define P1_SOLVER_H

#include <vector>
#include <functional>
#include "poisson_base.h" // Подключаем общую структуру TaskParams

// ПЕРЕИМЕНОВАНО: PoissonSolver -> PoissonSolver_p1
class PoissonSolver_p1 {
public:
    // ЗАМЕНЕНО: TaskConfig -> TaskParams
    PoissonSolver_p1(const TaskParams& config,
                  std::function<double(double, double)> f_star,
                  std::function<double(double, double)> u_exact);

    void init_grid();
    void solve();
    double calculate_max_error() const;

    int get_iterations() const { return iterations_done; }
    double get_last_diff() const { return last_diff; }
    
    // ДОБАВЛЕНО: Серверу нужен массив v для графиков
    std::vector<double> get_v() const { return v; }

private:
    // ЗАМЕНЕНО: TaskConfig -> TaskParams
    TaskParams cfg;
    std::vector<double> v; 
    std::vector<double> f; 
    
    std::function<double(double, double)> f_star;
    std::function<double(double, double)> u_exact;

    double hx, hy;
    int iterations_done;
    double last_diff;

    inline int idx(int i, int j) const {
        return i * (cfg.m + 1) + j;
    }
};

#endif // P1_SOLVER_H