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
**    Harry Storbacka
**    Mark Page
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_layout.h"
#include "API/GUI/Providers/gui_layout_provider.h"
#include "API/Core/Math/size.h"

class CL_GUILayout_Impl
{
public:
	CL_GUILayout_Impl()
	: provider(0)
	{
	}

	~CL_GUILayout_Impl()
	{
		if (provider)
			provider->destroy();
	}

	CL_GUILayoutProvider *provider;
};

/////////////////////////////////////////////////////////////////////////////
// CL_GUILayout Construction:

CL_GUILayout::CL_GUILayout()
{
}

CL_GUILayout::CL_GUILayout(CL_GUILayoutProvider *provider)
: impl(new CL_GUILayout_Impl)
{
	impl->provider = provider;
}

CL_GUILayout::~CL_GUILayout()
{
}


/////////////////////////////////////////////////////////////////////////////
// CL_GUILayout Attributes:

bool CL_GUILayout::is_null() const
{
	return !impl;
}

CL_GUILayoutProvider *CL_GUILayout::get_provider() const
{
	if (!impl)
		return 0;
	return impl->provider;
}

CL_Size CL_GUILayout::get_preferred_size() const
{
	return impl->provider->get_preferred_size();
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUILayout Operations:

void CL_GUILayout::set_geometry(const CL_Rect &pos)
{
	impl->provider->set_geometry(pos);
}

void CL_GUILayout::set_preferred_size(const CL_Size &size)
{
	impl->provider->set_preferred_size(size);
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUILayout Implementation:
