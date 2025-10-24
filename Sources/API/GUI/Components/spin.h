/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
	CL_Spin(CL_GUIComponent *parent);

	virtual ~CL_Spin();

/// \}
/// \name Attributes
/// \{
public:
	int get_value() const;

	float get_value_float() const;

	int get_min() const;

	int get_max() const;

	float get_min_float() const;

	float get_max_float() const;

	bool get_floating_point_mode() const;

/// \}
/// \name Operations
/// \{
public:
	void set_value(int value);

	void set_value_float(float value);

	void set_ranges(int min, int max);

	void set_ranges_float(float min, float max);

	void set_step_size(int step_size);

	void set_step_size_float(float step_size);

	void set_number_of_decimal_places(int decimal_places);

	void set_floating_point_mode(bool use_floating_point);

/// \}
/// \name Events
/// \{
public:
	CL_Callback_v0 &func_value_changed();

/// \}
/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_Spin_Impl> impl;
/// \}
};

/// \}
