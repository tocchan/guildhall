Collision Response
======

## Overview
Suggested tasks and order to do them; 

- Move Rigidbodies/Colliders to buckets seperated by simulation mode. 
- Update a physics step;
  - Update all dynamics
  - Resolve collisions
    - Dynamics vs Statics (*Optional: Squirrel recommended for better results*)
    - Dynamics vs Dynamics 
    - Dynamics vs Statics (*Leaves objects not stuck in the floor*)
- Implement Collision Resolution
  - Implement manifold generation for existing primitives
  - Implement correction logic
    - Dynamic vs Static (full push out of static)
    - Dynamic vs Dynamic (seperation is based on your mass)
    - **WEEK1 Checkpoint**
  - Implement elastic collision response
    - Dynamic vs Static (reflect velocity along normal of impact)
    - Dynamic vs Dynamic (correctly transfer momentum)
  - Test thoroughly
  - **Final Checkin**

## Buckets

```cpp
class RigidbodyBudcket
{
  public:
    std::vector<Rigidbody2D*> m_rigidbodies; 
    // std::vector<Collider2D*> 
}
```

## Manifold Generation

## Correction

### Dynamic -vs- Static

### Dynamic -vs- Dynamic

## Elastic Collision

### Dynamic -vs- Static

### Dynamic -vs- Dynamic





