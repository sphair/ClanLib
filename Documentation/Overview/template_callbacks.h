
/*! \page TemplateCallbacks Signal and SlotContainer

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
clan::Signal<void(int, int)> signal;
clan::Slot slot1 = signal.connect(&slot_function1);
clan::Slot slot2 = signal.connect(&slot_function2);
clan::Slot slot3 = signal.connect([](int p1, int p2) { clan::Console::write_line("Slot 3: %1,%2", p1, p2); });
 
// Invoke signal:
signal(21, 42);
\endcode

Important: when all references to the slot returned by connect are destroyed, the slot will disconnect from the signal.

If you have a lot of slots that needs to stay connected for a longer duration, the \ref clan::SlotContainer class may come in handy:

\code
void slot_function1(int p1, int p2, clan::String p3);
void slot_function2(int p1, int p2, clan::String p3);
 
clan::Signal<int, int, clan::String> signal;

{
    clan::SlotbackContainer slots;
    slots.connect(signal, &slot_function1);
    slots.connect(signal, &slot_function2);
    signal(21, 42, "text");
}

signal(23, 73, "text2");

\endcode

The first signal invoke will invoke both, slot_function1 and slot_function2. The second signal invoke will have no effect because the SlotContainer disconnects all slots contained in it when destroyed.

*/
