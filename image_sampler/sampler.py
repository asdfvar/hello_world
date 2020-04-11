#!/usr/bin/python3

import numpy as np
from matplotlib import pyplot as plt
from matplotlib import image as mpimg

img = mpimg.imread ("/home/euler/Pictures/shingle_color_template_of_4.jpeg")

color = []
color.append (img[:, 0:160,   :])
color.append (img[:, 160:320, :])
color.append (img[:, 320:480, :])
color.append (img[:, 480:640, :])

for ind in range (4):
   plt.figure (ind)
   plt.subplot (2, 1, 1)
   plt.imshow (color[ind])
   plt.subplot (2, 1, 2)
   plt.hist (color[ind][:,:,0][0], 30, color = 'r')
   plt.hist (color[ind][:,:,1][0], 30, color = 'g')
   plt.hist (color[ind][:,:,2][0], 30, color = 'b')

   average = []
   average.append (color[ind][:,:,0][0].flatten ().mean ())
   average.append (color[ind][:,:,1][0].flatten ().mean ())
   average.append (color[ind][:,:,2][0].flatten ().mean ())

   print (average)

plt.show ()
