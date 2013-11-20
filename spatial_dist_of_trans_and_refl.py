import cmc
import cmcutils

from pylab import *


if __name__ == "__main__":
    # all lengths are in units of x transport_mean_free_path
    nx = 51
    ny = 51
    nz = 20
    index = 1.4
    wavelength = 1/200.0

    np = 300000

    seed = 360
    scatterer_positions = cmcutils.random_scatterer_positions(nx, ny, nz)
    setup = cmc.Setup(scatterer_positions, index)
    result = cmc.run(setup, np, wavelength, seed)

    IT = abs(result.transmittance)**2
    IR = abs(result.reflectance)**2
    print(sum(IR), sum(IT), sum(IR) + sum(IT), np)

    Imax = amax(hstack((IT, IR)))

    IRnorm = IR/Imax
    ITnorm = IT/Imax
    print(amax(IRnorm))
    print(amax(ITnorm))

    figure()
    subplot(221)
    imshow(ITnorm)
    title('transmittance')
    subplot(222)
    imshow(IRnorm)
    title('reflectance')

    subplot(223)
    imshow(abs(result.incoherent_transmittance))
    title('incoherent transmittance')
    subplot(224)
    imshow(abs(result.incoherent_reflectance))
    title('incoherent reflectance')

    show()
