import cmc
import cmcutils


if __name__ == "__main__":
    nx = 500
    ny = 20
    nz = 20
    index = 1.4
    wavelength = 100e-6

    np = 100

    scatterer_positions = cmcutils.random_scatterer_positions(nx, ny, nz)
    setup = cmc.Setup(scatterer_positions, index)

    result = cmc.run(setup, np, wavelength)



