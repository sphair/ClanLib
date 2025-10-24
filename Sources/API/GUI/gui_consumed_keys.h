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
**    Harry Storbacka
*/

//! clanGUI="System"

#pragma once

#include "api_gui.h"
#include "../Core/System/sharedptr.h"

class CL_GUIConsumedKeys_Impl;

/// \brief GUI consumed keys description.
///
/// \xmlonly !group=GUI/System! !header=gui.h! \endxmlonly
class CL_API_GUI CL_GUIConsumedKeys
{
/// \name Enumerations
/// \{

public:
	/// \brief Keys a component can choose to handle.
	enum ConsumedKey
	{
		key_enter,
		key_arrows,
		key_characters,
		key_tab,
		key_space,
	};

/// \}
/// \name Construction
/// \{

public:
	/// \brief Constructs a GUI theme part.
	CL_GUIConsumedKeys();

	~CL_GUIConsumedKeys();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Is consumed
	///
	/// \param key = Consumed Key
	///
	/// \return bool
	bool is_consumed(ConsumedKey key);

/// \}
/// \name Operations
/// \{

public:
	/// \brief Sets a part state.
	void set_consumed(ConsumedKey key, bool consumed=true);

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_GUIConsumedKeys_Impl> impl;
/// \}
};

