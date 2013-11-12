import cmc
import cmcutils


if __name__ == "__main__":
    nx = 5
    ny = 4
    nz = 3
    index = 1.4
    wavelength = 100e-6

    np = 100

    scatterer_positions = cmcutils.random_scatterer_positions(nx, ny, nz)
    setup = cmc.Setup(scatterer_positions, index)

    print(setup.scatterer_positions)

    result = cmc.run(setup, np, wavelength)
    print(result.np)
    cmc.run_again(setup, result, np)
    print(result.np)

    print(result.reflectance)
    print(result.transmission)


