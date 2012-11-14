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

#include "GUI/precomp.h"
#include "API/Core/Text/string_format.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_message_close.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/main_window.h"
#include "API/GUI/Components/menubar.h"
#include "API/GUI/Components/statusbar.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/Window/display_window.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// MainWindow_Impl Class:

class MainWindow_Impl
{
public:
	void on_process_message(std::shared_ptr<GUIMessage> &msg);

	void on_render(Canvas &canvas, const Rect &update_rect);

	MainWindow *window;

	MenuBar *menubar;

	StatusBar *statusbar;

	std::vector<ToolBar *> toolbars;

	Callback_v0 func_close;

	Callback_v1<Rect&> func_resize;

	std::string title;
};

/////////////////////////////////////////////////////////////////////////////
// MainWindow Construction:

MainWindow::MainWindow(GUIManager *owner, const GUITopLevelDescription &description)
: GUIComponent(owner, description), impl(new MainWindow_Impl)
{
	impl->window = this;
	impl->title = description.get_title();

	func_process_message().set(impl.get(), &MainWindow_Impl::on_process_message);
	func_render().set(impl.get(), &MainWindow_Impl::on_render);

	set_tag_name("mainwindow");
	
	impl->menubar = new MenuBar(this);
	impl->statusbar = new StatusBar(this);

	//FIXME: int menubar_height = impl->menubar->get_preferred_height();
	//FIXME: int statusbar_height = impl->statusbar->get_preferred_height();
	//FIXME: Rect g = get_geometry();
	//FIXME: Rect menubar_rect(0, 0, g.get_width(), menubar_height);
	//FIXME: Rect statusbar_rect(0, g.get_height()-statusbar_height, g.get_width(), g.get_height());
	//FIXME: impl->menubar->set_geometry(menubar_rect);
	//FIXME: impl->statusbar->set_geometry(statusbar_rect);
}

MainWindow::MainWindow(GUIComponent *owner, const GUITopLevelDescription &description)
: GUIComponent( owner, description), impl(new MainWindow_Impl)
{
	set_tag_name("mainwindow");

	impl->window = this;
	impl->title = description.get_title();

	func_process_message().set(impl.get(), &MainWindow_Impl::on_process_message);
	func_render().set(impl.get(), &MainWindow_Impl::on_render);

	impl->menubar = new MenuBar(this);
	impl->statusbar = new StatusBar(this);

	//FIXME: int menubar_height = impl->menubar->get_preferred_height();
	//FIXME: int statusbar_height = impl->statusbar->get_preferred_height();
	//FIXME: Rect g = get_geometry();
	//FIXME: Rect menubar_rect(0, 0, g.get_width(), menubar_height);
	//FIXME: Rect statusbar_rect(0, g.get_height()-statusbar_height, g.get_width(), g.get_height());
	//FIXME: impl->menubar->set_geometry(menubar_rect);
	//FIXME: impl->statusbar->set_geometry(statusbar_rect);
}

MainWindow::~MainWindow()
{
}

/////////////////////////////////////////////////////////////////////////////
// MainWindow Events:

Callback_v0 &MainWindow::func_close()
{
	return impl->func_close;
}

Callback_v1<Rect&> &MainWindow::func_resize()
{
	return impl->func_resize;
}

/////////////////////////////////////////////////////////////////////////////
// MainWindow Attributes:

MainWindow *MainWindow::get_named_item(GUIComponent *reference_component, const std::string &id)
{
	MainWindow *object = NULL;
	if (reference_component)
		object = dynamic_cast<MainWindow*>(reference_component->get_named_item(id));

	if (!object)
		throw Exception(string_format("Cannot find MainWindow named item: %1", id));

	return object;
}

std::string MainWindow::get_title() const 
{
	return impl->title;
}

MenuBar *MainWindow::get_menubar() const
{
	return impl->menubar;
}

StatusBar *MainWindow::get_statusbar() const
{
	return impl->statusbar;
}

std::vector<ToolBar *> MainWindow::get_toolbars() const
{
	return impl->toolbars;
}

/////////////////////////////////////////////////////////////////////////////
// MainWindow Operations:

void MainWindow::set_title(const std::string &str)
{
	impl->title = str;
}

/////////////////////////////////////////////////////////////////////////////
// MainWindow Implementation:

void MainWindow_Impl::on_process_message(std::shared_ptr<GUIMessage> &msg)
{
	std::shared_ptr<GUIMessage_Close> close_msg = std::dynamic_pointer_cast<GUIMessage_Close>(msg);

	if (close_msg)
	{
		if (!func_close.is_null())
			func_close.invoke();
	}
}

void MainWindow_Impl::on_render(Canvas &canvas, const Rect &update_rect)
{
}

}
