/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
**    Magnus Norddahl
*/

#include "GUI/precomp.h"
#include "API/Core/Text/string_format.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/tab.h"
#include "API/GUI/Components/tab_page.h"
#include "API/Core/XML/dom_element.h"
#include "../gui_css_strings.h"
#include "tab_header.h"
#include "tab_page_impl.h"
#include <map>
#include "API/Core/Math/rect.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Tab_Impl Class:

class CL_Tab_Impl
{
public:
	void on_process_message(CL_GUIMessage &msg);

	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);

	void on_style_changed();

	void on_header_page_selected(CL_TabPage*);

	void on_resized();

	CL_Rect get_client_rect();

	CL_Tab *tab;

	std::vector<CL_TabPage*> pages;

	CL_GUIThemePart part_background;

	CL_TabHeader *tab_header;
};

/////////////////////////////////////////////////////////////////////////////
// CL_Tab Construction:

CL_Tab::CL_Tab(CL_GUIComponent *parent)
: CL_GUIComponent(parent), impl(new CL_Tab_Impl)
{
	set_type_name(CssStr::Tab::type_name);
	impl->tab = this;
	func_process_message().set(impl.get(), &CL_Tab_Impl::on_process_message);
	func_render().set(impl.get(), &CL_Tab_Impl::on_render);
	func_style_changed().set(impl.get(), &CL_Tab_Impl::on_style_changed);
	func_resized().set(impl.get(), &CL_Tab_Impl::on_resized);

	impl->tab_header = new CL_TabHeader(this);

	impl->tab_header->func_page_selected().set(impl.get(), &CL_Tab_Impl::on_header_page_selected);

	impl->on_style_changed();
}

CL_Tab::~CL_Tab()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Tab Attributes:

CL_Tab *CL_Tab::get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id)
{
	CL_Tab *object = NULL;
	if (reference_component)
		object = dynamic_cast<CL_Tab*>(reference_component->get_named_item(id));

	if (!object)
		throw CL_Exception(cl_format("Cannot find CL_Tab named item: %1", id));

	return object;
}

CL_TabPage *CL_Tab::get_page(int index) const
{
	return impl->pages[index];
}

CL_TabPage *CL_Tab::get_page_by_id(int id) const
{
	std::vector<CL_TabPage*>::const_iterator it;
	for (it = impl->pages.begin(); it != impl->pages.end(); ++it)
	{
		if ((*it)->get_id() == id)
			return (*it);
	}
		
	return 0;
}

CL_Size CL_Tab::get_preferred_size() const
{
	return impl->part_background.get_preferred_size();
}

int CL_Tab::get_current_page_index() const
{
	std::vector<CL_TabPage*>::const_iterator it;
	int i = 0;
	for (it = impl->pages.begin(); it != impl->pages.end(); ++it)
	{
		if ((*it)->is_visible())
			return i;
		i++;
	}
	return -1;
}

int CL_Tab::get_current_page_id() const
{
	int index = get_current_page_index();
	if (index != -1)
		return get_page(index)->get_id();
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Tab Operations:

CL_TabPage *CL_Tab::add_page(const CL_StringRef &label, int id)
{
	CL_TabPage *tab_page = new CL_TabPage(this);
	tab_page->impl->id = id;
	tab_page->impl->label = label;

	bool select_added_page = true;
	if (!impl->pages.empty())
	{
		tab_page->set_visible(false);
		select_added_page = false;
	}

	impl->pages.push_back(tab_page);
	impl->tab_header->add_page(tab_page, label);

	tab_page->set_geometry(impl->get_client_rect());

	request_repaint();

	if (select_added_page)
	{
		impl->tab_header->select_page(0);
	}

	return tab_page;
}

void CL_Tab::show_page(int index)
{
	std::vector<CL_TabPage*>::iterator it;
	int i = 0;
	for (it = impl->pages.begin(); it != impl->pages.end(); ++it)
	{
		if (i == index)
			(*it)->set_visible(true);
		else
			(*it)->set_visible(false);
		i++;
	}
	impl->tab_header->select_page(index);
	request_repaint();
}

void CL_Tab::show_page_by_id(int id)
{
	std::vector<CL_TabPage*>::iterator it;
	int index = -1;
	int i = 0;
	for (it = impl->pages.begin(); it != impl->pages.end(); ++it, i++)
	{
		if ((*it)->get_id() == id)
		{
			index = i;
			impl->pages[id]->set_visible(true);
		}
		else
		{
			impl->pages[id]->set_visible(false);
		}
	}
	if (index != -1)
		impl->tab_header->select_page(index);
	request_repaint();
}

void CL_Tab::remove_page_by_id(int id)
{
	std::vector<CL_TabPage*>::iterator it;
	for (it = impl->pages.begin(); it != impl->pages.end(); ++it)
	{
		if ((*it)->get_id() == id)
		{
			it = impl->pages.erase(it);
			break;
		}
	}
	request_repaint();
}

void CL_Tab::remove_page(int index)
{
	std::vector<CL_TabPage*>::iterator it;
	int i = 0;
	for (it = impl->pages.begin(); it != impl->pages.end(); ++it)
	{
		if (index == i)
		{
			it = impl->pages.erase(it);
			break;
		}
		i++;
	}
	request_repaint();	
}

int CL_Tab::get_page_count() const
{
	return impl->pages.size();
}

void CL_Tab::set_label(int index, const CL_StringRef &new_label)
{
	std::vector<CL_TabPage*>::iterator it; int i = 0;
	for (it = impl->pages.begin(); it != impl->pages.end(); ++it)
	{
		if (index == i)
		{
			(*it)->impl->label = new_label;
			impl->tab_header->page_renamed((*it));
			break;
		}
		i++;
	}
	request_repaint();
}

void CL_Tab::set_label_by_id(int id, const CL_StringRef &new_label)
{
	std::vector<CL_TabPage*>::iterator it;
	for (it = impl->pages.begin(); it != impl->pages.end(); ++it)
	{
		if ((*it)->get_id() == id)
		{
			(*it)->impl->label = new_label;
			break;
		}
	}
	request_repaint();
}


/////////////////////////////////////////////////////////////////////////////
// CL_Tab Implementation:

void CL_Tab_Impl::on_process_message(CL_GUIMessage &msg)
{
}

void CL_Tab_Impl::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_Rect rect = tab->get_geometry().get_size();
	rect.top = tab_header->get_geometry().bottom;

	part_background.render_box(gc, rect, update_rect);

}

void CL_Tab_Impl::on_style_changed()
{
	part_background = CL_GUIThemePart(tab);

	// Hack to test if child component caches need to be invalidated when CL_GUIComponent::set_*_name is called.
	tab_header->set_class_name(tab_header->get_class_name());
}

void CL_Tab_Impl::on_header_page_selected(CL_TabPage *tab_page)
{
	std::vector<CL_TabPage*>::iterator it;
	for (it = pages.begin(); it != pages.end(); ++it)
	{
		if ((*it) != tab_page)
			(*it)->set_visible(false);
		else
			(*it)->set_visible(true);
	}

	tab->request_repaint();
}

CL_Rect CL_Tab_Impl::get_client_rect()
{
	CL_Rect hr = tab_header->get_geometry();
	CL_Rect g = tab->get_geometry().get_size();
	g.top = hr.bottom;
	CL_Rect content = part_background.get_content_box(g);
	return content;
}

void CL_Tab_Impl::on_resized()
{
	CL_Rect header_rect = tab->get_geometry().get_size();
	header_rect.bottom = tab_header->get_preferred_height();

	tab_header->set_geometry(header_rect);

	std::vector<CL_TabPage*>::iterator it;
	for (it = pages.begin(); it!=pages.end(); ++it)
	{
		CL_Rect cr = get_client_rect();
		(*it)->set_geometry(cr);
	}
}
