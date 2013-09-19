
/*! \page TemplateCallbacks Template Callbacks

MainDocs:Template Callbacks

From ClanLib Game SDK
 

ClanLib includes a complete signalling library -- a signal library is a C++ template library that allow you to use type-safe C++ templates to setup function callbacks. 

The library currently supports three different types of callback templates: 
• Signals and slots 
• Virtual functions 
• Callbacks 

Signals and Slots

A signal is an object that, when emitted, invokes one or more slot functions. This setup allows a one-way messaging system that informs a number of functions whenever a certain event occours. A simple example with one signal and two slot functions: 


void slot_function1(int p1, int p2)
{
	CL_Console::write_line("Slot 1: %1,%2", p1, p2);
}
 
void slot_function2(int p1, int p2)
{
	CL_Console::write_line("Slot 2: %1,%2", p1, p2);
}
 
// Create signal and hook up slots:
CL_Signal_v2<int, int> signal;
CL_Slot slot1 = signal.connect(&slot_function1);
CL_Slot slot2 = signal.connect(&slot_function2);
 
// Emit signal:
signal.invoke(21, 42);

The 'v2' part of CL_Signal_v2 indicates that the slot functions returns void and take 2 parameters. The types of the parameters are then defined as int, int. When the CL_Slot handle object returned by CL_Signal_vX::connect is destroyed, the slot function is disconnected from the signal. If you plan to connect a lot of slots to signals, the CL_SlotContainer class may come in handy: 


void slot_function1(int p1, int p2, CL_String p3);
void slot_function2(int p1, int p2, CL_String p3);
 
CL_SlotContainer slots;
CL_Signal_v3<int, int, CL_String> signal;
slots.connect(signal, &slot_function1);
slots.connect(signal, &slot_function2);
signal.invoke(21, 42, "text");

Just like with the CL_Thread::start and cl_format functions, the slot callback function can be placed in a class and have additional fixed parameters passed along: 


class MyClass
{
public:
	void slot_func(int p1, int p2, int user1);
	CL_SlotContainer slots;
};
 
CL_Signal_v2<int, int> signal;
 
MyClass my_class;
my_class.slots.connect(signal, &my_class, &MyClass:slot_func, 100);
my_class.slots.connect(signal, &my_class, &MyClass:slot_func, 200);
 
signal.invoke(21, 42);

Virtual Functions

The CL_VirtualFunction_X template classes offers a different type of signal object, where each function connected replaces the previous object: 


int slot_function1(int p1, int p2, CL_Super_2<int, int, int> &super)
{
	return p1 + p2;
}
 
int slot_function2(int p1, int p2, CL_Super_2<int, int, int> &super)
{
	if (super.is_invokable())
		return -super.invoke(p1, p2);
	else
		return 0;
}
 
// Create signal and hook up slots:
CL_VirtualFunction_2<int, int, int> signal;
CL_Slot slot1 = signal.connect(&slot_function1);
CL_Slot slot2 = signal.connect(&slot_function2);
 
// Emit signal:
int result = signal.invoke(21, 42);
// result becomes -(p1 + p2)

In this example the CL_VirtualFunction::invoke function calls slot_function2, which then calls slot_function1 - just like a virtual function in a derived class could do. 

Callbacks

The last type of callback template classes available is CL_Callback_X. It simply calls one callback function when invoked, just like a standard C style function pointer would do. 


int callback_function(int p1, int p2)
{
	return p1 + p2;
}
 
CL_Callback_2<int, int, int> callback;
callback.set(&callback_function);
 
int result = callback.invoke(21, 42);

Callbacks are used in many ClanLib classes, like the func_expired callback in CL_Timer: 

CL_Callback_v0 &func_expired(); 


class MyClass
{
    CL_Timer timer;
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


*/
