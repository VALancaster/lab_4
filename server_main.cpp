#define _USE_MATH_DEFINES
#include "httplib.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <cmath>

#include "shared/poisson_base.h"

// Подключаем файлы участников
#include "core/p1_test_seidel/solver.h"
#include "core/p2_test_sor/solver.h"
#include "core/p3_main_seidel/solver.h"
#include "core/p4_main_sor/solver.h"
#include "core/p5_main_sor/solver.h"

using namespace httplib;
namespace fs = std::filesystem;

// --- Функции для тестовой задачи (Вариант 7) ---
double u_exact_func(double x, double y) {
    double sin_val = std::sin(M_PI * x * y);
    return std::exp(sin_val * sin_val);
}

double f_star_func(double x, double y) {
    double pi_xy = M_PI * x * y;
    double sin2 = std::sin(2.0 * pi_xy);
    double cos2 = std::cos(2.0 * pi_xy);
    double term = sin2 * sin2 + 2.0 * cos2;
    double u_val = u_exact_func(x, y);
    return - M_PI * M_PI * (x * x + y * y) * u_val * term;
}

// Упаковка в JSON
std::string to_json(int iters, double resid, double err, int n, int m, 
                    const std::vector<double>& v, const std::vector<double>& diff) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(8);
    ss << "{";
    ss << "\"iterations\": " << iters << ",";
    ss << "\"residual\": " << resid << ",";
    ss << "\"error\": " << err << ",";
    ss << "\"n\": " << n << ",";
    ss << "\"m\": " << m << ",";
    
    ss << "\"values\": [";
    for (size_t i = 0; i < v.size(); ++i) { ss << v[i] << (i == v.size() - 1 ? "" : ","); }
    ss << "],";
    
    ss << "\"diff\": [";
    for (size_t i = 0; i < diff.size(); ++i) { ss << diff[i] << (i == diff.size() - 1 ? "" : ","); }
    ss << "]}";
    return ss.str();
}

int main() {
    Server svr;

    svr.Get("/", [](const Request&, Response& res) {
        std::string path = "../frontend/index.html";
        if (!fs::exists(path)) path = "frontend/index.html";
        
        std::ifstream ifs(path);
        if (ifs) {
            std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
            res.set_content(content, "text/html; charset=utf-8");
        } else {
            res.status = 404; res.set_content("Error: index.html not found.", "text/plain");
        }
    });

    svr.Get("/solve", [](const Request& req, Response& res) {
        int id = req.has_param("id") ? std::stoi(req.get_param_value("id")) : 3;
        int n = req.has_param("n") ? std::stoi(req.get_param_value("n")) : 40;
        int m = req.has_param("m") ? std::stoi(req.get_param_value("m")) : 40;
        double eps = req.has_param("eps") ? std::stod(req.get_param_value("eps")) : 1e-6;
        double omega = req.has_param("omega") ? std::stod(req.get_param_value("omega")) : 1.0;

        TaskParams p; p.a = 0.0; p.b = 2.0; p.c = 0.0; p.d = 1.0;
        p.n = n; p.m = m; p.eps_met = eps; p.omega = omega; p.Nmax = 100000;

        std::vector<double> v_out, diff_out;
        int iters = 0; double resid = 0.0; double error_val = 0.0;

        // ==========================================
        // 1-Й И 2-Й ЧЕЛОВЕК (ТЕСТОВАЯ ЗАДАЧА)
        // ==========================================
        if (id == 1 || id == 2) {
            if (id == 1) {
                // ВНИМАНИЕ: Если у 1-го в конструкторе TaskConfig, а не TaskParams, 
                // компилятор ругнется. Убедись, что они используют TaskParams из shared!
                PoissonSolver_p1 solver(p, f_star_func, u_exact_func);
                solver.init_grid();
                solver.solve();
                iters = solver.get_iterations();
                resid = solver.get_last_diff(); 
                error_val = solver.calculate_max_error();
                v_out = solver.get_v();
            } else {
                PoissonSolver_p2 solver(p, f_star_func, u_exact_func);
                solver.init_grid();
                solver.solve();
                iters = solver.get_iterations();
                resid = solver.get_last_diff();
                error_val = solver.calculate_max_error();
                v_out = solver.get_v();
            }

            diff_out.assign(v_out.size(), 0.0);
            for (int i = 0; i <= n; ++i) {
                for (int j = 0; j <= m; ++j) {
                    double x = p.a + i * (p.b - p.a) / n;
                    double y = p.c + j * (p.d - p.c) / m;
                    int idx = i * (m + 1) + j;
                    diff_out[idx] = std::abs(v_out[idx] - u_exact_func(x, y));
                }
            }
        } 
        // ==========================================
        // 3-Й, 4-Й И 5-Й ЧЕЛОВЕК (ОСНОВНАЯ ЗАДАЧА)
        // ==========================================
        else {
            SolverResult r1;
            if (id == 3) r1 = solve_main_seidel_p3(p);
            else if (id == 4) r1 = solve_main_sor_p4(p);
            else if (id == 5) r1 = solve_main_sor_p5(p);

            // Решаем на сетке x2 для получения eps2 (сгущение сетки)
            TaskParams p2 = p; p2.n *= 2; p2.m *= 2;
            SolverResult r2;
            if (id == 3) r2 = solve_main_seidel_p3(p2);
            else if (id == 4) r2 = solve_main_sor_p4(p2);
            else if (id == 5) r2 = solve_main_sor_p5(p2);

            iters = r1.iterations_done; 
            resid = r1.final_residual;
            v_out = r1.values; 
            diff_out.assign(v_out.size(), 0.0);

            for (int i = 0; i <= n; ++i) {
                for (int j = 0; j <= m; ++j) {
                    int idx1 = i * (m + 1) + j;
                    int idx2 = (2 * i) * (2 * m + 1) + (2 * j);
                    diff_out[idx1] = std::abs(r1.values[idx1] - r2.values[idx2]);
                    error_val = std::max(error_val, diff_out[idx1]);
                }
            }
        }

        res.set_content(to_json(iters, resid, error_val, n, m, v_out, diff_out), "application/json");
    });

    std::cout << "Server started at http://localhost:8080" << std::endl;
    svr.listen("0.0.0.0", 8080);
    return 0;
}