# multiplication
def multiply (a, b):
   print "Will compute", a, "times", b, ":", a*b
   return a * b

# summation
def summation (x):
   summ = sum (list (x))
   print "summation of x:", summ
   return summ

# summation subset
def sub_summation (x, n):
   summ = sum (list (x[0:n]))
   print "sub summation of x:", summ
   return summ
