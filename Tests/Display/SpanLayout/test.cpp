/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Kenneth Gangstoe
*/

#include <ClanLib/application.h>
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/swrender.h>

class TestApp
{
public:
	virtual int main(const std::vector<CL_String> &args)
	{
		CL_ConsoleWindow console("Console");

		try
		{
			CL_DisplayWindowDescription desc;
			desc.set_size(CL_Size(800,600), true);
			desc.set_title("Span Layout Test");
			CL_DisplayWindow window(desc);
			
			CL_GraphicContext &gc = window.get_gc();

			CL_FontDescription font_desc1;
			font_desc1.set_typeface_name("Verdana");
			font_desc1.set_height(-13);
			CL_Font font1(gc, font_desc1);

			CL_Image smiley(gc, "smiley.png");

			CL_SpanLayout span;
			span.add_text(" This is a ", font1, CL_Colorf::white, 1);
			span.add_text("red", font1, CL_Colorf::red, 2);
			span.add_text(" text! ", font1, CL_Colorf::white, 3);
			span.add_text("And this   complete   text   is   green with non-blocking space..", font1, CL_Colorf::green, 4);
			span.add_image(smiley, 10, 5);
			span.add_text("This is a really long descriptive and interesting text. ", font1, CL_Colorf::yellow, 6);
			span.add_text("[", font1, CL_Colorf::black, 7);
			span.add_text("15:35", font1, CL_Colorf::white, 8);
			span.add_text("]", font1, CL_Colorf::black, 9);
			span.add_image(smiley, 0, 10);
			span.add_image(smiley, 2, 11);
			span.add_text("kthxbye!", font1, CL_Colorf::blue, 12);

			span.layout(gc, 200);
			span.set_position(CL_Point(10, 10));

			while (!window.get_ic().get_keyboard().get_keycode(CL_KEY_ESCAPE))
			{
				gc.clear(CL_Colorf::gray70);

				span.draw_layout(gc);

				CL_Point mouse_pos = window.get_ic().get_mouse().get_position();
				CL_SpanLayout::HitTestResult result = span.hit_test(gc, mouse_pos);

				CL_String type;
				switch(result.type)
				{
					case CL_SpanLayout::HitTestResult::no_objects_available:
						type = "no_objects_available";
						break;
					case CL_SpanLayout::HitTestResult::outside_top:
						type = "outside_top";
						break;
					case CL_SpanLayout::HitTestResult::outside_left:
						type = "outside_left";
						break;
					case CL_SpanLayout::HitTestResult::outside_right:
						type = "outside_right";
						break;
					case CL_SpanLayout::HitTestResult::outside_bottom:
						type = "outside_bottom";
						break;
					case CL_SpanLayout::HitTestResult::inside:
						type = "inside";
						break;
				}
				CL_String result_text = cl_format("HitTestResult: Type:%1 ID:%2 Offset:%3", type, result.object_id, result.offset);

				font1.draw_text(gc, 10, 300, result_text);

				window.flip();
				CL_KeepAlive::process();
				CL_System::sleep(50);
			}
		}
		catch(CL_Exception error)
		{
			CL_Console::write_line("Exception caught:");
			CL_Console::write_line(error.message);

			console.display_close_message();
			return -1;
		}

		return 0;
	}
};

class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		CL_SetupCore setup_core;
		CL_SetupDisplay setup_display;
		CL_SetupSWRender setup_swrender;

		TestApp app;
		return app.main(args);
	}
};

CL_ClanApplication app(&Program::main);
