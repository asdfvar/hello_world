#!/usr/bin/python3

import ctypes

# Load the shared library
example_lib = ctypes.CDLL ("./example.so")

# Specify the argument and return types for the 'add' function
example_lib.add.argtypes = [ctypes.c_int, ctypes.c_int]
example_lib.add.restype = ctypes.c_int

# Call the 'add' function
result = example_lib.add (5, 3)
print ("The sum is: %d" % result)

# Specify the argument type for the 'greet' function
example_lib.greet.argtypes = [ctypes.c_char_p]

# Call the 'greet' function
example_lib.greet ("World".encode ('utf-8'))
