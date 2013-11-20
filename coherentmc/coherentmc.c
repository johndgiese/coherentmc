#include <iso646.h>
#include <stdbool.h>
#include <stdio.h>

#include <assert.h>
#include <tgmath.h>

#include "coherentmc.h"
#include "a4d.h"
#include "a2c.h"
#include "randutils.h"

typedef struct {
    double r_prev[3];
    double r[3];
    long double pathlength;
} Photon;

static inline Photon *new_photon(Setup *setup);
static inline bool propagating_in_sample(Setup *setup, Photon *p);
static inline bool is_transmitted(Setup *setup, Photon *p);
static inline bool is_reflected(Setup *setup, Photon *p);
static inline void tally_photon(Setup *setup, Result *result, Photon *p, double wavelength);
static inline double distance(double a[3], double b[3]);
static inline void interpolate_to_z(Photon *p, double zm);
static inline int positive_mod(int x, int d);

#ifdef DEBUG
    #define DPRINT(str) printf(#str "\n")
    #define DPRINT_INT(str) printf("%s = %d\n", #str, (str))
    #define DPRINT_DBL(str) printf("%s = %g\n", #str, (str))
    #define DPRINT_LDB(str) printf("%s = %Lg\n", #str, (str))
    #define DPRINT_CDB(str) printf("%s = %g + %gI\n", #str, creal(str), cimag(str))
    #define ASSERT(exp) assert(exp)
#else
    #define DPRINT(str) ;
    #define DPRINT_INT(str) ;
    #define DPRINT_DBL(str) ;
    #define DPRINT_LDB(str) ;
    #define DPRINT_CDB(str) ;
    #define ASSERT(exp) ;
#endif

// run the coherent monte carlo simulation
void run(Setup *setup, Result *result, int np, double wavelength, int seed) {

    random_open(seed, 2000);

    for(int ip = 0; ip < np; ip++) {
        Photon *p = new_photon(setup);
        if (not is_transmitted(setup, p)) {
            while(propagating_in_sample(setup, p));
        }
        tally_photon(setup, result, p, wavelength);
        free(p);
    }

    result->np += np;
    random_close();
}


// Assume photon is centered at the origin, and is launched in the +z-direction.
static inline Photon *new_photon(Setup *setup) {
    Photon* p = (Photon*) malloc(sizeof(Photon));

    // Photon starts in the middle of the sample, at z = 0
    p->r[0] = setup->nx/2.0;
    p->r[1] = setup->ny/2.0;
    p->r[2] = 0.0;

    p->r[0] = setup->nx/2.0;
    p->r[1] = setup->ny/2.0;
    p->r[2] = random_distance();

    p->pathlength = distance(p->r, p->r_prev);

    return p;
}


static inline bool is_transmitted(Setup *setup, Photon *p) {
    return p->r[2] > setup->nz;
}


static inline bool is_reflected(Setup *setup, Photon *p) {
    return p->r[2] < 0.0;
}


static inline bool propagating_in_sample(Setup *setup, Photon *p) {

    // KEEP A RECORD OF THE PREVIOUS LOCATION
    p->r_prev[0] = p->r[0];
    p->r_prev[1] = p->r[1];
    p->r_prev[2] = p->r[2];

    // GENERATE A RANDOM DISPLACEMENT
    double displacement[3];
    random_displacement(displacement);
    p->r[0] += displacement[0];
    p->r[1] += displacement[1];
    p->r[2] += displacement[2];

    // CHECK IF THE PHOTON LEAVES THE SAMPLE
    if (is_transmitted(setup, p) or is_reflected(setup, p)) {
        // the pathlength contribution of the last propagation is handled later
        return false;
    }

    // OTHERWISE CONSTRAIN THE DISPLACEMENT TO A SCATTERER POSITION
    int xi = floor(p->r[0]);
    int yi = floor(p->r[1]);
    int zi = floor(p->r[2]);

    // calculate how many times each photon wraps around the boundary conditions
    // the simulation should be setup to avoid multiple wrap arounds, however
    // the code "properly" handles it anyay
    int x_wrap_arounds = xi/setup->nx + (xi < 0 ? -1 : 0);
    int y_wrap_arounds = yi/setup->ny + (yi < 0 ? -1 : 0);

    bool is_wrapping = x_wrap_arounds != 0 or y_wrap_arounds != 0;

    if (x_wrap_arounds != 0) {
        xi = positive_mod(xi, setup->nx);
    }
    if (y_wrap_arounds != 0) {
        yi = positive_mod(yi, setup->ny);
    }

    double x_wrapped = a4d_get(setup->scatterer_positions, xi, yi, zi, 0);
    double y_wrapped = a4d_get(setup->scatterer_positions, xi, yi, zi, 1);
    double z_wrapped = a4d_get(setup->scatterer_positions, xi, yi, zi, 2);
    p->r[0] = x_wrapped;
    p->r[1] = y_wrapped;
    p->r[2] = z_wrapped;

    // CALCULATE PATHLENGTH, ACCOUNTING FOR WRAPPING
    if (is_wrapping) {
        double r_unwrapped[3];
        r_unwrapped[0] = x_wrapped + x_wrap_arounds*setup->nx;
        r_unwrapped[1] = y_wrapped + y_wrap_arounds*setup->ny;
        r_unwrapped[2] = z_wrapped;
        p->pathlength += distance(r_unwrapped, p->r_prev);
    } else {
        p->pathlength += distance(p->r, p->r_prev);
    }
    return true;
}


static inline void tally_photon(Setup *setup, Result *result, Photon *p, double wavelength) {
    bool photon_is_reflected = is_reflected(setup, p);

    // at this point, p->r is some point outside of the sample, p->r_prev is
    // the previous photon location which is gaurunteed to be in the sample,
    // and p->pathlength accounts for photon travel up to p->r
    if (photon_is_reflected) {
        interpolate_to_z(p, 0);
    } else {
        interpolate_to_z(p, setup->nz);
    }

    p->pathlength += distance(p->r, p->r_prev);
    double k = setup->index*2*M_PI/wavelength;
    complex double field_contribution = exp(-I*k*p->pathlength);

    int xi = positive_mod(lround(p->r[0]), setup->nx);
    int yi = positive_mod(lround(p->r[1]), setup->ny);

    if (photon_is_reflected) {
        a2c_plus_set(result->reflectance, field_contribution, xi, yi);
        a2c_plus_set(result->incoherent_reflectance, 1, xi, yi);
    } else {
        a2c_plus_set(result->transmittance, field_contribution, xi, yi);
        a2c_plus_set(result->incoherent_transmittance, 1, xi, yi);
    }
}


static inline double distance(double a[3], double b[3]) {
    return sqrt(
        (a[0] - b[0])*(a[0] - b[0]) +
        (a[1] - b[1])*(a[1] - b[1]) +
        (a[2] - b[2])*(a[2] - b[2])
    );
}


static inline void interpolate_to_z(Photon *p, double zm) {

    double dz = abs((zm - p->r_prev[2])/(p->r[2] - p->r_prev[2]));

    double dxdz = p->r[0] - p->r_prev[0];
    double dydz = p->r[1] - p->r_prev[1];

    p->r[0] = p->r_prev[0] + dxdz*dz;
    p->r[1] = p->r_prev[1] + dydz*dz;
    p->r[2] = zm;
}


static inline int positive_mod(int x, int d) {
    return (x % d + d) % d;
}

