import cmc
import numpy


def random_scatterer_positions(nx, ny, nz):
    """
    Generate a grid describing a random scatter slab.

    This is usually used in conjunction with the cmc.Setup object.

    Example:
        import cmc

        index = 1.33; np = 100; wavelength = 500e-9;

        sp = random_scatter_positions(10, 10, 5)
        setup = cmc.Setup(sp, index)
        result = cmc.run(setup, np, wavelength)

    """

    data = numpy.random.rand(nx, ny, nz, 3)
    for ix in xrange(nx):
        for iy in xrange(ny):
            for iz in xrange(nz):
                data[ix, iy, iz, 0] = ix
                data[ix, iy, iz, 1] = iy
                data[ix, iy, iz, 2] = iz
    return data
