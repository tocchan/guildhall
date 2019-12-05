
## Decimal Numbers

So when dealing with decimal values, we usually represent them in the form...

`final_value = value * pow( base, exponent )`

For for example, we can represent the number 102.4 as...

```
102.4 * pow(10,  0)
1024 *  pow(10, -1)
1.024 * pow(10,  2)  <-- note: normalized form
```

... so on and so forth.  

When dealing with decimals in programming, we have two major tools we use, 
**floating point notation** and **fixed point notation**. 

- **fixed point** - The base and exponent is fixed, allowing for a smaller but uniform range of values. 
- **floating point notation** - Base is fixed, but both exponent and value are specified.  Allows the us to represent 
  a larger range, though less precise.  

------

## Floating Point Numbers
- https://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html
- https://en.wikipedia.org/wiki/IEEE_754
- https://blog.penjee.com/binary-numbers-floating-point-conversion/
- https://www.ias.ac.in/article/fulltext/reso/021/01/0011-0030

Floating point numbers encode both the value and the exponent into the binary representation.
As mentioned above though, there are multiple ways to represent the same value, 
so IEEE 754 (the standard we use for floats) will pick a **representative value** from each group, 
which we will call the **normalized** or **normal** value. 

A **normal value** is a value that starts with 1, followed by any number of decimal digits; 

Layout is...
- SIGN (1b) - EXPONENT (8b) - MANTISSA (23b)
- ...but usually descried as 1-23-8 (ie, sign, precision, exponent)

- **Mantissa**
  - *Mantissa* refers the the numbers that follow the decimal point (ie: `fract`).  
  - Normalized Form

- **Exponenet**
  - ...

Special Case Expononents...
- All 1s...
  - Infinity if the mantissa is 0 (ie, 1)
  - NaN if non-zero (Not A Number)
    - QNaN - Quiet NaN - not an error and is carried forward (0 / 0)
    - SNaN - Signalling NaN - a bad error, such as dividing by (n / 0, where n != 0  )
- All 0s...
  - Zero if mantissa is 0
  - DEN (DENormalized) (ie, subnormal, dennormal)
    - Extremely small numbers not representable by normal floating point rules
    - Mantissa is now assumed to have a leading 0 to represent these numbers (ie, we only have so many we only have (1^24 - 1) values, each with a lot of gaps between them
    - On some architectures - denormalized numbers have a significant speed penalty (up to 100x slower).  
    - You can disable denormal numbers as a compiler option.  This will result in any operation that WOULD be #DEN to instead go to zero (preserving sign)
      - https://software.intel.com/en-us/cpp-compiler-developer-guide-and-reference-ftz-qftz


### 16-bit floats

- Shading Languages can support 16-bit floats (1 - 10 - 5)
- Follow a lot of the same rules; 


### Talking Points
- At what point do I start losing precision - when does it matter?
- What is the minimum and maximum numbers I can represent with a given reprsentation
- Subnormal values should be avoided if possible; 
  - Subnormal is any value less then the minimum normal value; 

------

## Fixed Point Math
You may come across fixed point notation in file foramts (maps/fonts/etc), as well as some math/networking
libaries, so just 

### Benefits/Drawbacks
- Easy to understand
- Not natively supported (usually) (though the math is just integer math)
- Very well defined behaviour (easy to tell your precision, and where things break down)
  - Safe over a network (different machines/compilers/etc may compute floating point operations differntly, but don't for integer types)
    - This is mostly due to order of operations with floats
  - Easy to compress
- Conversion from floats not the fastest, so less intuitive to use as a front facing interface






### Implementation
- Representation (whole/fraction)
  - How to pick your precision
  
- Converting to and from floats (simple)
  - Divide by your precision (2^p) (better yet, store the quotient off and multiply)
  - Multiply by your precision(2^p)

- Addition/Subtraction
- Multiply/Dividde




### `UNORM` and `SNORM` Formats
- In rendering, these are a form of fixed point notation that map the entire
number range to 0 to 1

- `SNORM` has two values for -1.0f (min value, and min_value + 1), to ensure an even spacing)
- `UNORM` maps 0 to 0, max to 1.0f


