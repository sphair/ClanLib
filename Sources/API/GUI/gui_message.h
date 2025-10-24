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
**    Magnus Norddahl
**    Harry Storbacka
*/

/// \addtogroup clanGUI_System clanGUI System
/// \{

#pragma once

#include "api_gui.h"
#include "../Core/System/sharedptr.h"

class CL_GUIComponent;
class CL_GUIMessage_Impl;

/// \brief GUI message.
///
/// \xmlonly !group=GUI/System! !header=gui.h! \endxmlonly
class CL_API_GUI CL_GUIMessage
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a GUI message.
	CL_GUIMessage();

	virtual ~CL_GUIMessage();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the message type name.
	CL_StringRef get_type() const;

	/// \brief Returns true if the message matches the type.
	bool is_type(const CL_StringRef &type) const;

	/// \brief Returns the stored data object for a given data name.
	CL_UnknownSharedPtr get_data(const CL_StringRef &data_name) const;

	/// \brief Returns the target destination of the message.
	CL_GUIComponent *get_target() const;

	/// \brief Returns true if the message has been consumed by a component.
	bool is_consumed() const;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Sets the message type name.
	void set_type(const CL_StringRef &type);

	/// \brief Stores an object in the given data name slot.
	void set_data(const CL_StringRef &data_name, const CL_UnknownSharedPtr &ptr);

	/// \brief Sets the target destination.
	void set_target(CL_GUIComponent *target);

	/// \brief Sets this message to be a null message.
	void set_null();

	/// \brief Sets this message as consumed.
	void set_consumed();

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_GUIMessage_Impl> impl;
/// \}
};

/// \}
