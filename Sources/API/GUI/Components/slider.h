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
*/

/// \addtogroup clanGUI_Components clanGUI Components
/// \{

#pragma once

#include "../api_gui.h"
#include "../gui_component.h"

class CL_Slider_Impl;

/// \brief Track bar component.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CL_Slider : public CL_GUIComponent
{
/// \name Construction
/// \{

public:

	/// \brief Constructs a Slider
	///
	/// \param parent = GUIComponent
	CL_Slider(CL_GUIComponent *parent);

	virtual ~CL_Slider();

/// \}
/// \name Attributes
/// \{

public:
	using CL_GUIComponent::get_named_item;

	/// \brief Find the child CL_Slider with the specified component ID name.
	///
	/// If it was not found, an exception is thrown.
	static CL_Slider *get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id);

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

	/// \brief Get Tick count
	///
	/// \return tick_count
	int get_tick_count() const;

	/// \brief Get Page step
	///
	/// \return page_step
	int get_page_step() const;

	/// \brief Get Position
	///
	/// \return position
	int get_position() const;

	/// \brief Get Lock to ticks
	///
	/// \return lock_to_ticks
	bool get_lock_to_ticks() const;

	/// \brief Get Preferred size
	///
	/// \return preferred_size
	CL_Size get_preferred_size() const;

/// \}
/// \name Operations
/// \{

public:

	/// \brief Set vertical
	///
	/// \param enable = bool
	void set_vertical(bool enable);

	/// \brief Set horizontal
	///
	/// \param enable = bool
	void set_horizontal(bool enable);

	/// \brief Set min
	///
	/// \param slider_min = value
	void set_min(int slider_min);

	/// \brief Set max
	///
	/// \param slider_max = value
	void set_max(int slider_max);

	/// \brief Set tick count
	///
	/// \param tick_count = value
	void set_tick_count(int tick_count);

	/// \brief Set page step
	///
	/// \param steps = value
	void set_page_step(int steps);

	/// \brief Set lock to ticks
	///
	/// \param lock = bool
	void set_lock_to_ticks(bool lock);

	/// \brief Set ranges
	///
	/// \param slider_min = value
	/// \param slider_max = value
	/// \param tick_count = value
	/// \param page_step = value
	void set_ranges(int slider_min, int slider_max, unsigned int tick_count, int page_step);

	/// \brief Set position
	///
	/// \param pos = value
	void set_position(int pos);

/// \}
/// \name Callbacks
/// \{

public:
	/// \brief Emitted while the slider is being moved.
	CL_Callback_v0 &func_value_changed();

	/// \brief Emitted when the slider value is decremented (while moving or when clicking the track).
	/** Invoked while moving or when clicking the track.*/
	CL_Callback_v0 &func_value_decremented();

	/// \brief Emitted when the slider value is incremented.
	/** Invoked while moving or when clicking the track.*/
	CL_Callback_v0 &func_value_incremented();

	/// \brief Emitted after the slider has been moved.
	CL_Callback_v0 &func_slider_moved();

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_Slider_Impl> impl;
/// \}
};

/// \}
