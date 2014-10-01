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
	desc.set_size(clan::Size(640, 480), true);
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

	Svg svg("Resources/like3.svg");

	while (!quit)
	{
		canvas.clear(clan::Colorf(0.9f, 0.9f, 0.9f));
		canvas.set_transform(clan::Mat4f::translate(190.0f, 110.0f, 0.0f) * clan::Mat4f::scale(10.0f, 10.0f, 10.0f));
		svg.render(canvas);
		canvas.set_transform(clan::Mat4f::identity());

		window.flip(1);
		clan::KeepAlive::process(0);
	}

	return 0;
}
