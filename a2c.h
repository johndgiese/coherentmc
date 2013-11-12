#ifndef A2C_H
#define A2C_H

#include <stdlib.h>
#include <complex.h>

typedef struct {
    int nx;
    int ny;
    complex double *data;
} a2c;

a2c *a2c_empty(int nx, int ny);
void a2c_initialize(a2c *array, complex double value);
a2c *a2c_zeros(int nx, int ny);
a2c *a2c_ones(int nx, int ny);

void a2c_free(a2c *array);
a2c *a2c_copy(a2c *array);

int a2c_size(a2c *array);

complex double a2c_get(a2c *array, int ix, int iy);
void a2c_set(a2c *array, complex double val, int ix, int iy);

#endif // A2C_H

