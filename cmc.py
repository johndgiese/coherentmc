import os
import ctypes as ct

import numpy


__all__ = ["Setup", "Result", "run", "run_again"]


class c_complex_double(ct.Structure):
    """
    Type for c99 basic _Complex double type.

    The ctypes doesn't represent the complex type natively, however the c99
    spec indicates that _Complex doubles are required to be stored in this manner.
    """

    _fields_ = [
        ("real", ct.c_double),
        ("imag", ct.c_double),
    ]


class a4d(ct.Structure):
    """
    Structure used internally by the c-simulaion code.

    Since traditional c-arrays can only be used if the size is known at
    compilation time, and variable length arrays don't appear to be supported
    by ctypes, a custom array type is used, called a4d for 4d array of doubles.

    See the header file for details on how to use this in c.
    """

    _fields_ = [
        ("nx", ct.c_int),
        ("ny", ct.c_int),
        ("nz", ct.c_int),
        ("nc", ct.c_int),
        ("data", ct.POINTER(ct.c_double)),
    ]


class a2c(ct.Structure):
    """
    Structure used internally by the c-simulaion code.

    See docstring for a4d; this one is a 2d array of complex doubles.
    """
    _fields_ = [
        ("nx", ct.c_int),
        ("ny", ct.c_int),
        ("data", ct.POINTER(c_complex_double)),
    ]


class Setup(ct.Structure):
    """
    Object representing a scattering slab.

    The object is constructed using two parameters:
    * scatterer_positions - 4-dimensional numpy array of doubles
    * index - a double describing the index of refraction in the slab

    The first three dimensions of the scatterer_positions ndarray index a grid,
    and the fourth dimension always has length 3, and contains the coordinates
    of the scatterer inside that grid position.

    The grid spacing is implicitly assumed to be a single mean transport length.
    """

    _fields_ = [
        ("nx", ct.c_int),
        ("ny", ct.c_int),
        ("nz", ct.c_int),
        ("_scatterer_positions", ct.POINTER(a4d)),
        ("index", ct.c_double),
    ]

    def __init__(self, scatterer_positions, index):
        self.scatterer_positions = scatterer_positions
        nx, ny, nz, nc = scatterer_positions.shape
        scatter_positions_ptr = scatterer_positions.ctypes.data_as(ct.POINTER(ct.c_double))
        
        data = a4d(nx, ny, nz, nc, scatter_positions_ptr)

        super(Setup, self).__init__(nx, ny, nz, ct.pointer(data), index)


class Result(ct.Structure):
    """
    A result object from a coherent monte carlo simulation.

    Contains two complex ndarrays represnting the diffuse reflectance and
    diffuse transmittance.

    Also has a counter keeping track of the number of photons used to generate
    this result, and the wavelength of the light in the simulation.
    """

    _fields_ = [
        ("nx", ct.c_int),
        ("ny", ct.c_int),
        ("_reflectance", ct.POINTER(a2c)),
        ("_transmittance", ct.POINTER(a2c)),
        ("wavelength", ct.c_double),
        ("np", ct.c_int),
    ]

    def __init__(self, setup, wavelength):
        nx, ny, nz, nc = setup.scatterer_positions.shape

        self.transmittance = numpy.zeros((nx, ny), numpy.complex128)
        transmittance_data_ptr = self.transmittance.ctypes.data_as(ct.POINTER(c_complex_double))
        transmittance = ct.pointer(a2c(nx, ny, transmittance_data_ptr))

        self.reflectance = numpy.zeros((nx, ny), numpy.complex128)
        reflectance_data_ptr = self.reflectance.ctypes.data_as(ct.POINTER(c_complex_double))
        reflectance = ct.pointer(a2c(nx, ny, reflectance_data_ptr))

        super(Result, self).__init__(nx, ny, reflectance, transmittance, wavelength, 0)



libc = ct.cdll.LoadLibrary("./coherentmc/coherentmc.so")
_run = libc.run
_run.argtypes = [ct.POINTER(Setup), ct.POINTER(Result), ct.c_int, ct.c_double]
_run.restype = ct.c_void_p


def run(setup, np, wavelength):
    """
    Calculate the diffuse transmittance and reflection through setup.

    Use np photon launches, and the specified wavelength.  Returns a result
    object.
    """
    result = Result(setup, wavelength)
    _run(ct.pointer(setup), ct.pointer(result), np, wavelength)
    return result


def run_again(setup, result, np):
    """
    Run a simulation again adding more photons into a result.

    This function is useful in two situations:

    1. You want to run more photons on the same setup
    2. You want to simulate a dynamic setup by changing the scatterer positions
       between runs.

    Note: You can not change the wavelength after it has been set.
    """
    wavelength = result.wavelength
    _run(ct.pointer(setup), ct.pointer(result), np, wavelength)
    return result
