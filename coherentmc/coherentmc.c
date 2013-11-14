#include <iso646.h>
#include <stdbool.h>
#include <stdio.h>

#include <math.h>
#include <complex.h>

#include "coherentmc.h"
#include "a4d.h"
#include "a2c.h"
#include "random.h"


void run(Setup *setup, Result *result, int np, double wavelength) {

    random_open(10, 2000);
    double distance;
    double theta, phi;

    // just for debugging
    for (int ix = 0; ix < setup->nx; ix++) {
        for (int iy = 0; iy < setup->ny; iy++) {
            random_direction(&theta, &phi);
            a2c_set(result->reflectance, theta + I*phi, ix, iy);

            random_direction(&theta, &phi);
            a2c_set(result->transmission, theta + I*phi, ix, iy);
        }
    }

    result->np += np;

    random_close();
}

