#include <iso646.h>
#include <stdbool.h>
#include <stdio.h>

#include <math.h>
#include <complex.h>

#include "coherentmc.h"
#include "a4d.h"
#include "a2c.h"
#include "randutils.h"

typedef struct {
    double r_prev[3];
    double r[3];
    int xi;
    int yi;
    int zi;
    int x_wrap_arounds;
    int y_wrap_arounds;
    double pathlength;
} Photon;

static inline Photon *new_photon(void);
static inline bool is_ballistic(Setup *setup, Photon *p);
static inline bool propagate_photon(Setup *setup, Photon *p);
static inline bool handle_wrapping(Setup *setup, Photon *p);
static inline void tally_diffuse_photon(Result *result, Photon *p, double wavelength);
static inline void tally_ballistic_photon(Result *result, Photon *p, double wavelength);
static inline double distance(double a[3], double b[3]);

#define DEBUG

#ifdef DEBUG
    #define DPRINT(str) printf(#str "\n");
    #define DPRINT_I(str) printf(#str " = %d\n", str);
    #define DPRINT_D(str) printf(#str " = %g\n", str);
#else
    #define DPRINT(str) ;
    #define DPRINT_I(str) ;
    #define DPRINT_D(str) ;
#endif
    

// run the coherent monte carlo simulation
void run(Setup *setup, Result *result, int np, double wavelength) {

    random_open(10, 2000);
    double reduced_wavelength = wavelength/setup->index;

    for(int ip = 0; ip < np; ip++) {
        printf("\n\n");
        DPRINT_I(ip);
        Photon *p = new_photon();
        if (not is_ballistic(setup, p)) {
            DPRINT(not ballistic)
            while(propagate_photon(setup, p));
            tally_diffuse_photon(result, p, reduced_wavelength);
        } else {
            DPRINT(ballistic)
            tally_ballistic_photon(result, p, reduced_wavelength);
        }
        free(p);
    }

    result->np += np;
    random_close();
}


// Did the photon pass through the sample without scattering at all?
static inline bool is_ballistic(Setup *setup, Photon *p) {
    return p->r[2] > setup->nz;
}


// Assume photon is centered at the origin, and is launched in the +z-direction.
static inline Photon *new_photon(void) {
    Photon* p = (Photon*) malloc(sizeof(Photon));

    // TODO: generalize
    p->r_prev[0] = p->r_prev[1] = p->r_prev[2] = 0;

    p->r[0] = p->r[1] = 0;
    p->r[2] = random_distance();
    p->pathlength = distance(p->r, p->r_prev);

    p->xi = p->yi = 0;
    p->zi = lround(p->r[2]);

    p->x_wrap_arounds = p-> y_wrap_arounds = 0;
    return p;
}


static inline bool handle_wrapping(Setup *setup, Photon *p) {

    // calculate how many times each photon wraps around the boundary conditions
    // the simulation should be setup to avoid multiple wrap arounds, however
    // the code "properly" handles it anyay
    p->x_wrap_arounds = p->xi/setup->nx + (p->xi < 0 ? -1 : 0);
    p->y_wrap_arounds = p->yi/setup->ny + (p->yi < 0 ? -1 : 0);

    bool is_wrapping = p->x_wrap_arounds != 0 or p->y_wrap_arounds != 0;

    if (is_wrapping) {
        // perform a true modulo
        p->xi = (p->xi % setup->nx + setup->nx) % setup->nx;
        p->yi = (p->yi % setup->ny + setup->ny) % setup->ny;
    }
    return is_wrapping;
}


static inline bool propagate_photon(Setup *setup, Photon *p) {

    // keep a record of the previous location
    p->r_prev[0] = p->r[0];
    p->r_prev[1] = p->r[1];
    p->r_prev[2] = p->r[2];

    double displacement[3];
    random_displacement(displacement);
    double z_before_contstraining = displacement[2] + p->r[2];

    // DIFFUSE TRANSMISSION
    if (z_before_contstraining > setup->nz) {

        DPRINT(Diffuse transmission)

        // TODO: force photon to lie on z=nz plane
        // TODO: handle wrap around
        return false;
    }


    // DIFFUSE REFLECTION
    if (z_before_contstraining < 0) {

        DPRINT(Diffuse reflection)

        // TODO: force photon to lie on z=0 plane
        // TODO: handle wrap around
        return false;
    }


    // REMAINS IN SAMPLE

    // constrain the displacement to a scatterer position
    p->xi += lround(displacement[0]);
    p->yi += lround(displacement[1]);
    p->zi += lround(displacement[2]);
    handle_wrapping(setup, p);
    double x_wrapped = a4d_get(setup->scatterer_positions, p->xi, p->yi, p->zi, 0);
    double y_wrapped = a4d_get(setup->scatterer_positions, p->xi, p->yi, p->zi, 1);
    double z_wrapped = a4d_get(setup->scatterer_positions, p->xi, p->yi, p->zi, 2);
    p->r[0] = x_wrapped;
    p->r[1] = y_wrapped;
    p->r[2] = z_wrapped;

    // update the photon's total pathlength travelled using the unwrapped position
    double r_unwrapped[3];
    r_unwrapped[0] = x_wrapped + p->x_wrap_arounds*setup->nx;
    r_unwrapped[1] = y_wrapped + p->y_wrap_arounds*setup->ny;
    r_unwrapped[2] = z_wrapped;
    p->pathlength += distance(r_unwrapped, p->r_prev);

    return true;
}


static inline void tally_diffuse_photon(Result *result, Photon *p, double wavelength) {
    // for debugging
    if (p->r[2] == 0) {
        a2c_plus_set(result->reflectance, 1 + I*p->pathlength, 0, 0);
    } else {
        a2c_plus_set(result->transmission, 1 + I*p->pathlength, 0, 0);
    }
}


static inline void tally_ballistic_photon(Result *result, Photon *p, double wavelength) {
    // for debugging
    a2c_plus_set(result->reflectance, 1, 1, 1);
}


static inline double distance(double a[3], double b[3]) {
    return sqrt(
        (a[0] - b[0])*(a[0] - b[0]) +
        (a[1] - b[1])*(a[1] - b[1]) +
        (a[2] - b[2])*(a[2] - b[2])
    );
}
