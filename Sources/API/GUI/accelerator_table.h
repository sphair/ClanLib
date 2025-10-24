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
*/

/// \addtogroup clanGUI_System clanGUI System
/// \{

#pragma once

#include "api_gui.h"
#include "../Core/System/sharedptr.h"
#include "../Core/Signals/callback_v1.h"
#include "../GUI/gui_message.h"
#include <vector>

class CL_AcceleratorTable_Impl;
class CL_AcceleratorKey_Impl;
class CL_AcceleratorKey;

/// \brief GUI Accelerator Table.
///
/// \xmlonly !group=GUI/System! !header=gui.h! \endxmlonly
class CL_API_GUI CL_AcceleratorTable
{
/// \name Construction
/// \{

public:
	CL_AcceleratorTable();

	~CL_AcceleratorTable();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns a vector containing the accelerator keys defined for this table.
	CL_AcceleratorKey get_accelerator(int id, bool shift=false, bool alt=false, bool ctrl=false);

	/// \brief Returns a vector containing the accelerator keys defined for this table.
	std::vector<CL_AcceleratorKey> get_accelerators();

/// \}
/// \name Operations
/// \{

public:
	/// \brief Add an accelerator table entry to the accelerator table.
	void add_accelerator(const CL_AcceleratorKey &accel);

	/// \brief Check if the message is an accelerator key and invoke relevant accelerator callback.
	void process_message(CL_GUIMessage &msg);

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_AcceleratorTable_Impl> impl;
/// \}
};

/// \}
