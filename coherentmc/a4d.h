#ifndef A4D_H
#define A4D_H

#include <stddef.h>
#include <string.h>

typedef struct {
    int nx;
    int ny;
    int nz;
    int nc;
    double *data;
} a4d;

a4d *a4d_empty(int nx, int ny, int nz, int nc);
void a4d_initialize(a4d *array, double value);
a4d *a4d_zeros(int nx, int ny, int nz, int nc);
a4d *a4d_ones(int nx, int ny, int nz, int nc);

void a4d_free(a4d *array);
a4d *a4d_copy(a4d *array);

int a4d_size(a4d *array);

double a4d_get(a4d *array, int ix, int iy, int iz, int ic);
void a4d_set(a4d *array, double val, int ix, int iy, int iz, int ic);
void a4d_plus_set(a4d *array, double val, int ix, int iy, int iz, int ic);

#endif // A4D_H

