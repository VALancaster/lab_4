#ifndef P3_SOLVER_H
#define P3_SOLVER_H

#include "poisson_base.h"

// Функция для решения основной задачи методом Зейделя
// На вход принимает параметры задачи, возвращает структуру с результатом
SolverResult solve_main_seidel_p3(TaskParams params);

#endif // P3_SOLVER_H