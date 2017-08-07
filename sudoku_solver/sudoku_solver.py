#!/usr/bin/python

import numpy as np

data = np.zeros( (10, 9, 9), dtype=int)

data[0, 0:10, 0:10] = np.array([[4, 8, 7, 0, 0, 2, 1, 6, 5], [0, 5, 0, 0, 0, 7, 8, 0, 0], [2, 0, 0, 8, 0, 6, 0, 0, 0], [0, 1, 4, 0, 0, 0, 0, 0, 6], [0, 0, 2, 9, 0, 4, 3, 0, 0], [6, 0, 0, 0, 0, 0, 2, 7, 0], [0, 0, 0, 4, 0, 1, 0, 0, 7], [0, 0, 9, 7, 0, 0, 0, 8, 0], [7, 2, 8, 6, 0, 0, 4, 5, 1]])

data[1, 0:10, 0:10] = 1 * np.ones( (9, 9), dtype=int)
data[2, 0:10, 0:10] = 2 * np.ones( (9, 9), dtype=int)
data[3, 0:10, 0:10] = 3 * np.ones( (9, 9), dtype=int)
data[4, 0:10, 0:10] = 4 * np.ones( (9, 9), dtype=int)
data[5, 0:10, 0:10] = 5 * np.ones( (9, 9), dtype=int)
data[6, 0:10, 0:10] = 6 * np.ones( (9, 9), dtype=int)
data[7, 0:10, 0:10] = 7 * np.ones( (9, 9), dtype=int)
data[8, 0:10, 0:10] = 8 * np.ones( (9, 9), dtype=int)
data[9, 0:10, 0:10] = 9 * np.ones( (9, 9), dtype=int)

groups = np.zeros( (10, 3, 3), dtype=int)
groups[0, 0:4, 0:4] = 0 * np.ones( (3, 3), dtype=int)
groups[1, 0:4, 0:4] = 1 * np.ones( (3, 3), dtype=int)
groups[2, 0:4, 0:4] = 2 * np.ones( (3, 3), dtype=int)
groups[3, 0:4, 0:4] = 3 * np.ones( (3, 3), dtype=int)
groups[4, 0:4, 0:4] = 4 * np.ones( (3, 3), dtype=int)
groups[5, 0:4, 0:4] = 5 * np.ones( (3, 3), dtype=int)
groups[6, 0:4, 0:4] = 6 * np.ones( (3, 3), dtype=int)
groups[7, 0:4, 0:4] = 7 * np.ones( (3, 3), dtype=int)
groups[8, 0:4, 0:4] = 8 * np.ones( (3, 3), dtype=int)
groups[9, 0:4, 0:4] = 9 * np.ones( (3, 3), dtype=int)

for row in range(0, 9):
   for col in range(0, 9):

      # remove possibilities if there is already a number here
      for num in range(1, 10):
         if data[0, row, col] != 0:
            data[num, row, col] = 0;

      # remove duplicates in this row
      for icol in range(0, 9):
         if data[0, row, icol] != 0:
            data[ data[0, row, icol], row, col] = 0

      # remove duplicates in this row
      for irow in range(0, 9):
         if data[0, irow, col] != 0:
            data[ data[0, irow, col], row, col] = 0

for grow in range(0, 3):
   for gcol in range(0, 3):

      # generate the groups
      for grow_offset in range(0, 3):
         for gcol_offset in range(0, 3):
            number = data[0, grow * 3 + grow_offset, gcol * 3 + gcol_offset]
            groups[number, grow, gcol] = 0;

      for grow_offset in range(0, 3):
         for gcol_offset in range(0, 3):
            pass

print data
print groups
