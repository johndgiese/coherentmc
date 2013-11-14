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

    np = 100

    scatterer_positions = cmcutils.random_scatterer_positions(nx, ny, nz)
    setup = cmc.Setup(scatterer_positions, index)

    result = cmc.run(setup, np, wavelength)


    theta = real(result.transmission.reshape(nx*ny))
    phi = imag(result.transmission.reshape(nx*ny))
    sphi = sin(phi)

    x = cos(theta)*sphi
    y = sin(theta)*sphi
    z = cos(phi)

    mymath.scatter3(x, y, z)
    show()

    

