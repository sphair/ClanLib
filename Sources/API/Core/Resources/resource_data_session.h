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
*/

/// \addtogroup clanCore_Resources clanCore Resources
/// \{

#pragma once

#include "../api_core.h"
#include "resource.h"

/// \brief Resource data usage tracker / garbage collector.
///
/// \xmlonly !group=Core/Resources! !header=core.h! \endxmlonly
class CL_API_CORE CL_ResourceDataSession
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a resource data session.
	CL_ResourceDataSession();

	/// \brief Constructs a ResourceDataSession
	///
	/// \param data_name = String
	/// \param resource = Resource
	CL_ResourceDataSession(const CL_String &data_name, const CL_Resource &resource);

	/// \brief Constructs a ResourceDataSession
	///
	/// \param copy = Resource Data Session
	CL_ResourceDataSession(const CL_ResourceDataSession &copy);

	~CL_ResourceDataSession();

/// \}
/// \name Attributes
/// \{

public:

/// \}
/// \name Operations
/// \{

public:
	CL_ResourceDataSession &operator =(const CL_ResourceDataSession &copy);

/// \}
/// \name Implementation
/// \{

private:
	CL_String name;

	CL_Resource resource;
/// \}
};

/// \}
