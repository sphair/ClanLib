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

/// \addtogroup clanGUI_Components clanGUI Components
/// \{

#pragma once

#include "../api_gui.h"
#include "../gui_component.h"
#include "../../Core/Signals/callback_v0.h"

namespace clan
{

class ScrollBar_Impl;

/// \brief Scroll bar component.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI ScrollBar : public GUIComponent
{
/// \name Construction
/// \{
public:

	/// \brief Constructs a ScrollBar
	///
	/// \param parent = GUIComponent
	ScrollBar(GUIComponent *parent);

	virtual ~ScrollBar();

/// \}
/// \name Attributes
/// \{
public:
	using GUIComponent::get_named_item;

	/// \brief Find the child ScrollBar with the specified component ID name.
	///
	/// If it was not found, an exception is thrown.
	static ScrollBar *get_named_item(GUIComponent *reference_component, const std::string &id);

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
	Callback_v0 &func_scroll();

	/// \brief Called when scrolled to minimum position.
	Callback_v0 &func_scroll_min();

	/// \brief Called when scrolled to maximum position.
	Callback_v0 &func_scroll_max();

	/// \brief Called when decrement button is clicked.
	Callback_v0 &func_scroll_line_decrement();

	/// \brief Called when increment button is clicked.
	Callback_v0 &func_scroll_line_increment();

	/// \brief Called when decrement track is clicked.
	Callback_v0 &func_scroll_page_decrement();

	/// \brief Called when increment track is clicked.
	Callback_v0 &func_scroll_page_increment();

	/// \brief Called when user released scroll thumb.
	Callback_v0 &func_scroll_thumb_release();

	/// \brief Called as user moves the scroll thumb.
	Callback_v0 &func_scroll_thumb_track();

	/// \brief Called when user releases the mouse after scrolling.
	Callback_v0 &func_scroll_end();

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<ScrollBar_Impl> impl;
/// \}
};

}

/// \}
