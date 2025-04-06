#!/usr/bin/python3

import numpy as np
from VectorAdd import basic_add

a = np.array([1., 2., 3., 4.])
b = np.array([2., 4., 6, 8.])
o = np.zeros(4, dtype=np.float64)

basic_add (o, a, b)
