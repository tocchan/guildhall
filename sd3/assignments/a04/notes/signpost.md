Signpost Allocator
======

## Basic Idea
I have one or more pools of memory.  We distribute this memory by marking sections of it
as either free or in use, as well as distance to both the previous and next allocation.  

These markers, or **signposts**, are where the allocator gets its  name.

## General Algorithm

1. Start with a pool of memory.
2. Put a signpost near the beginning marking the entire allocation as free.

### Allocation

```
void* SignpostAllocator::alloc( size_t size )
   Find a free signpost that is large enough for the allocation desired.
   If no signpost - return nullptr

   Mark current signpost as in use; 
   If signpost is large enough to be broken into two pieces...
      Create a new signpost far enough ahead to make room for allocation
      Fix up all pointers...
         My next
         Old next previous
         New signpost prev & next

   return address just past signpost (possibly moved forward)
```

### Free
Some one of the post-conditions of any operation should be that you'll never 
have two free posts next to each other.  If the operation were to do that, 
you instead combine the contiguous free blocks together...

```
void SignpostAllocator::free( void* allocation )
   Move back to signpost associated with allocation

   If previous and next is free (remove myself case)...
      Update previous signpost to point to next taken block (next's next)
      Update next's next to point back to previous.  
   Else if only next is free (remove next signpost case)...
      Mark my next to the taken block (next's next)
      Point next's next back to me
      Mark myself as free
   Else if previous is free (removing myself)...
      Point previous to my next
      Point next's previous to my previous...
   Else... (both my neighbors are taken)
      Mark myself as free...

```

#### Case 1 - Removing between two free 
For these examples, let's assume a signpost is 8 bytes large, allowing for 2GB allocations (one bit is used to mark free, leaving us one unused bit).  I'm going to store the mile-markers in bytes (not including the signpost size)

`_` represents a free byte, and `#` will represent a used byte; 

We will always be freeing the the second signpost...

`{...[-n, 4,f]____[-4,4,u]####[-4,4,f]____[-4,n,u ]...}`

In this case, since my previous ane next neighbors are free, they get combined into a single free block.

(note, if my previous is free, and my next is free, that implies two back would be used, and two next would be used since no two free blocks should be next to each other.)

So afterwares we should have...
`{...[-n,28,f]____________________________[-28,n,u]...}`

Notice the last and previous signposts point back to each other.


### Case 2 - Next is free

`{...[-n, 4,u]####[-4,4,u ]####[-4,4,f]----[-4,n,u]...}`

In this case, I can't combine with previous, but can combine with next, 
so I do just that; 

`{...[-n, 4,u]####[-4,16,f]________________[-16,n,u]...}`


### Case 3 - Previous is free, but next isn't
Hopefully by now, you can see were this is going, but just to finish out the pictures;

`{...[-n, 4,f]____[-4,4,u]####[-4,4,u ]####[-4,n,u ]...}`

Goes to...

`{...[-n,16,f]________________[-16,4,u]####[-4,n,u ]...}`

### Case 4 - Both my neighbors are in use
Simplest case..

`{...[-n, 4,f]####[-4,4,u]####[-4,4,f ]####[-4,n,u ]...}`

Goes to

`{...[-n, 4,f]####[-4,4,f]____[-4,4,f ]####[-4,n,u ]...}`

#### Alternative Free
For less code, but possibly a little more work, you can also just do...

- If next is free, next free case
- If previous is free, do previous free case
- Mark myself as free

In that order... *Why is that order important important?*  Marking yourself free at the end is always safe, because
if you're being destroyed, you're only changing unused memory, so you don't hurt anyone. 

## Optimizations

Currently, freeing is `O(1)` time, but allocation can be `O(n)`.  *Why?*

... (talk more about this in lecture)

(note to self: talk about alternatives to measuring sizes in bytes)


## Thread Safety
The unsafe part of this algorithm is when you're updating the signposts (maintaining the linked-lists).  Putting a
lock around `alloc` and `free` will make this safe.

In the `free` case, this is fine. It is a very short function and we're not impacted by the lock.

In the `alloc` case, this is worse due to the relatively large search times for a free block (even after 
optimization).  


## Fragmentation
This allocator has both *internal* and *external* fragmentation.   Though the *internal* is fairly minimal 
(only in cases where an allocation wouldn't leave enough room for a new signpost, ie, a minimum allocation size).  

External is far worse.  Due to how things cut up, it is easy to construct a case where even though there is enough
space for an allocation, it is not contiguous.

For example, allocated alternatively `1024B` and `512B` allocations until you fail.  Now free all the `512B` 
allocations.  Even though technically 33% of the pool is free, I can no longer allocate a single `1024B` allocation.   

Windows can skirt this issue by **defragmenting** the memory, and fixing up the virtual memory table.  If you have raw
access to memory though, that is not an option.  

### Ways to help External Fragmentation
Though this won't help the theoritical case above, there are ways we can help fragmentation.  

- When allocating, always try to use the smallest free signpost that is large enough for your needs.  This 
promotes reusing blocks that are the same size as something recently freed, and keeps larger blocks free 
to when larger allocations are needed.

- Use a rather large base size.  So for example, instead of allocating to a byte count, allocate to a byte count 
that is a multiple of some minimum allocation size (for example, `128B`).  This results in more internal 
fragmentation, but has the benefit of common allocations (which are usually small) being reused more often, 
and not creating as many strange allocations.  

