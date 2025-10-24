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
#include "../../Core/Signals/callback_v0.h"

class CL_SpanLayout;
class CL_Label_Impl;

/// \brief Check box component.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CL_Label : public CL_GUIComponent
{
/// \name Construction
/// \{

public:

	/// \brief Constructs a Label
	///
	/// \param parent = GUIComponent
	CL_Label(CL_GUIComponent *parent);

	virtual ~CL_Label();

/// \}
/// \name Attributes
/// \{

public:
	enum Alignment
	{
		align_left,
		align_center,
		align_right,
		align_justify
	};

	using CL_GUIComponent::get_named_item;

	/// \brief Find the child CL_Label with the specified component ID name.
	///
	/// If it was not found, an exception is thrown.
	static CL_Label *get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id);

	/// \brief Get Alignment
	///
	/// \return alignment
	Alignment get_alignment() const;

	/// \brief Get Text
	///
	/// \return text
	CL_String get_text() const;

	/// \brief Get Preferred size
	///
	/// \return preferred_size
	CL_Size get_preferred_size() const;

/// \}
/// \name Operations
/// \{

public:

	/// \brief Set text
	///
	/// \param text = String Ref
	void set_text(const CL_StringRef &text);

	/// \brief Set span
	///
	/// \param layout = Span Layout
	void set_span(const CL_SpanLayout &layout);

	/// \brief Set alignment
	///
	/// \param alignment = Alignment
	void set_alignment(Alignment alignment);

/// \}
/// \name Events
/// \{

public:

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_Label_Impl> impl;
/// \}
};

/// \}
