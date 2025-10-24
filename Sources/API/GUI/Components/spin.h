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

class CL_Spin_Impl;

/// \brief Spin component.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CL_Spin : public CL_GUIComponent
{
/// \name Construction
/// \{
public:

	/// \brief Constructs a Spin
	///
	/// \param parent = GUIComponent
	CL_Spin(CL_GUIComponent *parent);

	virtual ~CL_Spin();

/// \}
/// \name Attributes
/// \{
public:

	
	using CL_GUIComponent::get_named_item;

	/// \brief Find the child CL_Spin with the specified component ID name.
	///
	/// If it was not found, an exception is thrown.
	static CL_Spin *get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id);

	/// \brief Get Value
	///
	/// \return value
	int get_value() const;

	/// \brief Get Value float
	///
	/// \return value_float
	float get_value_float() const;

	/// \brief Get Min
	///
	/// \return min
	int get_min() const;

	/// \brief Get Max
	///
	/// \return max
	int get_max() const;

	/// \brief Get Min float
	///
	/// \return min_float
	float get_min_float() const;

	/// \brief Get Max float
	///
	/// \return max_float
	float get_max_float() const;

	/// \brief Get Floating point mode
	///
	/// \return floating_point_mode
	bool get_floating_point_mode() const;

/// \}
/// \name Operations
/// \{
public:

	/// \brief Set value
	///
	/// \param value = value
	void set_value(int value);

	/// \brief Set value float
	///
	/// \param value = value
	void set_value_float(float value);

	/// \brief Set ranges
	///
	/// \param min = value
	/// \param max = value
	void set_ranges(int min, int max);

	/// \brief Set ranges float
	///
	/// \param min = value
	/// \param max = value
	void set_ranges_float(float min, float max);

	/// \brief Set step size
	///
	/// \param step_size = value
	void set_step_size(int step_size);

	/// \brief Set step size float
	///
	/// \param step_size = value
	void set_step_size_float(float step_size);

	/// \brief Set number of decimal places
	///
	/// \param decimal_places = value
	void set_number_of_decimal_places(int decimal_places);

	/// \brief Set floating point mode
	///
	/// \param use_floating_point = bool
	void set_floating_point_mode(bool use_floating_point);

/// \}
/// \name Events
/// \{
public:

	/// \brief Func value changed
	///
	/// \return Callback_v0
	CL_Callback_v0 &func_value_changed();

/// \}
/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_Spin_Impl> impl;
/// \}
};

/// \}
