/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
**    Magnus Norddahl
**    Mark Page
*/

#include "precomp.h"
#include "svg_viewer.h"
#include "svg.h"

int SvgViewer::run(const std::vector<std::string> &args)
{
	bool quit = false;
    clan::SlotContainer sc;

	clan::DisplayWindowDescription desc;
	desc.set_title("ClanLib SVG Viewer Example");
	desc.set_size(clan::Size(1000, 800), true);
	desc.set_allow_resize(true);

	clan::DisplayWindow window(desc);
	clan::Canvas canvas(window);

	sc.connect(window.sig_window_close(), [&](){quit = true; });
	sc.connect(window.get_ic().get_keyboard().sig_key_up(), [&](const clan::InputEvent &key)
	{
		if (key.id == clan::keycode_escape)
		{
			quit = true;
		}
	});

	clan::Font font(canvas, "Tahoma", 24);

	Svg svg("Resources/tiger.svg");
	float angle = 0.0f;
	float scale = 1.2f;
	sc.connect(window.get_ic().get_mouse().sig_key_up(), [&](const clan::InputEvent &key)
	{
		if (key.id == clan::mouse_wheel_up)
			scale += 0.1f;
		if (key.id == clan::mouse_wheel_down)
			scale -= 0.1f;
	});

	clan::GameTime time;

	while (!quit)
	{
		canvas.clear(clan::Colorf(0.9f, 0.9f, 0.9f));

		if (window.get_ic().get_mouse().get_keycode(clan::mouse_left))
		{
			angle += time.get_time_elapsed() * 64.0f;
			if (angle >= 360.0f) angle = -360.0f;
		}
		if (window.get_ic().get_mouse().get_keycode(clan::mouse_right))
		{
			angle -= time.get_time_elapsed() * 64.0f;
			if (angle < 0.0f) angle += 360.0f;
		}

		clan::Mat4f rotation = clan::Mat4f::translate(128.0f, 128.0f, 0.0f) * clan::Mat4f::rotate(clan::Angle(angle, clan::angle_degrees), 0.0f, 0.0f, 1.0f) * clan::Mat4f::translate(-128.0f, -128.0f, 0.0f);

		canvas.set_transform(clan::Mat4f::translate(380.0f, 275.0f, 0.0f) * clan::Mat4f::scale(scale, scale, scale) * rotation);
		svg.render(canvas);
		canvas.set_transform(clan::Mat4f::identity());
		font.draw_text(canvas, 17, 40, clan::string_format("%1 FPS", time.get_updates_per_second()), clan::Colorf::black);

		window.flip(0);
		clan::KeepAlive::process(0);
		time.update();
	}

	return 0;
}
