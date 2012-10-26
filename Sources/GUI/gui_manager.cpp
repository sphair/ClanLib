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
**    Harry Storbacka
**    Kenneth Gangstoe
*/

#include "GUI/precomp.h"
#include "API/CSSLayout/css_document.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/GUI/accelerator_table.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_theme_default.h"
#include "API/GUI/gui_window_manager_texture.h"
#include "API/GUI/gui_window_manager_system.h"
#include "API/Display/Window/display_window_description.h"
#include "API/Display/Window/input_event.h"
#include "API/Core/System/keep_alive.h"
#include "API/Core/System/event.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Core/IOData/file_help.h"
#include "API/CSSLayout/css_layout.h"
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

	GUIThemeDefault theme;
	set_theme(theme);
}

GUIManager::GUIManager(const DisplayWindow &display_window)
	: impl(new GUIManager_Impl)
{
	GUIWindowManagerTexture window_manager(display_window);
	set_window_manager(window_manager);

	GUIThemeDefault theme;
	set_theme(theme);
}

GUIManager::GUIManager(const std::string &path_to_theme)
	: impl(new GUIManager_Impl)
{
	GUIWindowManagerSystem window_manager;
	initialize(window_manager, path_to_theme);
}

GUIManager::GUIManager(const DisplayWindow &display_window, const std::string &path_to_theme)
: impl(new GUIManager_Impl)
{
	GUIWindowManagerTexture window_manager(display_window);
	initialize(window_manager, path_to_theme);
}

GUIManager::GUIManager(GUIWindowManager &window_manager, const std::string &path_to_theme)
: impl(new GUIManager_Impl)
{
	initialize(window_manager, path_to_theme);
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

CSSDocument GUIManager::get_css_document() const
{
	return impl->css_document;
}

GUITheme GUIManager::get_theme() const
{
	return impl->theme;
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

Font GUIManager::get_registered_font(const FontDescription &desc)
{
	return impl->get_registered_font(desc);
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

Signal_v1<GUIMessage &> &GUIManager::sig_filter_message()
{
	return impl->sig_filter_message;
}

Callback_0<int> &GUIManager::func_exec_handler()
{
	return impl->func_exec_handler;
}

/////////////////////////////////////////////////////////////////////////////
// GUIManager Operations:

void GUIManager::add_theme(const std::string &path_to_theme)
{
	VirtualFileSystem vfs(path_to_theme);
	VirtualDirectory dir = vfs.get_root_directory();

	ResourceManager resources("resources.xml", dir);
	impl->theme.add_resources(resources);

	impl->css_document.load("theme.css", dir);
	impl->reset_properties(); // Clear the properties cache
}

void GUIManager::set_theme(GUITheme &theme)
{
	impl->theme = theme;
}

void GUIManager::set_css_document(CSSDocument css)
{
	impl->css_document = css;
	impl->reset_properties(); // Clear the properties cache
}

void GUIManager::set_css_document(const std::string &fullname)
{
	CSSDocument css;
	css.load(fullname); 
	set_css_document(css);
}

void GUIManager::set_css_document(const std::string &filename, const VirtualDirectory &directory)
{
	CSSDocument css;
	css.load(filename, directory); 
	set_css_document(css);
}

void GUIManager::add_resources(const ResourceManager &resources)
{
	get_theme().get_resources().add_resources(resources);
}

void GUIManager::add_resources(const std::string &filename)
{
	get_theme().get_resources().add_resources(ResourceManager(filename));
}

void GUIManager::add_resources(const std::string &filename, const VirtualDirectory &directory)
{
	get_theme().get_resources().add_resources(ResourceManager(filename, directory));
}

void GUIManager::initialize_layout_manager(const std::string &xml_fullname, const std::string &css_fullname)
{
	impl->layout_manager = GUIThemeLayoutManager(xml_fullname, css_fullname);
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

void GUIManager::dispatch_message(GUIMessage message)
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

void GUIManager::register_font(const Font &font, const FontDescription &desc)
{
	impl->register_font(font, desc);
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

CSSLayout GUIManager::create_layout(GUIComponent *component)
{
	return impl->layout_manager.create_layout(component->get_type_name());
}

bool GUIManager::has_layout(GUIComponent *component)
{
	return impl->layout_manager.has_layout(component->get_type_name());
}

/////////////////////////////////////////////////////////////////////////////
// GUIManager Implementation:

void GUIManager::initialize(GUIWindowManager &window_manager, const std::string &path_to_theme)
{
	VirtualFileSystem vfs(path_to_theme);
	VirtualDirectory dir = vfs.get_root_directory();

	GUIThemeDefault theme;
	ResourceManager resources("resources.xml", dir);
	theme.set_resources(resources);

	set_theme(theme);
	set_window_manager(window_manager);
	set_css_document("theme.css", dir);

	if(FileHelp::file_exists(path_to_theme + "\\layout.xml") && FileHelp::file_exists(path_to_theme + "\\layout.css"))
		initialize_layout_manager(path_to_theme + "\\layout.xml", path_to_theme + "\\layout.css");
}

}
