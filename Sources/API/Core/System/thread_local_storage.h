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

/// \addtogroup clanCore_System clanCore System
/// \{


#pragma once


#include "../api_core.h"
#include "sharedptr.h"

class CL_ThreadLocalStorage_Impl;

/// \brief Thread class.
///
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
class CL_API_CORE CL_ThreadLocalStorage
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a Thread Local Storage object.
	CL_ThreadLocalStorage();

	~CL_ThreadLocalStorage();

private:
	/// \brief Create the initial instance of CL_ThreadLocalStorage (Called by CL_SetupCore)
	static void create_initial_instance();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Get a variable.
	static CL_UnknownSharedPtr get_variable(const CL_StringRef &name);


/// \}
/// \name Operations
/// \{

public:
	/// \brief Set a variable.
	static void set_variable(const CL_StringRef &name, CL_UnknownSharedPtr ptr);


/// \}
/// \name Implementation
/// \{

private:
/// \}
	friend class CL_SetupCore;
};


/// \}
