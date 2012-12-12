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
*/

/// \addtogroup clanGUI_Components clanGUI Components
/// \{

#pragma once

#include "../api_gui.h"
#include "../gui_component.h"

namespace clan
{

class Tab_Impl;
class TabHeader;
class TabPage;
class DomElement;

/// \brief Tab component.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI Tab : public GUIComponent
{
/// \name Construction
/// \{

public:

	/// \brief Constructs a Tab
	///
	/// \param parent = GUIComponent
	Tab(GUIComponent *parent);

	virtual ~Tab();

/// \}
/// \name Attributes
/// \{

public:
	using GUIComponent::get_named_item;

	/// \brief Find the child Tab with the specified component ID name.
	///
	/// If it was not found, an exception is thrown.
	static Tab *get_named_item(GUIComponent *reference_component, const std::string &id);

	/// \brief Get page
	///
	/// \param index = value
	///
	/// \return Tab Page
	TabPage *get_page(int index) const;

	/// \brief Get page by id
	///
	/// \param id = value
	///
	/// \return Tab Page
	TabPage *get_page_by_id(int id) const;

	/// \brief Get Current page index
	///
	/// \return current_page_index
	int get_current_page_index() const;

	/// \brief Get Current page id
	///
	/// \return current_page_id
	int get_current_page_id() const;

	/// \brief returns the number of pages in the tab.
	int get_page_count() const;

/// \}
/// \name Events
/// \{
public:
	/// \brief void func_page_selected(Page *tab_page)
	Callback_v1<TabPage*> &func_page_selected();

/// \}
/// \name Operations
/// \{

public:
	/// \brief Create new tab page. By default the ID of the tab is 0.
	TabPage *add_page(const std::string &label, int id=0);

	/// \brief Show a page by its index.
	void show_page(int index);

	/// \brief Show a page by its ID. In case of non-unique ID's the first match will be shown.
	void show_page_by_id(int id);

	/// \brief Remove a page by its index
	void remove_page(int index);

	/// \brief Remove a page by its ID.
	/** In case of non-unique ID's the first match will be shown.
	    The pointer returned by add_page() will become invalid.*/
	void remove_page_by_id(int id);

	/// \brief Set the label of the component specified by index.
	void set_label(int index, const std::string &new_label);

	/// \brief Set the label of the component specified by index.
	void set_label_by_id(int id, const std::string &new_label);

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<Tab_Impl> impl;

/// \}
};

}

/// \}
