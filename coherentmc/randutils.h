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
// distribution on the sphere. The first argument is the angle (in the x-y plane)
// measured counter-clockwise from the positive x-axis. The second argument is
// the angle between the +z-axis and the sample. 
void random_direction(double *theta, double*phi);

// Calculate a number following a negative exponential distribution with mean
// one.
double random_distance(void);

// Generate a random displacement
// Generates a random direction and displacement, and then converts to
// Cartesian coordinates
void random_displacement(double displacement[3]);

#endif // RANDOM_H
