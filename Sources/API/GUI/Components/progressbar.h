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
**    Kenneth Gangstoe
**    Harry Storbacka
*/

/// \addtogroup clanGUI_Components clanGUI Components
/// \{


#pragma once


#include "../api_gui.h"
#include "../gui_component.h"

class CL_ProgressBar_Impl;

/// \brief Progress bar component.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CL_ProgressBar : public CL_GUIComponent
{
/// \name Construction
/// \{

public:
	CL_ProgressBar(CL_GUIComponent *parent);

	virtual ~CL_ProgressBar();


/// \}
/// \name Attributes
/// \{

public:
	int get_min() const;

	int get_max() const;

	int get_step_size() const;

	int get_position() const;

	bool is_marquee_mode() const;

	int get_marquee_animation_speed() const;


/// \}
/// \name Operations
/// \{

public:
	void set_min(int progress_min);

	void set_max(int progress_max);

	void set_range(int progress_min, int progress_max);

	void set_step_size(int size);

	void set_position(int pos);

	void advance_position(int pos);

	void step_position();

	void set_marquee_mode(bool enable);

	void set_marquee_animation_speed(int milliseconds);


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_ProgressBar_Impl> impl;
/// \}
};


/// \}
