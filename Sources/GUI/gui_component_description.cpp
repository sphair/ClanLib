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

#include "GUI/precomp.h"
#include "API/GUI/gui_component_description.h"
#include "gui_component_description_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GUIComponentDescription Construction:

CL_GUIComponentDescription::CL_GUIComponentDescription()
: impl(new CL_GUIComponentDescription_Impl)
{
}

CL_GUIComponentDescription::CL_GUIComponentDescription(const CL_GUIComponentDescription &copy, const CL_StringRef &default_type_name)
: impl(new CL_GUIComponentDescription_Impl)
{
	*impl.get() = *copy.impl.get();
	if (impl->type_name.empty())
		impl->type_name = default_type_name;
}

CL_GUIComponentDescription::~CL_GUIComponentDescription()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIComponentDescription Attributes:

CL_StringRef CL_GUIComponentDescription::get_type_name() const
{
	return impl->type_name;
}
	
CL_StringRef CL_GUIComponentDescription::get_class_name() const
{
	return impl->class_name;
}

CL_StringRef CL_GUIComponentDescription::get_id_name() const
{
	return impl->id_name;
}

bool CL_GUIComponentDescription::get_allow_resize() const
{
	return impl->allow_resize;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIComponentDescription Operations:

void CL_GUIComponentDescription::set_type_name(const CL_StringRef &name)
{
	impl->type_name = name;
}
	
void CL_GUIComponentDescription::set_class_name(const CL_StringRef &name)
{
	impl->class_name = name;
}

void CL_GUIComponentDescription::set_id_name(const CL_StringRef &name)
{
	impl->id_name = name;
}

void CL_GUIComponentDescription::set_allow_resize(bool value)
{
	impl->allow_resize = value;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIComponentDescription Implementation:
