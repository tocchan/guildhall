## Terms

- **Momentum**:  *p = velocity * mass*
- **Impulse**: Change in momenentum, `j = p_final - p_initial`
- **Kinetic Energy**: `K = 0.5f * (velocity * velocity) * mass`
- **Restitution**:  Energy conserved by the system.  `e = sqrt( K_final / K_initial )`


## Impulse Equation
Impulse can be thought of an instant change in momentum, which can translate to an 
instance change in velocity. 

**Impulse Along Normal: jn**
- jn: impulse along normal
- ma & mb: mass of object **a** and **b**
- e: Coefficient of Restitution (bounciness)
- va & vb: velocity of **a** and **b**
- n: normal of impact

`jn = ((ma * mb) / (ma + mb)) * (1 + e) * Dot( (vb - va), n)`

## ApplyImpulse

Once we have the impulse along normal, we apply it to both objects;

```cpp
rb0->ApplyImpulse( jn * n, impactPoint ); 
rb1->ApplyImpulse( -jn * n, impactPoint ); 
```

```cpp
void Rigidbody::ApplyImpulse( vec2 impulse, float point ) 
{
   m_velocity += impulse / mass; 
}