#!/usr/bin/python3

import numpy as np
from numpy import linalg as la
from matplotlib import pyplot as plt

np.random.seed (0)

A = np.random.rand (2,2)

print ("A:")
print (A)

U, S, Vh = la.svd (A)

print ("SVD (A): A = U * S * Vh where")
print ("U (det = " + str (la.det (U)) + "):")
print (U)
print ("S:")
print (S)
print ("Vh (det = " + str (la.det (Vh)) + "):")
print (Vh)

w, v = la.eig (A)
print ("eigen decomposition (A):")
print ("w:")
print (w)
print ("v:")
print (v)

N = 129
thetas = np.linspace (0.0, 2.0*np.pi, N, endpoint = True)
u = np.zeros ((2, N))

for ind, theta in enumerate (thetas):
   u[:,ind] = [np.cos (theta), np.sin (theta)]

Au = np.matmul (A, u)

Vhu   = np.matmul (Vh, u)
SVhu  = np.matmul (np.eye (2) * S, Vhu)
USVhu = np.matmul (U, SVhu)

print ("|Au - USVhu| = " + str (la.norm (Au - USVhu)))

plt.figure ()
plt.plot (    u[0],     u[1], color = 'k'); plt.plot (    u[0,0],     u[1,0], 'x', color = 'k')
plt.plot (    u[0,int (0.03*N)],     u[1,int (0.03*N)], 'o', color = 'k')
plt.plot (  Vhu[0],   Vhu[1], color = 'r'); plt.plot (  Vhu[0,0],   Vhu[1,0], 'x', color = 'r')
plt.plot (  Vhu[0,int (0.03*N)],   Vhu[1,int (0.03*N)], 'o', color = 'r')
plt.plot ( SVhu[0],  SVhu[1], color = 'g'); plt.plot ( SVhu[0,0],  SVhu[1,0], 'x', color = 'g')
plt.plot ( SVhu[0,int (0.03*N)],  SVhu[1,int (0.03*N)], 'o', color = 'g')
plt.plot (USVhu[0], USVhu[1], color = 'b'); plt.plot (USVhu[0,0], USVhu[1,0], 'x', color = 'b')
plt.plot (USVhu[0,int (0.03*N)], USVhu[1,int (0.03*N)], 'o', color = 'b')

plt.plot ([0, v[0,0]], [0, v[1,0]], color = 'k')
plt.plot ([0, v[0,1]], [0, v[1,1]], color = 'k')
plt.show ()
