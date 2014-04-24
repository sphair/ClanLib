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
**    Harry Storbacka
**    Kenneth Gangstoe
*/

#include "GUI/precomp.h"
#include "API/CSSLayout/CSSDocument/css_document.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/IOData/file_system.h"
#include "API/GUI/accelerator_table.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_window_manager_texture.h"
#include "API/GUI/gui_window_manager_system.h"
#include "API/Display/Window/display_window_description.h"
#include "API/Display/Window/input_event.h"
#include "API/Core/System/keep_alive.h"
#include "API/Core/System/event.h"
#include "API/Core/Resources/xml_resource_document.h"
#include "API/Core/Resources/xml_resource_manager.h"
#include "API/Core/IOData/file_help.h"
#include "gui_manager_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GUIManager Construction:

GUIManager::GUIManager()
: impl(new GUIManager_Impl)
{
	GUIWindowManagerSystem window_manager;
	set_window_manager(window_manager);
}

GUIManager::GUIManager(const DisplayWindow &display_window)
	: impl(new GUIManager_Impl)
{
	GUIWindowManagerTexture window_manager(display_window);
	set_window_manager(window_manager);
}

GUIManager::GUIManager(const std::string &path_to_css_and_resources)
	: impl(new GUIManager_Impl)
{
	GUIWindowManagerSystem window_manager;
	initialize(window_manager, path_to_css_and_resources);
}

GUIManager::GUIManager(const DisplayWindow &display_window, const std::string &path_to_css_and_resources)
: impl(new GUIManager_Impl)
{
	GUIWindowManagerTexture window_manager(display_window);
	initialize(window_manager, path_to_css_and_resources);
}

GUIManager::GUIManager(GUIWindowManager &window_manager, const std::string &path_to_css_and_resources)
: impl(new GUIManager_Impl)
{
	initialize(window_manager, path_to_css_and_resources);
}

GUIManager::GUIManager(std::shared_ptr<GUIManager_Impl> impl)
	: impl(impl)
{
}

GUIManager::~GUIManager()
{
}

/////////////////////////////////////////////////////////////////////////////
// GUIManager Attributes:

ResourceManager GUIManager::get_resource_manager() const
{
	return impl->resources;
}

CSSDocument GUIManager::get_css_document() const
{
	return impl->css_document;
}

GUIWindowManager GUIManager::get_window_manager() const
{
	return impl->window_manager;
}

GUIComponent *GUIManager::get_capture_component() const
{
	return impl->mouse_capture_component;
}

bool GUIManager::get_exit_flag() const
{
	return impl->exit_flag;
}

int GUIManager::get_exit_code() const
{
	return impl->exit_code;
}

GUIComponent * GUIManager::get_focused_component()
{
	return impl->get_focus_component();
}

std::string GUIManager::get_clipboard_text() const
{
	if (impl->root_components.empty())
		return std::string();

	GUITopLevelWindow *cur = impl->root_components[0];
	return impl->window_manager.get_display_window(cur).get_clipboard_text();
}

/////////////////////////////////////////////////////////////////////////////
// GUIManager Events:

Signal<std::shared_ptr<GUIMessage> &> &GUIManager::sig_filter_message()
{
	return impl->sig_filter_message;
}

Callback<int()> &GUIManager::func_exec_handler()
{
	return impl->func_exec_handler;
}

/////////////////////////////////////////////////////////////////////////////
// GUIManager Operations:

void GUIManager::add_theme(const std::string &fullname)
{
	std::string path = PathHelp::get_fullpath(fullname, PathHelp::path_type_file);
	std::string filename = PathHelp::get_filename(fullname, PathHelp::path_type_file);
	FileSystem vfs(path);

	impl->css_document.add_sheet(author_sheet_origin, filename, vfs);
}

void GUIManager::add_theme(const std::string &fullname, const FileSystem &fs)
{
	impl->css_document.add_sheet(author_sheet_origin, fullname, fs);
}

void GUIManager::set_theme(CSSDocument css)
{
	impl->css_document = css;
}

void GUIManager::set_theme(const std::string &fullname)
{
	CSSDocument css;
	css.add_sheet(author_sheet_origin, fullname);
	set_theme(css);
}

void GUIManager::set_theme(const std::string &filename, const FileSystem &fs)
{
	CSSDocument css;
	css.add_sheet(author_sheet_origin, filename, fs);
	set_theme(css);
}

void GUIManager::set_resource_manager(ResourceManager &resource_manager)
{
	impl->resources = resource_manager;
}

void GUIManager::add_resources(const XMLResourceDocument &additional_resources)
{
	XMLResourceManager::get_doc(impl->resources).add_resources(additional_resources);
}

void GUIManager::add_resources(const std::string &filename)
{
	XMLResourceManager::get_doc(impl->resources).add_resources(XMLResourceDocument(filename));
}

void GUIManager::add_resources(const std::string &filename, const FileSystem &fs)
{
	XMLResourceManager::get_doc(impl->resources).add_resources(XMLResourceDocument(filename, fs));
}

void GUIManager::set_window_manager(GUIWindowManager &window_manager)
{
	impl->window_manager = window_manager;
	window_manager.set_site(&impl->wm_site);
}

void GUIManager::set_capture_component(GUIComponent *component, bool state)
{
	if (state)
	{
		impl->mouse_capture_component = component;
		impl->window_manager.capture_mouse(impl->get_toplevel_window(component), true);
	}
	else
	{
		if (impl->mouse_capture_component == component)
		{
			impl->window_manager.capture_mouse(impl->get_toplevel_window(component), false);
			impl->mouse_capture_component = NULL;
		}
	}
}

void GUIManager::set_tablet_proximity_component(GUIComponent *comp, bool state)
{
	GUITopLevelWindow *win = impl->get_toplevel_window(comp);

	if (state)
	{
		win->proximity_component = comp;
	}
	else
	{
		if (win->proximity_component == comp)
		{
			win->proximity_component = 0;
		}
	}
}
void GUIManager::process_messages(int timeout)
{
	KeepAlive::process(timeout);
	impl->invalidate_constant_repaint_components();
	impl->window_manager.update();
}

int GUIManager::exec()
{
	if (!impl->func_exec_handler.is_null())
		return impl->func_exec_handler.invoke();

	while (!impl->exit_flag)
	{
		impl->exit_code = 0;

		if (!impl->func_exec_handler.is_null())
			impl->func_exec_handler.invoke();

		int timeout = -1;
		if (impl->is_constant_repaint_enabled())
		{
			timeout = 0;
		}

		process_messages(timeout);

	}
	impl->exit_flag = false;
	return impl->exit_code;
}

void GUIManager::exit_with_code(int exit_code)
{
	impl->exit_flag = true;
	impl->exit_code = exit_code;
}

void GUIManager::clear_exit_flag()
{
	impl->exit_flag = false;
}

void GUIManager::dispatch_message(std::shared_ptr<GUIMessage> message)
{
	impl->dispatch_message(message);
}

void GUIManager::request_repaint(const Rect &rect, GUIComponent *root_component)
{
	std::vector<GUITopLevelWindow>::size_type pos, size;
	size = impl->root_components.size();

	for (pos = 0; pos < size; pos++)
	{
		GUITopLevelWindow *cur = impl->root_components[pos];

		if (cur->component == root_component)
		{
			impl->window_manager.request_repaint(cur, rect);
			break;
		}
	}
}

void GUIManager::render_windows()
{
	std::vector<GUITopLevelWindow>::size_type pos, size;
	size = impl->root_components.size();

	for (pos = 0; pos < size; pos++)
	{
		GUITopLevelWindow *cur = impl->root_components[pos];
		Rect geometry = cur->component->get_geometry();
		cur->component->paint(Rect(Point(0,0), geometry.get_size()));
	}
}

void GUIManager::set_clipboard_text(const std::string &str)
{
	if (impl->root_components.empty())
		return;

	GUITopLevelWindow *cur = impl->root_components[0];
	impl->window_manager.get_display_window(cur).set_clipboard_text(str);
}

void GUIManager::set_accelerator_table( const AcceleratorTable &table )
{
	impl->accel_table = table;
}

/////////////////////////////////////////////////////////////////////////////
// GUIManager Implementation:

void GUIManager::initialize(GUIWindowManager &window_manager, const std::string &path_to_css_and_resources)
{
	FileSystem vfs(path_to_css_and_resources);

	impl->resources = XMLResourceManager::create(XMLResourceDocument("resources.xml", vfs));

	set_window_manager(window_manager);
	set_theme("theme.css", vfs);
}

}
