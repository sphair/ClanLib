/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
*/

/// \addtogroup clanNetwork_System clanNetwork System
/// \{

#pragma once

#include "api_network.h"

namespace clan
{

/// \brief Initialization class for clanNetwork.
class CL_API_NETWORK SetupNetwork
{
/// \name Construction
/// \{

public:
	/// \brief Initializes clanNetwork.
	/** <p>Constructing a SetupNetwork object is equalent to calling SetupNetwork::init().</p>
	    <p>When the SetupNetwork instance is destroyed, SetupNetwork::deinit() is called.</p>*/
	SetupNetwork(bool register_resources_only = false);

	~SetupNetwork();

/// \}
/// \name Operations
/// \{

public:
/// \}
};

}

/// \}
