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

    // just for debugging
    for (int ix = 0; ix < setup->nx; ix++) {
        for (int iy = 0; iy < setup->ny; iy++) {
            a2c_set(result->reflectance, random_double(), ix, iy);
            a2c_set(result->transmission, random_double(), ix, iy);
        }
    }

    result->np += np;

    random_close();
}

