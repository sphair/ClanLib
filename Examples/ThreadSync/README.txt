         Name: ThreadSync
       Status: Windows(Y), Linux(Y)
        Level: Advanced
   Maintainer: Core developers
      Summary: Thread condition synchronisation example.

Thread condition synchronisation example. This shows how to use
the wait/notify features of the mutex class although the example is
a bit short.

The queue class here basically implements a synchronous queue that
can be accessed safely by more than one thread. Here is how it works.

If a thread calls 'getItem' this is what happens:
   - The class takes out a mutex. If any other thread is currently
     operating on the queue, it waits for them to finish here.
   - It checks if the queue is empty. If it isn't, it reads the front
     value, releases the mutex and returns.
   - If the queue is empty, because this is a *synchronous* queue, we
     want to wait until something arrives in the queue. Therefore
     the queue calls 'wait()' on the mutex. This is the important bit.
     wait causes the following:
     - The calling thread goes to sleep
     - The mutex on the queue is released (a mutex has to be locked
       before wait is called!).
     Therefore, since the mutex is released, another process can now
     read or write the queue... continued below.

When a thread calls 'putItem' the following steps are taken:
   - The queue takes out a mutex (which blocks if someone else is
     currently reading or writing).
   - It puts the item on the queue.
   - It releases the mutex.
   - It calls 'notify' on the mutex. This is the important bit here.
     Notify wakes up the first thread that has gone to sleep on the
     queue. That thread will wake up and the mutex it has given up
     will be relocked. In this queue, calls only go to sleep in the
     read function if there are no items. Hence, after putting an
     item in we wake the first sleeping thread up so it can read its
     item.

Here is an example sequence, there are two threads, A and B. The part
on the left indicates the thread
   A - getItem()     - Thread A wants to read
   A - enter()       - A now holds a lock on the queue.
   B - putItem(1)    - Thread B wants to write
   B - enter()       - Thread B tries to lock and goes to sleep
   A - size==0       - yes
   A - wait()        - A waits for signals now. B wakes up because
	                       A has released its mutex with wait.
   B - push(1)
   B - leave()       - B gives up its mutex
   B - notify()      - B wakes up the first waiting thread, which is A
	                       A automatically gets its mutex back.
   A - size==0       - no (remember the size check is in a loop..)
   A - front()       - A reads the queue
   A - leave()       - A unlocks the mutex
