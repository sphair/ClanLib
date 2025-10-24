/*
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
*/

#include <ClanLib/core.h>
#include <ClanLib/application.h>

#include <queue>
#include <iostream>

class TestQueue 
{
public:
	TestQueue()
	{
		mutex = CL_Mutex::create();
	}

	void putItem(int i)
	{
		mutex->enter();

		items.push(i);

		mutex->notify();
		mutex->leave();
	}

	int getItem()
	{
		mutex->enter();

		while (items.size()==0)
		{
			mutex->leave();
			mutex->wait();
			mutex->enter();
		}

		int i=items.front();
		items.pop();

		mutex->leave();

		return i;
	}

private:
	std::queue<int> items;
	CL_Mutex *mutex;
};

class TestWriter : public CL_Runnable 
{
public:
	TestWriter(TestQueue& q) : que(q) { }

	virtual void run() {
		int i=0;

		while (true)
		{
			std::cout << "Writing value " << i << " into the queue." << std::endl << std::flush;

			que.putItem(i);

			std::cout << "Finished writing " << i << " into the queue." << std::endl << std::flush;

			i++;

			CL_System::sleep(10);
		}
	}

private:
	TestQueue& que;
};

class TestReader : public CL_Runnable 
{
public:
	TestReader(TestQueue& q) : que(q) { }

	virtual void run()
	{
		while (true)
		{
			std::cout << "Reading queue." << std::endl << std::flush;

			int i=que.getItem();

			std::cout << "Read " << i << " from queue." << std::endl << std::flush;
		}
	}

private:
	TestQueue& que;
};

class ThreadSyncApp : public CL_ClanApplication
{
public:
	virtual int main(int, char **)
	{
		CL_ConsoleWindow console("My console");
		console.redirect_stdio();

		CL_SetupCore setup_core;

		TestQueue q;
		TestReader r(q);
		TestWriter w(q);

		CL_Thread t1(&r);
		CL_Thread t2(&w);

		t1.start();
		t2.start();

		t1.wait();

		// Display console close message and wait for a key
		console.display_close_message();

		return 0;
	}
} app;
