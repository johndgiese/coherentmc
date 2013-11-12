#include <iso646.h>
#include <complex.h>

#include "coherentmc.h"
#include "util.h"
#include "a2c.h"
#include "a4d.h"

int main(int argc, const char *argv[])
{
    int nx = 5;
    int ny = 4;
    int nz = 3;
    int np = 10;
    double index = 1.33;
    double wavelength = 500e-9;

    Result* result;
    Setup* setup;
    setup = newSetup(nx, ny, nz, index);

    result = run(setup, np, wavelength);
    printf("\nTOP\n");
    a2c_print(result->reflectance);
    printf("\nBOTTOM\n");
    a2c_print(result->transmission);

    freeSetup(setup);
    freeResult(result);

    return 0;
}
