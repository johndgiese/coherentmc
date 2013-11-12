#include <iso646.h>
#include <stdbool.h>
#include <stdio.h>

#include <math.h>
#include <complex.h>

#include "coherentmc.h"
#include "util.h"
#include "a4d.h"
#include "a2c.h"


void run(Setup *setup, Result *result, int np, double wavelength) {

    // just for debugging
    for (int ix = 0; ix < setup->nx; ix++) {
        for (int iy = 0; iy < setup->ny; iy++) {
            a2c_set(result->reflectance, 7, ix, iy);
            a2c_set(result->transmission, 7, ix, iy);
        }
    }

    result->np += np;
}

