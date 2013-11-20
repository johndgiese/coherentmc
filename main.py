import cmc
import cmcutils

from pylab import *
import mymath


if __name__ == "__main__":
    nx = 50
    ny = 20
    nz = 20
    index = 1.4
    wavelength = 100e-6

    np = 10000

    scatterer_positions = cmcutils.random_scatterer_positions(nx, ny, nz)
    setup = cmc.Setup(scatterer_positions, index)
    result = cmc.run(setup, np, wavelength, 250)

    print(result.transmittance[0][0].real)
    print(result.reflectance[0][0].real)
    print(result.transmittance[0][0].imag)
    print(result.reflectance[0][0].imag)


