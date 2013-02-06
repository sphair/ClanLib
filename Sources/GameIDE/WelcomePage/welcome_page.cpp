/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
*/
#include "GameIDE/precomp.h"
#include "welcome_page.h"

namespace clan
{

WelcomePage::WelcomePage()
: welcome(0)
{
	set_title("Welcome");
	set_dockable_window_geometry(Rect(Point(50, 50), Size(300, 400)));

	//welcome = new Label(this);
	logo = new ImageView(this);
	logo->set_scale_to_fit(false);

	//welcome->set_text("Start a new project to get started.");

	ResourceManager resources = get_resources();
	logo->set_sprite(Sprite(get_canvas(), "GameIdeLogo", &resources));
}

void WelcomePage::on_resized()
{
	Rect client_box = get_size();
	client_box.shrink(5);
	//welcome->set_geometry(client_box);
	logo->set_geometry(client_box);
}

}

