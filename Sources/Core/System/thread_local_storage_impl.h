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

#pragma once


#include "API/Core/System/sharedptr.h"
#include <map>

class CL_ThreadLocalStorage_Impl
{
/// \name Construction
/// \{

public:
	CL_ThreadLocalStorage_Impl();

private:
	~CL_ThreadLocalStorage_Impl();


/// \}
/// \name Attributes
/// \{

public:
	CL_UnknownSharedPtr get_variable(const CL_StringRef &name);


/// \}
/// \name Operations
/// \{

public:
	void set_variable(const CL_StringRef &name, CL_UnknownSharedPtr ptr);

	void add_reference();

	void release_reference();


/// \}
/// \name Implementation
/// \{

protected:
	int reference_count;

	std::map<CL_String, CL_UnknownSharedPtr> data;
/// \}
};


