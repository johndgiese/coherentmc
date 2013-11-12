#include <stdlib.h>

#ifndef UTIL_H
#define UTIL_H

#include "coherentmc.h"
#include "util.h"
#include "a2c.h"
#include "a4d.h"


Setup* newSetup(int nx, int ny, int nz, double index) {
    Setup *setup = (Setup*) malloc(sizeof(Setup));
    setup->nx = nx;
    setup->ny = ny;
    setup->nz = nz;
    setup->index = index;
    setup->scatterer_positions = a4d_zeros(nx, ny, nz, 3);
    return setup;
}

void freeSetup(Setup* setup) {
    a4d_free(setup->scatterer_positions);
    free(setup);
}


Result* newResult(Setup* setup, double wavelength) {
    Result* result = (Result*) malloc(sizeof(Result));
    int nx = setup->nx;
    int ny = setup->ny;
    result->nx = nx;
    result->ny = ny;
    result->reflectance = a2c_zeros(nx, ny);
    result->transmission = a2c_zeros(nx, ny);
    result->np = 0;
    result->wavelength = wavelength;
    return result;
}


void freeResult(Result* result) {
    a2c_free(result->reflectance);
    a2c_free(result->transmission);
    free(result);
}

#endif // UTIL_H
