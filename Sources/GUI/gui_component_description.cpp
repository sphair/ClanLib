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
**    Harry Storbacka
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_component_description.h"
#include "gui_component_description_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GUIComponentDescription Construction:

GUIComponentDescription::GUIComponentDescription()
: impl(new GUIComponentDescription_Impl)
{
}

GUIComponentDescription::GUIComponentDescription(const GUIComponentDescription &copy, const std::string &default_type_name)
: impl(new GUIComponentDescription_Impl)
{
	*impl.get() = *copy.impl.get();
	if (impl->type_name.empty())
		impl->type_name = default_type_name;
}

GUIComponentDescription::~GUIComponentDescription()
{
}

/////////////////////////////////////////////////////////////////////////////
// GUIComponentDescription Attributes:

std::string GUIComponentDescription::get_tag_name() const
{
	return impl->type_name;
}
	
std::string GUIComponentDescription::get_class() const
{
	return impl->class_name;
}

std::string GUIComponentDescription::get_id() const
{
	return impl->id_name;
}

bool GUIComponentDescription::get_allow_resize() const
{
	return impl->allow_resize;
}

/////////////////////////////////////////////////////////////////////////////
// GUIComponentDescription Operations:

void GUIComponentDescription::set_tag_name(const std::string &name)
{
	impl->type_name = name;
}
	
void GUIComponentDescription::set_class(const std::string &name)
{
	impl->class_name = name;
}

void GUIComponentDescription::set_id(const std::string &name)
{
	impl->id_name = name;
}

void GUIComponentDescription::set_allow_resize(bool value)
{
	impl->allow_resize = value;
}

/////////////////////////////////////////////////////////////////////////////
// GUIComponentDescription Implementation:

}
