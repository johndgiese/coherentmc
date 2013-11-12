#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "a4d.h"


a4d *a4d_empty(int nx, int ny, int nz, int nc) {
    a4d *array = (a4d*) malloc(sizeof(a4d));
    array->data = (double*) malloc(sizeof(double)*nx*ny*nz*nc);
    array->nx = nx;
    array->ny = ny;
    array->nz = nz;
    array->nc = nc;
    return array;
}

void a4d_initialize(a4d *array, double value) {
    int num_elements = a4d_size(array);
    for (int i = 0; i < num_elements; i++) {
        array->data[i] = value;
    }
}

a4d *a4d_zeros(int nx, int ny, int nz, int nc) {
    a4d *array = a4d_empty(nx, ny, nz, nc);
    a4d_initialize(array, 0);
    return array;
}

a4d *a4d_ones(int nx, int ny, int nz, int nc) {
    a4d *array = a4d_empty(nx, ny, nz, nc);
    a4d_initialize(array, 1);
    return array;
}

void a4d_free(a4d *array) {
    free(array->data);
    free(array);
}

a4d *a4d_copy(a4d *array) {
    int nx = array->nx;
    int ny = array->ny;
    int nz = array->nz;
    int nc = array->nc;

    a4d *copy = a4d_empty(nx, ny, nz, nc);

    copy->nx = nx;
    copy->ny = ny;
    copy->nz = nz;
    copy->nc = nc;

    int num_elements = nx*ny;
    memcpy(array->data, copy->data, num_elements);

    return copy;
}

int a4d_size(a4d *array) {
    return array->nx*array->ny*array->nz*array->nc;
}

double a4d_get(a4d *array, int ix, int iy, int iz, int ic) {
    int ny = array->ny;
    int nz = array->nz;
    int nc = array->nc;
    return array->data[ix*ny*nz*nc + iy*nz*nc + iz*nc + ic];
}

void a4d_set(a4d *array, double val, int ix, int iy, int iz, int ic) {
    int ny = array->ny;
    int nz = array->nz;
    int nc = array->nc;
    array->data[ix*ny*nz*nc + iy*nz*nc + iz*nc + ic] = val;
}

