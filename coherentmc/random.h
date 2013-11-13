#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>

// setup the internal state of the random number generator
// must be called before using other functions
void random_open(int seed, int set_rand_array_size);

// free up the internal state of the random number generator
// must be called before terminating the program
void random_close(void);

double random_double(void);

// Calculate two random angles (in radians) generated from a uniform
// distribution on the sphere. The first argument is the angle between the
// +z-axis and the sample, the second argument is the angle (in the x-y plane)
// measured counter-clockwise from the positive x-axis.
void random_direction(double *phi, double*psi);

// Calculate a number following a negative exponential distribution with mean
// one.
void random_distance(double *distance);


#endif // RANDOM_H
