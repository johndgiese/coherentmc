#ifndef COHERENTMC_H
#define COHERENTMC_H TRUE

#include <complex.h>

#include "a2c.h"
#include "a4d.h"

typedef struct {
    int nx;
    int ny;
    int nz;
    a4d *scatterer_positions;
    double index;
} Setup;

typedef struct {
    int nx;
    int ny;
    a2c *reflectance;
    a2c *transmittance;
    a2c *incoherent_reflectance;
    a2c *incoherent_transmittance;
    double wavelength;
    int np;
} Result;

void run(Setup* setup, Result* result, int np, double wavelength, int seed);

#endif // COHERENTMC_H

