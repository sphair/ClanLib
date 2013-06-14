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
**    Mark Page
*/


#pragma once

#include "../api_gui.h"
#include "../gui_component.h"
#include "../../Core/Signals/callback_v0.h"
#include "../../Display/Render/graphic_context.h"

namespace clan
{
/// \addtogroup clanGUI_Components clanGUI Components
/// \{

class DragBox_Impl;

/// \brief Drag box component.
class CL_API_GUI DragBox : public GUIComponent
{
/// \name Construction
/// \{

public:

	/// \brief Constructs a DragBox
	///
	/// \param parent = GUIComponent
	DragBox(GUIComponent *parent);

	virtual ~DragBox();

/// \}
/// \name Attributes
/// \{

public:

	using GUIComponent::get_named_item;

	/// \brief Find the child DragBox with the specified component ID name.
	///
	/// If it was not found, an exception is thrown.
	static DragBox *get_named_item(GUIComponent *reference_component, const std::string &id);

/// \}
/// \name Operations
/// \{

public:

/// \}
/// \name Events
/// \{

public:

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<DragBox_Impl> impl;
/// \}
};

}

/// \}
