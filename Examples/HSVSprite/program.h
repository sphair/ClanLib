
#pragma once

class Program
{
public:
	static int main(const std::vector<CL_String> &args);

	Program();
	int run();

private:
	void on_close();

	bool quit;
};
