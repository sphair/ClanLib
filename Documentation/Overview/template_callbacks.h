
/*! \page TemplateCallbacks Signals, Callbacks and CallbackContainer

ClanLib includes a complete signalling library -- a signal library is a C++ template library that allow you to use type-safe C++ templates to setup function callbacks. 

The library currently supports two different types of callback templates: 

<ul>
<li>Callbacks </li>
<li>Signals </li>
</ul>

<h2>Callbacks</h2>

The first type of callback template classes available is clan::Callback. It simply calls one callback function when invoked, just like a standard C style function pointer would do. 

\code

int callback_function(int p1, int p2)
{
	return p1 + p2;
}
 
clan::Callback<int(int, int)> callback;
callback.set(&callback_function);
 
int result = callback.invoke(21, 42);
\endcode

Of course you can use the C++11 Lambda Expressions as well.

\code

int addTo = 3;
clan::Callback<int(int)> callback([addTo](int summand){ return summand + addTo; });
int result8 = callback.invoke(5);
int result0 = callback.invoke(-3);

\endcode

Callbacks are used in many ClanLib classes, like the func_expired callback in \ref clan::Timer :

\code
clan::Callback<void()> &func_expired(); 


class MyClass
{
    clan::Timer timer;
    ....
}
 
void MyClass::on_timer_expired()
{
    ... do something when the timer expires ...
}
 
void MyClass::initialize_timer()
{
    timer.func_expired().set(this, &MyClass::on_timer_expired);
}

\endcode

<h2>Signals</h2>

A signal is an object that, when emitted, invokes one or more slot functions. This setup allows a one-way messaging system that informs a number of functions whenever a certain event occours. A simple example with one signal functions: 

\code
void slot_function1(int p1, int p2)
{
	clan::Console::write_line("Slot 1: %1,%2", p1, p2);
}

void slot_function2(int p1, int p2)
{
	clan::Console::write_line("Slot 2: %1,%2", p1, p2);
}
 
// Create signal and hook up slots:
clan::Signal<int, int> signal;
signal.connect(Callback<void(int,int)>(&slot_function1));
signal.connect(Callback<void(int,int)>(&slot_function2));
 
// Emit signal:
signal.invoke(21, 42);
\endcode

The corresponding callback is a callback with the same parameters and void as return type.
You also can use the new C++11 initializer list to simplify the callback creation.

\code
void slot_function1(int p1, int p2)
{
        clan::Console::write_line("Slot 1: %1,%2", p1, p2);
}

// Create signal and hook up slots:
clan::Signal<int, int> signal;
signal.connect({&slot_function1});

// Emit signal:
signal.invoke(21, 42);
\endcode

Note the two curly braces around <tt>&slot_function1</tt>. 

The types of the parameters are defined as int, int.
If you want do disconnect a Callback from a signal afterwards, you have to store the callback.

\code
void slot_function1(int p1, int p2, clan::String p3);
void slot_function2(int p1, int p2, clan::String p3);

// Create signal and callback
clan::Signal<int, int, clan::String> signal;
Callback<void(int, int, clan::String)> callback1(&slot_function1);
Callback<void(int, int, clan::String)> callback2(&slot_function2);

// connect callbacks
signal.connect(callback1);
signal.connect(callback2);

// emit signal
signal.invoke(0, 5, "text");

// disconnect one callback
signal.disconnect(callback1);

// emit signal
signal.invoke(3, 3, "text2");
\endcode

If you have a lot of callbacks that needs to be released at some point, the \ref clan::CallbackContainer class may come in handy:

\code
void slot_function1(int p1, int p2, clan::String p3);
void slot_function2(int p1, int p2, clan::String p3);
 
clan::Signal<int, int, clan::String> signal;
{
    clan::CallbackContainer cc;
    cc.connect(signal, Callback<void(int, int, clan::String)>(&slot_function1));
    cc.connect(signal, {&slot_function2});
    signal.invoke(21, 42, "text");
}
signal.invoke(23, 73, "text2");

\endcode

The first signal.invoke will invoke both, slot_function1 and slot_function2, but the second signal.invoke will have no effect, because the CallbackContainer disconnects all signals and callbacks, when it goes out of scope.

Just like with the \ref clan::Thread::start and \ref clan::string_format functions, the slot callback function can be placed in a class and have additional fixed parameters passed along: 

\code

class MyClass
{
public:
	void slot_func(int p1, int p2, int user1);
	clan::SlotContainer slots;
};
 
clan::Signal<int, int> signal;
 
MyClass my_class;
signal.connect({&my_class, &MyClass::slot_func, 100});
signal.connect({std::bind(&MyClass::slot_func, &my_class, std::placeholders::_1, std::placeholders::_2, 200)});

signal.invoke(21, 42);

\endcode


*/
