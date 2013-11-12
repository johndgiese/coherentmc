#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <stdio.h>

#include "a2c.h"


a2c *a2c_empty(int nx, int ny) {
    a2c *array = (a2c*) malloc(sizeof(a2c));
    int num_elements = a2c_size(array);
    array->data = (complex double*) malloc(sizeof(complex double)*num_elements);
    array->nx = nx;
    array->ny = ny;
    return array;
}

void a2c_initialize(a2c *array, complex double value) {
    int num_elements = a2c_size(array);
    for (int i = 0; i < num_elements; i++) {
        array->data[i] = value;
    }
}

a2c *a2c_zeros(int nx, int ny) {
    a2c *array = a2c_empty(nx, ny);
    a2c_initialize(array, 0);
    return array;
}

a2c *a2c_ones(int nx, int ny) {
    a2c *array = a2c_empty(nx, ny);
    a2c_initialize(array, 1);
    return array;
}

void a2c_free(a2c *array) {
    free(array->data);
    free(array);
}

a2c *a2c_copy(a2c *array) {
    int nx = array->nx;
    int ny = array->ny;

    a2c *copy = a2c_empty(nx, ny);

    copy->nx = nx;
    copy->ny = ny;

    int num_elements = nx*ny;
    memcpy(array->data, copy->data, num_elements);

    return copy;
}

int a2c_size(a2c *array) {
    return array->nx*array->ny;
}

complex double a2c_get(a2c *array, int ix, int iy) {
    int ny = array->ny;
    return array->data[ix*ny + iy];
}

void a2c_set(a2c *array, complex double val, int ix, int iy) {
    int ny = array->ny;
    array->data[ix*ny + iy] = val;
}
