/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
#include "flextable.h"

using namespace clan;

clan::ApplicationInstance<FlexTable> clanapp;

FlexTable::FlexTable()
{
#if defined(WIN32) && !defined(__MINGW32__)
	clan::D3DTarget::set_current();
#else
	clan::OpenGLTarget::set_current();
#endif

	// Create a source for our resources
	FileResourceDocument doc(FileSystem("../../ThemeAero"));
	ResourceManager resources = FileResourceManager::create(doc);

	// Mark this thread as the UI thread
	ui_thread = UIThread(resources);

	// Create a window:
	DisplayWindowDescription desc;
	desc.set_title("UICore: Flex Table");
	desc.set_allow_resize(true);
	desc.set_size(Sizef(1400, 800), false);
	window = std::make_shared<TopLevelWindow>(desc);

	// Exit run loop when close is clicked or ESC pressed.
	auto pRootView = window->root_view();
	pRootView->slots.connect(pRootView->sig_close(), [&](CloseEvent &e) { RunLoop::exit(); });
	pRootView->slots.connect(pRootView->sig_key_press(), [&](clan::KeyEvent &e) 
		{ if (e.key() == clan::Key::escape) RunLoop::exit(); }
	);

	// Need for receive a keyboard events.
	pRootView->set_focus();
	window->root_view()->style()->set("background-color: white;");

	// Main window icons
	window->display_window().set_small_icon(clan::PixelBuffer("Resources/app_icon_16x16.png", doc.get_file_system()));
	window->display_window().set_large_icon(clan::PixelBuffer("Resources/app_icon_32x32.png", doc.get_file_system()));

	auto outer_view = window->root_view()->add_child<clan::View>();
	outer_view->style()->set("border: 20px solid blue; padding: 15px; margin: 0px; background-color: black; width: 1000px; height: 500px;");

	auto column_flex_view = outer_view->add_child<clan::View>();
	column_flex_view->style()->set("border: 8px solid yellow; padding: 15px; margin: 0px; display:flex; align-items:flex-start; flex-direction:column; height:400px; background-color: #666666;");

	std::string row_flex_style("border: 8px solid red; padding: 15px; margin: 0px; width:100%; box-sizing: border-box;	display:flex; align-items:flex-start; background-color: #444444;");
	std::string row_view_style("color: white; height:60px; border: 8px solid white;	background: black;");

	for (int cnt = 0; cnt < 2; cnt++)
	{
		auto row_flex_view = column_flex_view->add_child<clan::View>();
		row_flex_view->style()->set(row_flex_style);

		auto row_view1 = row_flex_view->add_child<clan::View>();
		row_view1->style()->set(row_view_style);
		row_view1->style()->set("flex: 0 0 400px;");

		auto row_view2 = row_flex_view->add_child<clan::View>();
		row_view2->style()->set(row_view_style);
		row_view2->style()->set("flex: 1 0 auto;");

		auto row_view3 = row_flex_view->add_child<clan::View>();
		row_view3->style()->set(row_view_style);
		row_view3->style()->set("flex: 0 0 200px;");
	}

	// Prevent close program when hint or modal windows closes.
	window_manager.set_exit_on_last_close(false);
	
	// Make our window visible
	window->show();
}

bool FlexTable::update()
{
	window->display_window().flip();

	return true;
}