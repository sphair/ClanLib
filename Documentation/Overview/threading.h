
/*! \page Threading Threading

MainDocs:Threading

From ClanLib Game SDK
 

The following classes in ClanLib are related to threads: 
• CL_Thread, CL_Runnable 
• CL_Mutex, CL_MutexSection 
• CL_ThreadLocalStorage 

The Thread Class

CL_Thread is the class representing a handle to a thread. It exports functions to start threads, wait on threads and so on. To start a thread, call the CL_Thread::start function. The start function exist in several different versions, which allow you to specify how to execute the thread main function in a way most convenient for your appliation. 

One way is to use a method very common in other languages or libraries, such as Java. The application derives from the class CL_Runnable and implements the CL_Runnable::run function. A pointer to this class is then given to CL_Thread::start: 


class MyRunnable : public CL_Runnable
{
public:
	void run()
	{
		CL_Console::write_line("Hello from another thread");
	}
};
 
MyRunnable runnable;
CL_Thread thread;
thread.start(&runnable);
thread.join();

Although this method is simple and easy to understand, it also forces you to constantly derive a class each time you need to start a thread. Some of the other CL_Thread::start functions allows you to specify a function directly to CL_Thread::start: 


void my_thread_main()
{
	CL_Console::write_line("Hello from another thread");
}
 
CL_Thread thread;
thread.start(&my_thread_main);
thread.join();

The function is not limited to be a static function and just like the ClanLib template callback classes, you can specify a member function and one or more user-data parameters to be passed along: 


class MyClass
{
public:
	void thread_main(int param1, CL_String param2)
	{
		CL_Console::write_line("Hello from another thread");
	}
};
 
MyClass my_class;
CL_Thread thread;
thread.start(&my_class, &MyClass::thread_main, 42, "Hello");
thread.join();

To wait on a thread to complete its execution, call CL_Thread::join. If you do not call this function, the destructor of CL_Thread will not wait for your thread to complete - it will simply just close its handle to the thread. This means your thread will continue to run, but you will be unable to interact further with it. 

Synchronization

Classes in ClanLib are re-entrant (aka. apartment model), unless otherwise specified. This means that a class instance can only be safely accessed from one thread at a time. To ensure only one thread accesses data or functions simultanously, ClanLib provides the CL_Mutex class. A mutex is an object that can be locked by only one thread at the same time. If any other thread tries to lock the mutex, it will block until the first thread releases its lock. 


class MyClass
{
public:
	CL_Mutex mutex;
	CL_String variable;
 
	void thread_main1()
	{
		for (int i = 0; i < 10000; i++)
		{
			mutex.lock();
			variable = "Modified from thread 1";
			mutex.unlock();
		}
	}
 
	void thread_main2()
	{
		for (int i = 0; i < 10000; i++)
		{
			mutex.lock();
			variable = "Modified from thread 2";
			mutex.unlock();
		}
	}
};
 
MyClass my_class;
CL_Thread thread1, thread2;
thread1.start(&my_class, &MyClass::thread_main1);
thread2.start(&my_class, &MyClass::thread_main2);
thread1.join();
thread2.join();

It is vital to a program's execution that a mutex lock is always freed - otherwise you would eventually get a deadlock of all threads working with the data guarded by the mutex. If an exception was thrown in the above thread main functions, the mutex would not get unlocked. This can be solved by inserting a try / catch clause, or it could be changed to use CL_MutexSection to lock the mutex instead. CL_MutexSection locks a mutex in its constructor and releases it in its destructor, which ensures that all exit paths of a function will eventually unlock a mutex: 


void thread_main1()
{
	for (int i = 0; i < 10000; i++)
	{
		CL_MutexSection mutex_lock(&mutex);
		variable = "Modified from thread 1";
	}
}

Thread Local Storage

Thread local storage, or TLS for short, is the mechanism by which each thread in a given multithreaded process allocates storage for thread-specific data. This can usually be done using compiler specific keywords, i.e. __declspec(thread) int tls_i = 1; for Visual C++. However, these usually imply some limitations that make it unsuitable for storing class variables in the TLS. 

CL_ThreadLocalStorage provides a way to store CL_SharedPtr memory managed objects in the TLS. The function CL_ThreadLocalStorage::get_variable returns a different variable for every thread. 

A simple example: 


class MyClass
{
public:
	CL_String str;
	int i;
};
 
void thread_main()
{
	// Retrieve TLS variable:
	CL_SharedPtr tls_var(
		CL_ThreadLocalStorage::get_variable("my_var"));
 
	if (tls_var.is_null())
	{
		// First time this thread tries to get this object.
		// Create object and store it.
		tls_var = CL_SharedPtr(new MyClass);
		CL_ThreadLocalStorage::set_variable("my_var", tls_var);
	}
 
	tls_var->str = "Hello";
	tls_var->i = 42;
}
 
CL_Thread thread1, thread2;
thread1.start(&thread_main);
thread2.start(&thread_main);
thread1.join();
thread2.join();

If the thread has not been created by CL_Thread, the application must create a CL_ThreadLocalStorage instance in the thread before using the TLS functions. The TLS variables will be destroyed when the CL_ThreadLocalStorage object is destroyed. For CL_Thread objects, this happens when the thread main function finishes. 



*/
