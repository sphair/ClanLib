/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
*/


#pragma once

#include "api_gui.h"
#include <memory>
#include "../Core/Signals/callback.h"
#include "../GUI/gui_message.h"
#include "../GUI/gui_message_input.h"
#include <vector>

namespace clan
{
/// \addtogroup clanGUI_System clanGUI System
/// \{

class AcceleratorKey_Impl;

/// \brief GUI Accelerator Table Entry.
class AcceleratorKey
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a null instance
	AcceleratorKey();

	/// \brief Constructs a AcceleratorKey
	///
	/// \param id = value
	/// \param mod1 = value
	/// \param mod2 = value
	/// \param mod3 = value
	AcceleratorKey(int id, int mod1=-1, int mod2=-1, int mod3=-1);

	~AcceleratorKey();

/// \}
/// \name Attributes
/// \{

public:
	Callback<void(std::shared_ptr<GUIMessage> &, AcceleratorKey)> &func_pressed();

	/// \brief Get Shift
	///
	/// \return shift
	bool get_shift() const;

	/// \brief Get Alt
	///
	/// \return alt
	bool get_alt() const;

	/// \brief Get Ctrl
	///
	/// \return ctrl
	bool get_ctrl() const;

	/// \brief Get Id
	///
	/// \return id
	int get_id() const;

	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;


/// \}
/// \name Operations
/// \{

public:

	/// \brief Set Shift
	void set_shift(bool);

	/// \brief Set Alt
	void set_alt(bool);

	/// \brief Set Ctrl
	void set_ctrl(bool);

	/// \brief Set Id
	void set_id(int);


/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<AcceleratorKey_Impl> impl;
/// \}
};

}

/// \}
