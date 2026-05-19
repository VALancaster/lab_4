#ifndef GRID_H
#define GRID_H

#include "poisson_base.h"

class Grid {
public:
    Grid(const TaskParams& p) : params(p) {
        hx = (params.b - params.a) / params.n;
        hy = (params.d - params.c) / params.m;
    }

    // Получение индекса в одномерном массиве для узла (i, j)
    inline int index(int i, int j) const {
        return i * (params.m + 1) + j;
    }

    // Получение координат по индексу узла
    inline double x(int i) const { return params.a + i * hx; }
    inline double y(int j) const { return params.c + j * hy; }

    double hx, hy;
    TaskParams params;
};

#endif // GRID_H