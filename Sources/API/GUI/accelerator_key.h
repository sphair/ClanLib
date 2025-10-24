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
**    Harry Storbacka
*/

/// \addtogroup clanGUI_System clanGUI System
/// \{

#pragma once

#include "api_gui.h"
#include "../Core/System/sharedptr.h"
#include "../Core/Signals/callback_v2.h"
#include "../GUI/gui_message.h"
#include "../GUI/gui_message_input.h"
#include <vector>

class CL_AcceleratorKey_Impl;

/// \brief GUI Accelerator Table Entry.
///
/// \xmlonly !group=GUI/System! !header=gui.h! \endxmlonly
class CL_AcceleratorKey
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a null instance
	CL_AcceleratorKey();

	/// \brief Constructs a AcceleratorKey
	///
	/// \param id = value
	/// \param mod1 = value
	/// \param mod2 = value
	/// \param mod3 = value
	CL_AcceleratorKey(int id, int mod1=-1, int mod2=-1, int mod3=-1);

	~CL_AcceleratorKey();

/// \}
/// \name Attributes
/// \{

public:
	CL_Callback_v2<CL_GUIMessage, CL_AcceleratorKey> &func_pressed();

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
	bool is_null() const { return impl.is_null(); }

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
	CL_SharedPtr<CL_AcceleratorKey_Impl> impl;
/// \}
};

/// \}
