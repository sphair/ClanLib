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

#include <memory>
#include "../GUI/gui_message.h"
#include <vector>

namespace clan
{
/// \addtogroup clanGUI_System clanGUI System
/// \{

class AcceleratorTable_Impl;
class AcceleratorKey_Impl;
class AcceleratorKey;

/// \brief GUI Accelerator Table.
class AcceleratorTable
{
/// \name Construction
/// \{

public:
	AcceleratorTable();

	~AcceleratorTable();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns a vector containing the accelerator keys defined for this table.
	AcceleratorKey get_accelerator(int id, bool shift=false, bool alt=false, bool ctrl=false);

	/// \brief Returns a vector containing the accelerator keys defined for this table.
	std::vector<AcceleratorKey> get_accelerators();

/// \}
/// \name Operations
/// \{

public:
	/// \brief Add an accelerator table entry to the accelerator table.
	void add_accelerator(const AcceleratorKey &accel);

	/// \brief Check if the message is an accelerator key and invoke relevant accelerator callback.
	void process_message(std::shared_ptr<GUIMessage> &msg);

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<AcceleratorTable_Impl> impl;
/// \}
};

}

/// \}
