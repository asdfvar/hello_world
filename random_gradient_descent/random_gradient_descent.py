#!/usr/bin/python3

import numpy as np
from numpy import linalg as la
from matplotlib import pyplot as plt

# coef_0 + coef_1*x + coef_*x^2 + ...
def evaluate_polynomial (coefficients, a, b, dx):
   y = np.arange (a, b + dx, dx)
   result = np.zeros (len (y))
   x = np.ones (len (y))
   for ind, coef in enumerate (coefficients):
      result += coef * x
      x *= y
   return result

# Integrate via Simpson's rule
def integrate (function, dx):
   result = function[0]            + \
   np.sum (4.0 * function[1:-2:2]) + \
   np.sum (2.0 * function[2:-2:2]) + \
   function[-1]
   result *= dx / 3.0
   return result

degree = 3

truth = 2.0 * np.random.rand (degree) - 1.0; dx = 0.0001
fx = evaluate_polynomial (truth, 0.0, 1.0, dx)
area = integrate (fx, dx)

estimate = 2.0 * np.random.rand (degree) - 1.0
gx = evaluate_polynomial (estimate, 0.0, 1.0, dx)
final_error = integrate (np.abs (gx - fx), dx)

plt_error = []

step = 0.1
for itt in range (100):

   found_improvement = False
   for guess in range (1000):
      offset = 2.0 * np.random.rand (degree) - 1.0
      offset /= la.norm (offset)
      offset *= step
      gx = evaluate_polynomial (estimate + offset, 0.0, 1.0, dx)
      error = integrate (np.abs (gx - fx), dx)
      if error < final_error:
         final_error = error
         final_offset = offset
         found_improvement = True

   if found_improvement:
      estimate += final_offset
   else:
      step *= 0.1

   plt_error.append (final_error)

print ("truth     = " + str (truth   ))
print ("estimate  = " + str (estimate))
print ("step size = " + str (step    ))
plt.figure ()
plt.semilogy (plt_error)

plt.figure ()
fx = evaluate_polynomial (truth, 0.0, 1.0, dx)
gx = evaluate_polynomial (estimate, 0.0, 1.0, dx)
plt.plot (np.linspace (0.0, 1.0, len (fx)), fx, color = 'k')
plt.plot (np.linspace (0.0, 1.0, len (gx)), gx, color = 'g')
plt.show ()
