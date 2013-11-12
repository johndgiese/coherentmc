### Coherent Monte Carlo Simulation

Monte Carlo simulations of the Radiative Transport Equation are very popular in
the field of biomedical optics (and beyond).

There has recently been a good deal of interest in coherent scattering effects,
such as the "memory effect".  All monte carlo models that I am aware of are
unable to account for coherent effects in a physically consistent manner because
scattering events occur non-deterministically through out the sample.

This repository contains a coherent monte carlo simulator that is able to
properly model coherent scattering effects.

The key insight is that the scattering events must be deterministic.  This is
accomplished by keeping track of all the individual scatter locations.  Keeping
track of the individual scatterer positions ensures that the scattering, while
random, it is deterministic.  This is phyiscally more accurate---as we all know,
speckle patterns generated from scattering through a static object, while
random, are deterministic.

There are many other monte carlo models that are capable of producing speckle
patterns.  Speckle patterns are the most visible coherent effect, however other
more subtle coherent effects such as the memory effect are not so easily
described.  Furthermore, most such simulations, if run a second time (or even if
the number of photons in the simulation is altered) will produce a different
speckle pattern.

Assumptions behind the model:
* Uses a discrete scatter approximation
* The sample is a single slab
* The edges of the slab have periodic boundary conditions
* There is no absorption
* Scatters are isotropic

