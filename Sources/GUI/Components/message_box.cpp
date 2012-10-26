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
**    Harry Storbacka
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/Components/message_box.h"
#include "API/Display/screen_info.h"
#include "message_box_component.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// message_box function:

template<typename T>
MessageBoxResult message_box_impl( 
	T *owner,
	std::string title, 
	std::string detail_text, 
	MessageBoxButtons buttons,
	MessageBoxIcon icon,
	const Rect owner_geometry)
{
	GUITopLevelDescription desc;
	desc.set_title(title);
	desc.set_dialog_window(true);
	desc.set_allow_resize(true);
	desc.set_position(Rect(100,100,200,200), false);
	desc.set_visible(false);

	Window window(owner, desc);
	MessageBoxComponent mb(&window, detail_text, buttons, icon);
	Size content_size = mb.layout_content();

	window.func_close().set(&mb, &MessageBoxComponent::on_window_close, &window);

	DisplayWindow disp_win = window.get_display_window();

	Point win_tl = window.get_window_geometry().get_top_left();
	Point cont_tl = disp_win.client_to_screen(Point(0,0));
	Point decorations_top_left = cont_tl - win_tl;

	Point win_br = window.get_window_geometry().get_bottom_right();
	Rect client = window.get_client_area();
	Point cont_br =  disp_win.client_to_screen(client.get_bottom_right());
	Point decorations_bottom_right = win_br - cont_br;


	Rect win_geometry;
	int wing_width = decorations_top_left.x + content_size.width + decorations_bottom_right.x;
	int wing_height = decorations_top_left.y + content_size.height + decorations_bottom_right.y;

	win_geometry.left = owner_geometry.get_center().x - wing_width/2;
	win_geometry.top = owner_geometry.get_center().y - wing_height/2;
	win_geometry.right = win_geometry.left + wing_width;
	win_geometry.bottom = win_geometry.top + wing_height;

	window.set_window_geometry(win_geometry);
	mb.set_geometry(Rect(0,0,content_size.width, content_size.height));

	window.set_visible();

	MessageBoxResult reason = MessageBoxResult(window.exec());

	return reason;
}

MessageBoxResult message_box( 
	GUIComponent *owner,
	std::string title, 
	std::string detail_text, 
	MessageBoxButtons buttons,
	MessageBoxIcon icon)
{
	Rect owner_geometry = owner->get_window_geometry();
	return message_box_impl(owner, title, detail_text, buttons, icon, owner_geometry);
}

MessageBoxResult message_box(
	GUIManager *gui_manager,
	std::string title,
	std::string detail_text,
	MessageBoxButtons buttons,
	MessageBoxIcon icon)
{
	ScreenInfo screen_info;
	int primary_screen_index = 0;
	std::vector<Rect> screens = screen_info.get_screen_geometries(primary_screen_index);
	return message_box_impl(gui_manager, title, detail_text, buttons, icon, screens[primary_screen_index]);
}

}
