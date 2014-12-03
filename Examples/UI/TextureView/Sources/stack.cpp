
#include "precomp.h"
#include "basic2d.h"

using namespace clan;

class Stack_Impl
{
public:
	Stack_Impl(const std::function<void()> &func) : callback(func){}
	~Stack_Impl(){ pop(); }
	void pop()
	{ 
		if (callback)
		{
			callback();
			callback = std::function<void()>();
		}
	}

	std::function<void()> callback;
};

class Stack
{
public:

	Stack() {}
	Stack(const std::function<void()> &func) : impl(std::make_shared<Stack_Impl>(func)){}

	void pop() { if (impl) impl->pop(); }
private:
	std::shared_ptr<Stack_Impl> impl;
};

void Basic2D::hello_world(int value)
{
	member_value = value;
}

Stack Basic2D::test(int value)
{
	Stack stack([=](){hello_world(value); });
	return stack;
}
int Basic2D::start(const std::vector<std::string> &args)
{
	member_value = 0;
	if (1)
	{
		Stack stack = test(123);
		if (member_value == 123)
			throw Exception("Failed");
	}
	if (member_value != 123)
		throw Exception("Passed");


	return 0;
}

