
/*! \page TemplateCallbacks Template Callbacks

ClanLib includes a complete signalling library -- a signal library is a C++ template library that allow you to use type-safe C++ templates to setup function callbacks. 

The library currently supports two different types of callback templates: 

<ul>
<li>Signals and slots </li>
<li>Callbacks </li>
</ul>

<h2>Signals and Slots</h2>

A signal is an object that, when emitted, invokes one or more slot functions. This setup allows a one-way messaging system that informs a number of functions whenever a certain event occours. A simple example with one signal and two slot functions: 

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
clan::Signal_v2<int, int> signal;
clan::Slot slot1 = signal.connect(&slot_function1);
clan::Slot slot2 = signal.connect(&slot_function2);
 
// Emit signal:
signal.invoke(21, 42);
\endcode

The 'v2' part of \ref clan::Signal_v2 indicates that the slot functions returns void and take 2 parameters. The types of the parameters are then defined as int, int. When the \ref clan::Slot handle object returned by \ref clan::Signal_vX::connect is destroyed, the slot function is disconnected from the signal. If you plan to connect a lot of slots to signals, the \ref clan::SlotContainer class may come in handy: 

\code
void slot_function1(int p1, int p2, clan::String p3);
void slot_function2(int p1, int p2, clan::String p3);
 
clan::SlotContainer slots;
clan::Signal_v3<int, int, clan::String> signal;
slots.connect(signal, &slot_function1);
slots.connect(signal, &slot_function2);
signal.invoke(21, 42, "text");

\endcode

Just like with the \ref clan::Thread::start and \ref clan::string_format functions, the slot callback function can be placed in a class and have additional fixed parameters passed along: 

\code

class MyClass
{
public:
	void slot_func(int p1, int p2, int user1);
	clan::SlotContainer slots;
};
 
clan::Signal_v2<int, int> signal;
 
MyClass my_class;
my_class.slots.connect(signal, &my_class, &MyClass:slot_func, 100);
my_class.slots.connect(signal, &my_class, &MyClass:slot_func, 200);
 
signal.invoke(21, 42);

\endcode

<h2>Callbacks</h2>

The second type of callback template classes available is clan::Callback_X. It simply calls one callback function when invoked, just like a standard C style function pointer would do. 

\code

int callback_function(int p1, int p2)
{
	return p1 + p2;
}
 
clan::Callback_2<int, int, int> callback;
callback.set(&callback_function);
 
int result = callback.invoke(21, 42);
\endcode

Callbacks are used in many ClanLib classes, like the func_expired callback in \ref clan::Timer :

\code
clan::Callback_v0 &func_expired(); 


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

*/
