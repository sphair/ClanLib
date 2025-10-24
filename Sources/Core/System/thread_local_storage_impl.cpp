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

#include "Core/precomp.h"
#include "thread_local_storage_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ThreadLocalStorage_Impl Construction:

CL_ThreadLocalStorage_Impl::CL_ThreadLocalStorage_Impl()
: reference_count(1)
{
}

CL_ThreadLocalStorage_Impl::~CL_ThreadLocalStorage_Impl()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ThreadLocalStorage_Impl Attributes:

CL_UnknownSharedPtr CL_ThreadLocalStorage_Impl::get_variable(const CL_StringRef &name)
{
	std::map<CL_String, CL_UnknownSharedPtr>::const_iterator it = data.find(name);
	if (it != data.end())
		return it->second;
	else
		return CL_UnknownSharedPtr();	
}

/////////////////////////////////////////////////////////////////////////////
// CL_ThreadLocalStorage_Impl Operations:

void CL_ThreadLocalStorage_Impl::set_variable(const CL_StringRef &name, CL_UnknownSharedPtr ptr)
{
	data[name] = ptr;
}

void CL_ThreadLocalStorage_Impl::add_reference()
{
	reference_count++;
}

void CL_ThreadLocalStorage_Impl::release_reference()
{
	reference_count--;
	if (reference_count == 0)
		delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ThreadLocalStorage_Impl Implementation:
