#include <ClanLib/core.h>
#include <ClanLib/application.h>
 
class MyClass
{
public:
	MyClass();

	CL_Signal_v2<int, int> my_signal;
	CL_Signal_v3<char*, int, int> my_signal2;

	CL_Slot my_slot;
	CL_Slot my_slot2;

	void on_signal(int, int);
	static void on_signal2(char *, int, int);
};

MyClass::MyClass()
{
	// Example of connecting a slot to a member function:
	my_slot = my_signal.connect(this, &MyClass::on_signal);

	// Example of connecting a static function to a signal:
	my_slot2 = my_signal2.connect(&MyClass::on_signal2);
}

void MyClass::on_signal(int a, int b)
{                                                                               
	std::cout << "on_signal: " << a << ", " << b << std::endl;
}

void MyClass::on_signal2(char *a, int b, int c)
{
	std::cout << "on_signal2: " << a << ", " << b << ", " << c << std::endl;
}

class SignalExample : public CL_ClanApplication
{
	virtual int main(int, char **)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		MyClass my_class;

		// Examples of emitting a signal:
		my_class.my_signal(1, 2);
		my_class.my_signal2("Hello World", 3, 4);

		// Display console close message and wait for a key
		console.display_close_message();

		return 0;
	}
} app;
