import os
import ctypes as ct
import unittest

import numpy as np
import pylab as pl
import mymath


PLOTTING = True


class TestRandomDisplacement(unittest.TestCase):

    def setUp(self):
        libc = ct.cdll.LoadLibrary("../randutils.so")
        self.libc = libc
        self.random_displacement = libc.random_displacement
        self.random_displacement.argtypes = [ct.POINTER(ct.c_double)]
        seed = 100
        self.libc.random_open(seed, 1000)

    def shutDown(self):
        self.libc.random_close()

    def rand_points(self, length):
        pts = np.empty((length, 3))
        ctypes_point = (ct.c_double*3)(0, 0, 0)

        for ip in xrange(length):
            self.random_displacement(ctypes_point)
            pts[ip, 0] = ctypes_point[0]
            pts[ip, 1] = ctypes_point[1]
            pts[ip, 2] = ctypes_point[2]

        return pts

    def test_exponential_distribution(self):
        num_points = 10000
        pts = self.rand_points(num_points)
        length = np.empty(num_points)
        for ip in xrange(num_points):
            length[ip] = np.sqrt(pts[ip][0]**2 + pts[ip][1]**2 + pts[ip][2]**2)

        self.assertAlmostEqual(np.mean(length), 1 + 1, places=1)
        self.assertGreaterEqual(np.amin(length), 1)

        if PLOTTING:
            pl.hist(length, 100)
            show()

    def test_radial_distribution(self):
        num_points = 10000
        pts = self.rand_points(num_points)
        for ip in xrange(num_points):
            pts[ip] /= np.sqrt(pts[ip][0]**2 + pts[ip][1]**2 + pts[ip][2]**2)

        average_pt = np.mean(pts, 0)
        self.assertAlmostEqual(np.sqrt(np.sum(average_pt**2)), 0, places=1)
        self.assertAlmostEqual(average_pt[0], 0, places=1)
        self.assertAlmostEqual(average_pt[1], 0, places=1)
        self.assertAlmostEqual(average_pt[2], 0, places=1)

        if PLOTTING:
            mymath.scatter3(pts[:200, 0], pts[:200, 1], pts[:200, 2])
            pl.title("Random points on the unit sphere")
            pl.show()
        

        

if __name__ == '__main__':
    unittest.main()

