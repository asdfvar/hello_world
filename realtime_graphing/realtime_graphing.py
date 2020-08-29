#!/usr/bin/python3

import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time
import numpy as np

fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)

def animate (i):
   xar = np.random.rand (1000)
   yar = np.random.rand (1000)
   ax1.clear()
   ax1.plot(xar,yar)

ani = animation.FuncAnimation(fig, animate, interval=100)
plt.show()
