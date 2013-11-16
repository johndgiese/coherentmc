#ifndef RANDOM_H
#define RANDOM_H

#include <stdlib.h>
#include <tgmath.h>
#include <math.h>
#include <inttypes.h>

#include "./dSFMT/dSFMT.h"

#include "random.h"
void random_update_array(void);


// The dSFMT code can generate random numbers much faster if it does a full
// array at a time.  Unfortunately, it is much more convenient to grab a single
// double at a time.  To take advantage of the speed boost granted when
// generating a full array, I generate an array at a time, and use a static
// position variable (inside the random_double() function) to only grab a new
// array when necessary.
double *rand_array;

// The size of this array is also stored in a global; the size of this array can
// be specified when opening the random number generator, however there is a
// minimum size--for this reason the array is dynamically allocated and must be
// freed by the random_close() function.
int rand_array_size;


void random_open(uint32_t seed, int set_rand_array_size) {
    dsfmt_gv_init_gen_rand(seed);
    int min_rand_array_size = dsfmt_get_min_array_size();
    rand_array_size = fmax(min_rand_array_size, set_rand_array_size);
    rand_array = (double*) malloc(sizeof(double)*rand_array_size);
    random_update_array();
}


void random_close(void) {
    free(rand_array);
}


// grab a new vector full of numbers (used internally)
void random_update_array(void) {
    dsfmt_gv_fill_array_close_open(rand_array, rand_array_size);
}


// return a new random double
double random_double(void) {
    static int position = 0;
    double rand = rand_array[position];
    position = (position + 1) % rand_array_size;
    if (position == 0) {
        random_update_array();
    }
    return rand;
}


void random_direction(double *theta, double*phi) {
    *theta = 2*M_PI*random_double();
    *phi = acos(2*random_double() - 1);
}


void random_distance(double *distance) {
    *distance = -log(random_double());
}

double[3] random_displacement(void) {
    displacement double[3];
    double r = -log(random_double());
    double theta = 2*M_PI*random_double();
    double cos_phi = 2*random_double() - 1;
    double sin_phi = sqrt(1 - cos_phi*cos_phi);
    displacement[0] = r*sin_phi*cos(theta);
    displacement[1] = r*sin_phi*sin(theta);
    displacement[2] = r*cos_phi;
    return displacement;
}

#endif // RANDOM_H
