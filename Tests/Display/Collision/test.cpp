#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

class App
{
public:
	int start(const std::vector<CL_String> &args);

private:
	void on_input_up(const CL_InputEvent &key, const CL_InputState &state);
	void on_window_close();

private:
	bool quit;
};

class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		CL_SetupCore setup_core;
		CL_SetupDisplay setup_display;
		CL_SetupGL setup_gl;

		App app;
		return app.start(args);
	}
};

CL_ClanApplication app(&Program::main);

int App::start(const std::vector<CL_String> &args)
{
	quit = false;

	CL_ConsoleWindow console("Console", 80, 200);

	try
	{
		CL_DisplayWindowDescription desc;
		desc.set_title(cl_text("ClanLib Collision Test"));
		desc.set_size(CL_Size(800, 600), true);
		CL_DisplayWindow window(desc);

		CL_Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);
		CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

		CL_GraphicContext gc = window.get_gc();

		CL_CollisionOutline collision;

		CL_Contour contour1;
		contour1.get_points().push_back(CL_Pointf(0,0));
		contour1.get_points().push_back(CL_Pointf(100,0));
		contour1.get_points().push_back(CL_Pointf(100,100));
		contour1.get_points().push_back(CL_Pointf(0,100));
		collision.get_contours().push_back(contour1);

		CL_Contour contour2;
		contour2.get_points().push_back(CL_Pointf(432,320));
		contour2.get_points().push_back(CL_Pointf(543,542));
		contour2.get_points().push_back(CL_Pointf(154,432));
		contour2.get_points().push_back(CL_Pointf(230,234));
		collision.get_contours().push_back(contour2);

		while (!quit)
		{
			gc.clear();

			collision.draw(0, 0, CL_Colorf::red, gc);

			contour2.get_points()[0].x += 0.1f;

			window.flip(1);

			CL_DisplayMessageQueue::process();
		}
	}
	catch(CL_Exception& exception)
	{
		CL_Console::write_line("Exception caught:");
		CL_Console::write_line(exception.message);

		// Display the stack trace (if available)
		std::vector<CL_String> stacktrace = exception.get_stack_trace();
		int size = stacktrace.size();
		if (size > 0)
		{
			CL_Console::write_line("Stack Trace:");
			for (int cnt=0; cnt < size; cnt++)
			{
				CL_Console::write_line(stacktrace[cnt]);
			}
		}

		console.display_close_message();

		return -1;
	}
	return 0;
}

void App::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
	{
		quit = true;
	}
}

void App::on_window_close()
{
	quit = true;
}
