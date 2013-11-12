#ifndef UTIL_H
#define UTIL_H TRUE

#include <complex.h>

Result* newResult(Setup* setup, double wavelength);
void freeResult(Result* result);

Setup* newSetup(int nx, int ny, int nz, double index);
void freeSetup(Setup* setup);

#endif // UTIL_H

