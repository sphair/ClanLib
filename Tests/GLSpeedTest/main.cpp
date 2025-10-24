#include <ClanLib/application.h>
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <GL/gl.h>
#include <GL/glu.h>

void init_test1(); void deinit_test1(); void test1();
void init_test2(); void deinit_test2(); void test2();
void init_test3(); void deinit_test3(); void test3();
void init_test4(); void deinit_test4(); void test4();
void init_test5(); void deinit_test5(); void test5();
void init_test6(); void deinit_test6(); void test6();
void init_test7(); void deinit_test7(); void test7();
void init_test8(); void deinit_test8(); void test8();
void init_test9(); void deinit_test9(); void test9();
void init_test10(); void deinit_test10(); void test10();

/////////////////////////////////////////////////////////////////////////////
// Application class

typedef void (*TestFunction)();

class Application : public CL_ClanApplication
{
public:
	int main(int argc, char** argv);
	
	void run_tests();
	void run_test(const std::string &desc, int count, TestFunction fun);
	
} app;

/////////////////////////////////////////////////////////////////////////////
// Application main entry point:

int Application::main(int argc, char** argv)
{
	CL_ConsoleWindow debug_console("Debug console", 80, 1000);
	debug_console.redirect_stdio();
	try
	{
		CL_SetupCore::init();
		CL_SetupDisplay::init();
		CL_SetupGL::init();

		CL_DisplayWindow window("GLSpeedTest", 800, 600, false, true);
		
		run_tests();
		
		CL_SetupGL::deinit();
		CL_SetupDisplay::deinit();
		CL_SetupCore::deinit();
	}
	catch (CL_Error error)
	{
		std::cout << "Uncaught error: " << error.message.c_str() << std::endl;
	}

	std::cout << std::endl << "Press any key to quit" << std::endl;
	debug_console.wait_for_key();
	return 0;
}

void Application::run_tests()
{
	init_test1();
	run_test("glColor4f()", 1000000, test1);
	deinit_test1();

	init_test2();
	run_test("CL_OpenGLSurface::bind()", 1000000, test2);
	deinit_test2();

	init_test3();
	run_test("glBlendFunc()", 1000000, test3);
	deinit_test3();

	init_test4();
	run_test("glDisable2d()/glEnabled2d()", 1000000, test4);
	deinit_test4();

	init_test5();
	run_test("Rect - GL_POLYGON - glVertex2f", 1000000, test5);
	deinit_test5();

	init_test6();
	run_test("Rect - glRect", 1000000, test6);
	deinit_test6();

	init_test7();
	run_test("TexCoord - glMultiTexCoord2fARB", 100000, test7);
	deinit_test7();

	init_test8();
	run_test("TexCoord - glTexCoord2f", 100000, test8);
	deinit_test8();

	init_test9();
	run_test("Binding - glBind", 10000000, test9);
	deinit_test9();

	init_test10();
	run_test("Binding - glActiveTextureARB", 10000000, test10);
	deinit_test10();
}

void Application::run_test(const std::string &desc, int count, TestFunction func)
{
	std::cout << desc << ": Iterations " << count;
	
	int step = count / 10;

	unsigned int start_time = CL_System::get_time();
	for(int i=0; i < count; i++)
	{
		func();
		if(i % step == 0)
			std::cout << ".";
	}
	unsigned int end_time = CL_System::get_time();

	std::cout << std::endl;
	std::cout << "- Total time used: " << end_time - start_time;
	std::cout << "  Average iteration time: " << (float)(end_time - start_time) / count << std::endl;;
}

