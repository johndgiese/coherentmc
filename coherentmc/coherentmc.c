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
    double position[3];
    double distance;
} Photon;

Photon new_photon(Setup *setup);
bool propagate_photon(Setup *setup, Photon *photon);
void tally_result(Result *result, Photon photon, double wavelength);



// The main function!
void run(Setup *setup, Result *result, int np, double wavelength) {

    random_open(10, 2000);

    double reduced_wavelength = wavelength/setup->index;

    for(int ip = 0; ip < np; ip++) {
        Photon p = new_photon(setup);
        while(propagate_photon(setup, p));
        tally_result(result, p, reduced_wavelength);
    }

    result->np += np;

    random_close();
}



// For now, assume photon is centered at the origin, and is launched in the +z
// direction.  Also assume there is no ballistic light.
Photon new_photon(Setup *setup) {
    Photon p;
    p.x = p.y = 0;
    do {
        p.z = p.distance = random_distance();
    } while (p.z < setup->nz);
    return p;
}

bool propagate_photon(Setup *setup, Photon *photon) {
    double next_position[3];


}
