/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
#include <ClanLib/d3d.h>
using namespace clan;

class TestApp
{
public:
	virtual int main(const std::vector<std::string> &args)
	{

		DisplayWindowDescription desc;
		desc.set_size(Size(800,600), true);
		desc.set_title("Span Layout Test");
		DisplayWindow window(desc);

		Canvas canvas(window);
		GraphicContext gc = window.get_gc();

		FontDescription font_desc1;
		font_desc1.set_height(13);
		Font font1("Verdana", font_desc1);

		Image smiley(canvas, "smiley.png");

		SpanLayout span;
		span.add_text(" This is a ", font1, Colorf::white, 1);
		span.add_text("red", font1, Colorf::red, 2);
		span.add_text(" text! ", font1, Colorf::white, 3);
		span.add_text("And this   complete   text   is   green with non-blocking space..", font1, Colorf::green, 4);
		span.add_image(smiley, 10, 5);
		span.add_text("This is a really long descriptive and interesting text. ", font1, Colorf::yellow, 6);
		span.add_text("[", font1, Colorf::black, 7);
		span.add_text("15:35", font1, Colorf::white, 8);
		span.add_text("]", font1, Colorf::black, 9);
		span.add_image(smiley, 0, 10);
		span.add_image(smiley, 2, 11);
		span.add_text("kthxbye!", font1, Colorf::blue, 12);

		span.layout(canvas, 200);
		span.set_position(Point(10, 10));

		while (!window.get_ic().get_keyboard().get_keycode(keycode_escape))
		{
			gc.clear(Colorf::gray70);

			span.draw_layout(canvas);

			Point mouse_pos = window.get_ic().get_mouse().get_position();
			SpanLayout::HitTestResult result = span.hit_test(canvas, mouse_pos);

			std::string type;
			switch(result.type)
			{
				case SpanLayout::HitTestResult::no_objects_available:
					type = "no_objects_available";
					break;
				case SpanLayout::HitTestResult::outside_top:
					type = "outside_top";
					break;
				case SpanLayout::HitTestResult::outside_left:
					type = "outside_left";
					break;
				case SpanLayout::HitTestResult::outside_right:
					type = "outside_right";
					break;
				case SpanLayout::HitTestResult::outside_bottom:
					type = "outside_bottom";
					break;
				case SpanLayout::HitTestResult::inside:
					type = "inside";
					break;
			}
			std::string result_text = string_format("HitTestResult: Type:%1 ID:%2 Offset:%3", type, result.object_id, result.offset);

			font1.draw_text(canvas, 10, 300, result_text);

			window.flip();
			RunLoop::process();
			System::sleep(50);
		}

		return 0;
	}
};

class Program
{
public:
	static int main(const std::vector<std::string> &args)
	{
		SetupCore setup_core;
		SetupDisplay setup_display;
		SetupD3D setup_d3d;

		TestApp app;
		return app.main(args);
	}
};

Application app(&Program::main);
