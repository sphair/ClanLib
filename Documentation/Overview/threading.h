
/*! \page Threading Threading

The following classes in ClanLib are related to threads: 
\ref clan::Thread, \ref clan::Runnable 
\ref clan::Mutex, \ref clan::MutexSection 
\ref clan::ThreadLocalStorage 

<h2>The Thread Class</h2>

\ref clan::Thread is the class representing a handle to a thread. It exports functions to start threads, wait on threads and so on. To start a thread, call the \ref clan::Thread::start function. The start function exist in several different versions, which allow you to specify how to execute the thread main function in a way most convenient for your appliation. 
<br/>
One way is to use a method very common in other languages or libraries, such as Java. The application derives from the class \ref clan::Runnable and implements the \ref clan::Runnable::run function. A pointer to this class is then given to \ref clan::Thread::start: 

\code
class MyRunnable : public clan::Runnable
{
public:
	void run()
	{
		clan::Console::write_line("Hello from another thread");
	}
};
 
MyRunnable runnable;
clan::Thread thread;
thread.start(&runnable);
thread.join();
\endcode

Although this method is simple and easy to understand, it also forces you to constantly derive a class each time you need to start a thread. Some of the other \ref clan::Thread::start functions allows you to specify a function directly to \ref clan::Thread::start: 

\code
void my_thread_main()
{
	clan::Console::write_line("Hello from another thread");
}
 
clan::Thread thread;
thread.start(&my_thread_main);
thread.join();
\endcode

The function is not limited to be a static function and just like the ClanLib template callback classes, you can specify a member function and one or more user-data parameters to be passed along: 

\code
class MyClass
{
public:
	void thread_main(int param1, clan::String param2)
	{
		clan::Console::write_line("Hello from another thread");
	}
};
 
MyClass my_class;
clan::Thread thread;
thread.start(&my_class, &MyClass::thread_main, 42, "Hello");
thread.join();
\endcode

To wait on a thread to complete its execution, call \ref clan::Thread::join. If you do not call this function, the destructor of \ref clan::Thread will not wait for your thread to complete - it will simply just close its handle to the thread. This means your thread will continue to run, but you will be unable to interact further with it. 

<h2>Synchronization</h2>

Classes in ClanLib are re-entrant (aka. apartment model), unless otherwise specified. This means that a class instance can only be safely accessed from one thread at a time. To ensure only one thread accesses data or functions simultanously, ClanLib provides the \ref clan::Mutex class. A mutex is an object that can be locked by only one thread at the same time. If any other thread tries to lock the mutex, it will block until the first thread releases its lock. 

\code
class MyClass
{
public:
	clan::Mutex mutex;
	clan::String variable;
 
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
clan::Thread thread1, thread2;
thread1.start(&my_class, &MyClass::thread_main1);
thread2.start(&my_class, &MyClass::thread_main2);
thread1.join();
thread2.join();
\endcode

It is vital to a program's execution that a mutex lock is always freed - otherwise you would eventually get a deadlock of all threads working with the data guarded by the mutex. If an exception was thrown in the above thread main functions, the mutex would not get unlocked. This can be solved by inserting a try / catch clause, or it could be changed to use clan::MutexSection to lock the mutex instead. clan::MutexSection locks a mutex in its constructor and releases it in its destructor, which ensures that all exit paths of a function will eventually unlock a mutex: 

\code
void thread_main1()
{
	for (int i = 0; i < 10000; i++)
	{
		clan::MutexSection mutex_lock(&mutex);
		variable = "Modified from thread 1";
	}
}
\endcode

<h2>Thread Local Storage</h2>

Thread local storage, or TLS for short, is the mechanism by which each thread in a given multithreaded process allocates storage for thread-specific data. This can usually be done using compiler specific keywords, i.e. __declspec(thread) int tls_i = 1; for Visual C++. However, these usually imply some limitations that make it unsuitable for storing class variables in the TLS. 

\ref clan::ThreadLocalStorage provides a way to store std::shared_ptr memory managed objects in the TLS. The function \ref clan::ThreadLocalStorage::get_variable returns a different variable for every thread. 

A simple example: 

\code
class MyClass
{
public:
	clan::String str;
	int i;
};
 
void thread_main()
{
	// Retrieve TLS variable:
	std::shared_ptr tls_var(
		clan::ThreadLocalStorage::get_variable("my_var"));
 
	if (tls_var.is_null())
	{
		// First time this thread tries to get this object.
		// Create object and store it.
		tls_var = clan::SharedPtr(new MyClass);
		clan::ThreadLocalStorage::set_variable("my_var", tls_var);
	}
 
	tls_var->str = "Hello";
	tls_var->i = 42;
}
 
clan::Thread thread1, thread2;
thread1.start(&thread_main);
thread2.start(&thread_main);
thread1.join();
thread2.join();
\endcode

If the thread has not been created by \ref clan::Thread, the application must create a \ref clan::ThreadLocalStorage instance in the thread before using the TLS functions. The TLS variables will be destroyed when the \ref clan::ThreadLocalStorage object is destroyed. For \ref clan::Thread objects, this happens when the thread main function finishes. 



*/
