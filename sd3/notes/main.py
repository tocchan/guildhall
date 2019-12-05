# Comment in Python

# Function in Python
def Plus(a, b):
   return a + b

# Classes in Python
class MyClass:
   # member
   i = 12345

   def __init__(self): 
      # Constructor
      self.i = 98765
      
   def Method(self): 
      print( "Content: " + str(self.i) )


# test.py
def SomeFunction(a, b): 
  c = a + b
  print( "Sum of " + str(a) + " and " + str(b) + " is " + str(c) )

SomeFunction(5, 7)

# Call code from it
x = MyClass(); 
x.Method()

# Using my programs defined module
import squire
squire.log(123)

