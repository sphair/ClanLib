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
#include "property_manager.h"

namespace clan
{

PropertyManager::PropertyManager()
: listview(0)
{
	set_title("Property Manager");
	set_dockable_window_geometry(Rect(Point(50, 50), Size(300, 400)));

	listview = new ListView(this);
	listview->set_class("borderless", true);
	listview->get_header()->set_class("hidden", true);
	listview->get_header()->append(listview->get_header()->create_column("name", "Name")).set_width(110);
}

void PropertyManager::on_resized()
{
	listview->set_geometry(get_size());
}

}
