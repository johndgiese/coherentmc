#include <iso646.h>
#include <stdbool.h>
#include <stdio.h>

#include <math.h>
#include <complex.h>

#include "coherentmc.h"
#include "a4d.h"
#include "a2c.h"
#include "random.h"

typedef struct {
    double r_prev[3];
    double r[3];
    int xi;
    int yi;
    int zi;
    double distance;
} Photon;

Photon *new_photon();
bool is_ballistic(Setup *setup, Photon *photon);
bool propagate_photon(Setup *setup, Photon *photon);
void tally_diffuse_photon(Result *result, Photon *photon, double wavelength);
void tally_ballistic_photon(Result *result, Photon *photon, double wavelength);
double distance(double a[3], double b[3]);


// run the coherent monte carlo simulation
void run(Setup *setup, Result *result, int np, double wavelength) {

    random_open(10, 2000);
    double reduced_wavelength = wavelength/setup->index;

    for(int ip = 0; ip < np; ip++) {
        Photon *p = new_photon();
        if (not is_ballistic(setup, p)) {
            while(propagate_photon(setup, p));
            tally_diffuse_photon(result, p, reduced_wavelength);
        } else {
            tally_ballistic_photon(result, p, reduced_wavelength);
        }
        free(p);
    }

    result->np += np;
    random_close();
}


// Did the photon pass through the sample without scattering at all?
bool is_ballistic(Setup *setup, Photon *photon) {
    return photon->r[2] > setup->nz;
}


// Assume photon is centered at the origin, and is launched in the +z-direction.
Photon *new_photon() {
    Photon* p = (Photon*) malloc(sizeof(Photon));

    // TODO: generalize
    p->r_prev[0] = p->r_prev[1] = p->r_prev[2] = 0;

    p->r[0] = p->r[1] = 0;
    p->r[2] = p->distance = random_distance();
    return p;
}


bool propagate_photon(Setup *setup, Photon *photon) {

    // keep a record of the previous location
    photon->r_prev = photon->r;

    double displacement[3] = random_displacement();
    double z_before_contstraining = displacement[2] + photon->r[2];

    // DIFFUSE TRANSMISSION
    if (z_before_contstraining > setup->nz) {

        // TODO: force photon to lie on z=nz plane
        // TODO: handle wrap around
        return false;
    }


    // DIFFUSE REFLECTION
    if (z_before_contstraining < 0) {

        // TODO: force photon to lie on z=0 plane
        // TODO: handle wrap around
        return false;
    }

    // REMAINS IN SAMPLE

    // contstrain the displacement to a scatterer
    photon->xi = (photon->xi + lround(displacement[0]));
    photon->yi = (photon->yi + lround(displacement[1]));
    photon->zi = (photon->zi + lround(displacement[2]));

    // keep track of wrap around boundaries
    int wrap_arounds_x = photon->xi/setup->nx;
    int wrap_arounds_y = photon->yi/setup->ny;
    bool is_wrapping = wrap_arounds_x > 0 or wrap_arounds_y > 0;
    if (is_wrapping) {
        photon->xi %= setup->nx;
        photon->yi %= setup->ny;
    }

    // update the position using the wrapped position
    double r_wrapped[3] = setup->scatterer_positions->data[photon->xi][photon->yi][photon->zi];
    photon->r[0] = r_wrapped[0];
    photon->r[1] = r_wrapped[1];
    photon->r[2] = r_wrapped[2];

    // update the photon distance travelled using the unwrapped position
    double r_unwrapped[3];
    r_unwrapped[0] = r_wrapped[0] + wrap_arounds_x*setup->nx;
    r_unwrapped[1] = r_wrapped[1] + wrap_arounds_y*setup->ny;
    r_unwrapped[2] = r_wrapped[2];
    photon->distance += distance(r_unwrapped, photon->r_prev);

    return true;
}


void tally_diffuse_photon(Result *result, Photon *photon, double wavelength) {
    // for debugging
    if (p->r[2] == 0) {
        result->reflection->data[0][0] += 1 + I*p->pathlength;
    } else {
        result->transmission->data[0][0] += 1 + I*p->pathlength;
    }
}


void tally_ballistic_photon(Result *result, Photon *photon, double wavelength) {
    // for debugging
    result->reflection->data[1][1] += 1;
}

double distance(double a[3], double b[3]) {
    return sqrt(
        (a[0] - b[0])*(a[0] - b[0]) +
        (a[1] - b[1])*(a[1] - b[1]) +
        (a[2] - b[2])*(a[2] - b[2])
    );
}
