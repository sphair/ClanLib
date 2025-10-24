/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "API/Display/2D/draw.h"
#include "API/Display/Window/display_window.h"

/////////////////////////////////////////////////////////////////////////////
// CL_MainWindow_Impl Class:

class CL_MainWindow_Impl
{
public:
	void on_process_message(CL_GUIMessage &msg);

	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);

	CL_MainWindow *window;

	CL_MenuBar *menubar;

	CL_StatusBar *statusbar;

	std::vector<CL_ToolBar *> toolbars;

	CL_Callback_v0 func_close;

	CL_Callback_v1<CL_Rect&> func_resize;

	CL_String title;

	CL_GUIThemePart part_component;
};

/////////////////////////////////////////////////////////////////////////////
// CL_MainWindow Construction:

CL_MainWindow::CL_MainWindow(CL_GUIManager *owner, const CL_GUITopLevelDescription &description)
: CL_GUIComponent(owner, description), impl(new CL_MainWindow_Impl)
{
	impl->window = this;
	impl->title = description.get_title();
	impl->part_component = CL_GUIThemePart(this);

	func_process_message().set(impl.get(), &CL_MainWindow_Impl::on_process_message);
	func_render().set(impl.get(), &CL_MainWindow_Impl::on_render);

	set_type_name("mainwindow");
	
	impl->menubar = new CL_MenuBar(this);
	impl->statusbar = new CL_StatusBar(this);

	int menubar_height = impl->menubar->get_preferred_height();
	int statusbar_height = impl->statusbar->get_preferred_height();
	CL_Rect g = get_geometry();
	CL_Rect menubar_rect(0, 0, g.get_width(), menubar_height);
	CL_Rect statusbar_rect(0, g.get_height()-statusbar_height, g.get_width(), g.get_height());
	impl->menubar->set_geometry(menubar_rect);
	impl->statusbar->set_geometry(statusbar_rect);
}

CL_MainWindow::CL_MainWindow(CL_GUIComponent *owner, const CL_GUITopLevelDescription &description)
: CL_GUIComponent( owner, description), impl(new CL_MainWindow_Impl)
{
	set_type_name("mainwindow");

	impl->window = this;
	impl->title = description.get_title();
	impl->part_component = CL_GUIThemePart(this);

	func_process_message().set(impl.get(), &CL_MainWindow_Impl::on_process_message);
	func_render().set(impl.get(), &CL_MainWindow_Impl::on_render);

	impl->menubar = new CL_MenuBar(this);
	impl->statusbar = new CL_StatusBar(this);

	int menubar_height = impl->menubar->get_preferred_height();
	int statusbar_height = impl->statusbar->get_preferred_height();
	CL_Rect g = get_geometry();
	CL_Rect menubar_rect(0, 0, g.get_width(), menubar_height);
	CL_Rect statusbar_rect(0, g.get_height()-statusbar_height, g.get_width(), g.get_height());
	impl->menubar->set_geometry(menubar_rect);
	impl->statusbar->set_geometry(statusbar_rect);
}

CL_MainWindow::~CL_MainWindow()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_MainWindow Events:

CL_Callback_v0 &CL_MainWindow::func_close()
{
	return impl->func_close;
}

CL_Callback_v1<CL_Rect&> &CL_MainWindow::func_resize()
{
	return impl->func_resize;
}

/////////////////////////////////////////////////////////////////////////////
// CL_MainWindow Attributes:

CL_MainWindow *CL_MainWindow::get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id)
{
	CL_MainWindow *object = NULL;
	if (reference_component)
		object = dynamic_cast<CL_MainWindow*>(reference_component->get_named_item(id));

	if (!object)
		throw CL_Exception(cl_format("Cannot find CL_MainWindow named item: %1", id));

	return object;
}

CL_StringRef CL_MainWindow::get_title() const 
{
	return impl->title;
}

CL_MenuBar *CL_MainWindow::get_menubar() const
{
	return impl->menubar;
}

CL_StatusBar *CL_MainWindow::get_statusbar() const
{
	return impl->statusbar;
}

std::vector<CL_ToolBar *> CL_MainWindow::get_toolbars() const
{
	return impl->toolbars;
}

/////////////////////////////////////////////////////////////////////////////
// CL_MainWindow Operations:

void CL_MainWindow::set_title(const CL_StringRef &str)
{
	impl->title = str;
}

/////////////////////////////////////////////////////////////////////////////
// CL_MainWindow Implementation:

void CL_MainWindow_Impl::on_process_message(CL_GUIMessage &msg)
{
	if (msg.is_type(CL_GUIMessage_Close::get_type_name()))
	{
		if (!func_close.is_null())
			func_close.invoke();
	}
}

void CL_MainWindow_Impl::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_Rect rect = window->get_geometry();
	part_component.render_box(gc, rect, update_rect);
}
