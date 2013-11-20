from os import path

import cmc
import cmcutils

from pylab import *

FIGURE_DIR = 'figures'


# all lengths are in units of x transport_mean_free_path
nz_max = 40
nx = ny = 3*nz_max
index = 1.4
wavelength = 1/200.0

np = 300000

nz_vec = arange(2, nz_max)
p_transmitted = empty(nz_vec.shape)
p_reflectted = empty(nz_vec.shape)

for i, nz in enumerate(nz_vec):
    print("{:>5} of {}".format(i + 1, len(nz_vec)))
    seed = randint(100000)
    scatterer_positions = cmcutils.random_scatterer_positions(nx, ny, nz)
    setup = cmc.Setup(scatterer_positions, index)
    result = cmc.run(setup, np, wavelength, seed)
    p_transmitted[i] = sum(real(result.incoherent_transmittance))
    p_reflectted[i] = sum(real(result.incoherent_reflectance))

figure()
plot(nz_vec, p_transmitted)
plot(nz_vec, p_reflectted)
legend(('transmitted', 'reflected'), loc='center right')
xlabel('Sample thickness / transport scattering length')
ylabel('Power [a.u.]')
grid('on')
title('Transmitted and Reflected Power vs. Sample Thickness')

fname = path.splitext(path.basename(__file__))[0] + '.pdf'
fdir = path.join(FIGURE_DIR, fname)
savefig(fdir)

