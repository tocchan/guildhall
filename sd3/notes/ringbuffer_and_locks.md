
## Thread Notifcation/Wakeup
If a worker thread is mostly idle, we usually want to sleep the thread and only wake it up when work is present as not to needlessly use resources.

In our consumer example - we can accomplish this a few ways, but two common approaches are to use either a **Semaphore** or **Condition Variable**.  

### Semeaphores
Semaphores work similarly to mutexes, except multiple people can have it "locked" at a time.  

It works similar to a "leave-a-penny, take-a-penny" system.  The semaphore has a certain capacity for tokens, and it starts with an initial count.  People can either add a token (`ReleaseSemaphore`), or try to take a token (`WaitForSingleObject`).  If no token is present, the calling thread waits until there is. 

We can use this for usage example by initializing a semaphore with a 0 count, and an arbitrary max (in this example, we can use 1).  

When we log a message, we'll release the semaphore.  The log thread, who is constantly trying to take will wake up, do as much work as they can, and then try to take again.  If no one else has release, the count will again be zero and he will go back to sleep; 


### Condition Variables
Condition Variables, as the name implies, are locks that wait on certain conditions.  

The idea is there is a shared piece of data (say, messages is ready), that is protected by a mutex.  You acquire the lock to this shared piece of data, and then enter a condition variable.  This waits until the condition variable is notified and the lock can be acquired. 

Once you are woken up, you have the lock, and can modify the value.  

If you prefer to stick with standard C++ and not use windows/pthread libraries, this is the option.   


## Reader-Writer Locks, aka: shared_mutex
Filters are objects we edit rarely, but read constantly.  

Reads can happen in parallel (nothing is changing, so multiple people reading doesn't pose a risk).  But if someone is writing, we want only one person to be allowed to write, and no one to read while this is happening.

The write would be considered an 'exclusive lock', while the reads are a 'shared_lock'.  

Exclusive locks only allow one person in the lock at once (and will wait for all reads to finish before locking).  Behaving much like a mutex. 

Any number of consective reads can happen at once, but once a write is requested, all current reads will finish, and new reads and will, until the write (exlusive lock) is complete; 
