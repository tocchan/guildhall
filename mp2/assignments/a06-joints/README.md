A06 - Effectors and Joints
======

## Overview
A joint and effectos apply forces to an object based on a set of rules.  We'll be implementing on of each;  

- Spring Joint:  Applies forces to keep 

## Grading 

## Resources

## Demo

## Source (Headers)

## Checklist

- [ ] Implement Spring Joints 

    - [ ] Implement positional (x,y) constraints
       - [ ] For correction, solve correction on a per-axis basis, treating thing with contraints as static (so fully push in that direction)
       - [ ] For collision response, just solve as normal as if it didn't have a constraint (not sure if this is correct, but we'll see how it feels)
    - [ ] Implement rotational constraint
        - [ ] When resolving an impulse, use the rotational constraint to determine how much energy goes toward angular momentum and how much goes toward linear velocity;  