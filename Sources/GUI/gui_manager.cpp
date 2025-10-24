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
**    Harry Storbacka
**    Kenneth Gangstoe
*/

#include "GUI/precomp.h"
#include "API/Core/CSS/css_document.h"
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

/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager Construction:

CL_GUIManager::CL_GUIManager()
: impl(new CL_GUIManager_Impl)
{
	CL_GUIWindowManagerSystem window_manager;
	set_window_manager(window_manager);
}

CL_GUIManager::CL_GUIManager(CL_SharedPtr<CL_GUIManager_Impl> impl)
: impl(impl)
{
}

CL_GUIManager::CL_GUIManager(const CL_DisplayWindow &display_window, const CL_String &path_to_theme)
: impl(new CL_GUIManager_Impl)
{
	CL_GUIWindowManagerTexture window_manager(display_window);
	initialize(window_manager, path_to_theme);
}

CL_GUIManager::CL_GUIManager(const CL_String &path_to_theme)
: impl(new CL_GUIManager_Impl)
{
	CL_GUIWindowManagerSystem window_manager;
	initialize(window_manager, path_to_theme);
}

CL_GUIManager::CL_GUIManager(CL_GUIWindowManager &window_manager, const CL_String &path_to_theme)
: impl(new CL_GUIManager_Impl)
{
	initialize(window_manager, path_to_theme);
}

CL_GUIManager::~CL_GUIManager()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager Attributes:

CL_CSSDocument CL_GUIManager::get_css_document() const
{
	return impl->css_document;
}

CL_GUITheme CL_GUIManager::get_theme() const
{
	return impl->theme;
}

CL_GUIWindowManager CL_GUIManager::get_window_manager() const
{
	return impl->window_manager;
}

CL_GUIComponent *CL_GUIManager::get_capture_component() const
{
	return impl->mouse_capture_component;
}

bool CL_GUIManager::get_exit_flag() const
{
	return impl->exit_flag;
}

int CL_GUIManager::get_exit_code() const
{
	return impl->exit_code;
}

CL_UnknownSharedPtr CL_GUIManager::get_userdata()
{
	return impl->userdata;
}

CL_Font CL_GUIManager::get_registered_font(const CL_FontDescription &desc)
{
	return impl->get_registered_font(desc);
}

CL_GUIComponent * CL_GUIManager::get_focused_component()
{
	return impl->get_focus_component();
}

CL_String CL_GUIManager::get_clipboard_text() const
{
	if (impl->root_components.empty())
		return CL_String();

	CL_GUITopLevelWindow *cur = impl->root_components[0];
	return impl->window_manager.get_display_window(cur).get_clipboard_text();
}


/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager Events:

CL_Signal_v1<CL_GUIMessage &> &CL_GUIManager::sig_filter_message()
{
	return impl->sig_filter_message;
}

CL_Callback_1<int, bool> &CL_GUIManager::func_exec_handler()
{
	return impl->func_exec_handler;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager Operations:

void CL_GUIManager::set_theme(CL_GUITheme &theme)
{
	impl->theme = theme;
}

void CL_GUIManager::set_css_document(CL_CSSDocument css)
{
	impl->css_document = css;
	impl->reset_properties(); // Clear the properties cache
}

void CL_GUIManager::set_css_document(const CL_String &fullname)
{
	CL_CSSDocument css;
	css.load(fullname); 
	set_css_document(css);
}

void CL_GUIManager::set_css_document(const CL_String &filename, const CL_VirtualDirectory &directory)
{
	CL_CSSDocument css;
	css.load(filename, directory); 
	set_css_document(css);
}

void CL_GUIManager::add_resources(const CL_ResourceManager &resources)
{
	get_theme().get_resources().add_resources(resources);
}

void CL_GUIManager::add_resources(const CL_String &filename)
{
	get_theme().get_resources().add_resources(CL_ResourceManager(filename));
}

void CL_GUIManager::add_resources(const CL_String &filename, const CL_VirtualDirectory &directory)
{
	get_theme().get_resources().add_resources(CL_ResourceManager(filename, directory));
}

void CL_GUIManager::initialize_layout_manager(const CL_String &xml_fullname, const CL_String &css_fullname)
{
	impl->layout_manager = CL_GUIThemeLayoutManager(xml_fullname, css_fullname);
}

void CL_GUIManager::set_window_manager(CL_GUIWindowManager &window_manager)
{
	impl->window_manager = window_manager;
	window_manager.set_site(&impl->wm_site);
}

void CL_GUIManager::set_capture_component(CL_GUIComponent *component, bool state)
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

void CL_GUIManager::set_tablet_proximity_component(CL_GUIComponent *comp, bool state)
{
	CL_GUITopLevelWindow *win = impl->get_toplevel_window(comp);

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
void CL_GUIManager::process_messages(int timeout)
{
	CL_KeepAlive::process(timeout);
	impl->window_manager.setup_painting();
	impl->invalidate_constant_repaint_components();
	impl->window_manager.update();
	impl->window_manager.complete_painting();
}

int CL_GUIManager::exec(bool loop_until_complete)
{
	if (!impl->func_exec_handler.is_null())
		return impl->func_exec_handler.invoke(loop_until_complete);

	while (!impl->exit_flag)
	{
		impl->exit_code = 0;

		if (loop_until_complete)	// Check for a modal dialog code
		{
			if (!impl->func_exec_handler.is_null())
				impl->func_exec_handler.invoke(loop_until_complete);
		}

		int timeout = -1;
		if (impl->is_constant_repaint_enabled() || !loop_until_complete)
		{
			timeout = 0;
		}

		process_messages(timeout);

		// The user wants the exits now
		if (!loop_until_complete)
		{
			if (!impl->exit_flag)	// Return now, only if the exist flag is not set
			{
				return 0;
			}
		}
	}
	impl->exit_flag = false;
	return impl->exit_code;
}

void CL_GUIManager::exit_with_code(int exit_code)
{
	impl->exit_flag = true;
	impl->exit_code = exit_code;
}

void CL_GUIManager::clear_exit_flag()
{
	impl->exit_flag = false;
}

void CL_GUIManager::dispatch_message(CL_GUIMessage message)
{
	impl->dispatch_message(message);
}

void CL_GUIManager::request_repaint(const CL_Rect &rect, CL_GUIComponent *root_component)
{
	std::vector<CL_GUITopLevelWindow>::size_type pos, size;
	size = impl->root_components.size();

	for (pos = 0; pos < size; pos++)
	{
		CL_GUITopLevelWindow *cur = impl->root_components[pos];

		if (cur->component == root_component)
		{
			impl->window_manager.request_repaint(cur, rect);
			break;
		}
	}
}

void CL_GUIManager::render_windows()
{
	std::vector<CL_GUITopLevelWindow>::size_type pos, size;
	size = impl->root_components.size();

	for (pos = 0; pos < size; pos++)
	{
		CL_GUITopLevelWindow *cur = impl->root_components[pos];
		CL_Rect geometry = cur->component->get_geometry();
		cur->component->paint(CL_Rect(CL_Point(0,0), geometry.get_size()));
	}
}

void CL_GUIManager::set_userdata(CL_UnknownSharedPtr ptr)
{
	impl->userdata = ptr;
}

void CL_GUIManager::register_font(const CL_Font &font, const CL_FontDescription &desc)
{
	impl->register_font(font, desc);
}

void CL_GUIManager::set_clipboard_text(const CL_StringRef &str)
{
	if (impl->root_components.empty())
		return;

	CL_GUITopLevelWindow *cur = impl->root_components[0];
	impl->window_manager.get_display_window(cur).set_clipboard_text(str);
}

void CL_GUIManager::set_accelerator_table( const CL_AcceleratorTable &table )
{
	impl->accel_table = table;
}

CL_CSSLayout CL_GUIManager::create_layout(CL_GUIComponent *component)
{
	return impl->layout_manager.create_layout(component->get_type_name());
}

bool CL_GUIManager::has_layout(CL_GUIComponent *component)
{
	return impl->layout_manager.has_layout(component->get_type_name());
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager Implementation:

void CL_GUIManager::initialize(CL_GUIWindowManager &window_manager, const CL_String &path_to_theme)
{
	CL_VirtualFileSystem vfs(path_to_theme);
	CL_VirtualDirectory dir = vfs.get_root_directory();

	CL_GUIThemeDefault theme;
	CL_ResourceManager resources("resources.xml", dir);
	theme.set_resources(resources);

	set_theme(theme);
	set_window_manager(window_manager);
	set_css_document("theme.css", dir);

	if(CL_FileHelp::file_exists(path_to_theme + "\\layout.xml") && CL_FileHelp::file_exists(path_to_theme + "\\layout.css"))
		initialize_layout_manager(path_to_theme + "\\layout.xml", path_to_theme + "\\layout.css");
}
