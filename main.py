import cmc
import cmcutils

from pylab import *
import mymath


if __name__ == "__main__":
    # all lengths are in units of x transport_mean_free_path
    nx = 50
    ny = 50
    nz = 20
    index = 1.4
    wavelength = 1/200.0

    np = 10

    seed = 250
    scatterer_positions = cmcutils.random_scatterer_positions(nx, ny, nz)
    setup = cmc.Setup(scatterer_positions, index)
    result = cmc.run(setup, np, wavelength, seed)

    print(sum(abs(result.transmittance)))
    print(sum(abs(result.reflectance)))

    figure()
    subplot(121)
    imshow(abs(result.transmittance))
    title('transmittance')
    subplot(122)
    imshow(abs(result.reflectance))
    title('reflectance')
    show()
