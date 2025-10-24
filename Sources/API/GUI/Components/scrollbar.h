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
**    Magnus Norddahl
**    Harry Storbacka
**    Kenneth Gangstoe
*/

/// \addtogroup clanGUI_Components clanGUI Components
/// \{

#pragma once

#include "../api_gui.h"
#include "../gui_component.h"
#include "../../Core/Signals/callback_v0.h"

class CL_ScrollBar_Impl;

/// \brief Scroll bar component.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CL_ScrollBar : public CL_GUIComponent
{
/// \name Construction
/// \{
public:

	/// \brief Constructs a ScrollBar
	///
	/// \param parent = GUIComponent
	CL_ScrollBar(CL_GUIComponent *parent);

	virtual ~CL_ScrollBar();

/// \}
/// \name Attributes
/// \{
public:
	using CL_GUIComponent::get_named_item;

	/// \brief Find the child CL_ScrollBar with the specified component ID name.
	///
	/// If it was not found, an exception is thrown.
	static CL_ScrollBar *get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id);

	/// \brief Is Vertical
	///
	/// \return true = vertical
	bool is_vertical() const;

	/// \brief Is Horizontal
	///
	/// \return true = horizontal
	bool is_horizontal() const;

	/// \brief Get Min
	///
	/// \return min
	int get_min() const;

	/// \brief Get Max
	///
	/// \return max
	int get_max() const;

	/// \brief Get Line step
	///
	/// \return line_step
	int get_line_step() const;

	/// \brief Get Page step
	///
	/// \return page_step
	int get_page_step() const;

	/// \brief Get Position
	///
	/// \return position
	int get_position() const;

	/// \brief Get Preferred size
	///
	/// \return preferred_size
	virtual CL_Size get_preferred_size() const;

	/// \brief Returns the preferred width of the scrollbar.
	int get_preferred_width() const;

	/// \brief Returns the preferred height of the scrollbar.
	int get_preferred_height() const;

/// \}
/// \name Operations
/// \{
public:

	/// \brief Set vertical
	void set_vertical();

	/// \brief Set horizontal
	void set_horizontal();

	/// \brief Set min
	///
	/// \param scroll_min = value
	void set_min(int scroll_min);

	/// \brief Set max
	///
	/// \param scroll_max = value
	void set_max(int scroll_max);

	/// \brief Set line step
	///
	/// \param step = value
	void set_line_step(int step);

	/// \brief Set page step
	///
	/// \param step = value
	void set_page_step(int step);

	/// \brief Set ranges
	///
	/// \param scroll_min = value
	/// \param scroll_max = value
	/// \param line_step = value
	/// \param page_step = value
	void set_ranges(int scroll_min, int scroll_max, int line_step, int page_step);

	/// \brief Calculate the scrollbar range and page_step size for scrolling in pixels.
	/** <p>This is equal to calling set_min(0), set_max(total_size-view_size) and
	     set_page_step((total_size-scroll_max)*scroll_max)/(total_size)).
	     The range of the scrollbar will be the amount of pixels the view needs to
	     be moved to show all the data.</p>*/
	void calculate_ranges(int view_size, int total_size);

	/// \brief Set position
	///
	/// \param pos = value
	void set_position(int pos);

/// \}
/// \name Events
/// \{
public:
	/// \brief Called for every type of scrollbar scrolling.
	CL_Callback_v0 &func_scroll();

	/// \brief Called when scrolled to minimum position.
	CL_Callback_v0 &func_scroll_min();

	/// \brief Called when scrolled to maximum position.
	CL_Callback_v0 &func_scroll_max();

	/// \brief Called when decrement button is clicked.
	CL_Callback_v0 &func_scroll_line_decrement();

	/// \brief Called when increment button is clicked.
	CL_Callback_v0 &func_scroll_line_increment();

	/// \brief Called when decrement track is clicked.
	CL_Callback_v0 &func_scroll_page_decrement();

	/// \brief Called when increment track is clicked.
	CL_Callback_v0 &func_scroll_page_increment();

	/// \brief Called when user released scroll thumb.
	CL_Callback_v0 &func_scroll_thumb_release();

	/// \brief Called as user moves the scroll thumb.
	CL_Callback_v0 &func_scroll_thumb_track();

	/// \brief Called when user releases the mouse after scrolling.
	CL_Callback_v0 &func_scroll_end();

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_ScrollBar_Impl> impl;
/// \}
};

/// \}
