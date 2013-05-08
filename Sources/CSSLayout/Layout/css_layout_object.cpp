/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

#include "CSSLayout/precomp.h"
#include "API/CSSLayout/Layout/css_layout_object.h"
#include "css_layout_node_impl.h"
#include "BoxTree/css_box_object.h"

namespace clan
{

CSSLayoutObject::CSSLayoutObject()
{
}

CSSLayoutObject::CSSLayoutObject(const std::shared_ptr<CSSLayoutNode_Impl> &impl)
: CSSLayoutElement(impl)
{
}

void CSSLayoutObject::set_intrinsic_width(int width)
{
	CSSReplacedComponent *component = 0;
	if (!is_null())
		component = static_cast<CSSBoxObject*>(impl->box_node)->get_component();
	if (component)
	{
		component->intrinsic_has_width = true;
		component->intrinsic_width = width;
	}
}

void CSSLayoutObject::set_intrinsic_height(int height)
{
	CSSReplacedComponent *component = 0;
	if (!is_null())
		component = static_cast<CSSBoxObject*>(impl->box_node)->get_component();
	if (component)
	{
		component->intrinsic_has_height = true;
		component->intrinsic_height = height;
	}
}

void CSSLayoutObject::set_intrinsic_ratio(float ratio)
{
	CSSReplacedComponent *component = 0;
	if (!is_null())
		component = static_cast<CSSBoxObject*>(impl->box_node)->get_component();
	if (component)
	{
		component->intrinsic_has_ratio = true;
		component->intrinsic_ratio = ratio;
	}
}

void CSSLayoutObject::set_no_intrinsic_width()
{
	CSSReplacedComponent *component = 0;
	if (!is_null())
		component = static_cast<CSSBoxObject*>(impl->box_node)->get_component();
	if (component)
		component->intrinsic_has_width = false;
}

void CSSLayoutObject::set_no_intrinsic_height()
{
	CSSReplacedComponent *component = 0;
	if (!is_null())
		component = static_cast<CSSBoxObject*>(impl->box_node)->get_component();
	if (component)
		component->intrinsic_has_height = false;
}

void CSSLayoutObject::set_no_intrinsic_ratio()
{
	CSSReplacedComponent *component = 0;
	if (!is_null())
		component = static_cast<CSSBoxObject*>(impl->box_node)->get_component();
	if (component)
		component->intrinsic_has_ratio = false;
}

void CSSLayoutObject::set_component_private(CSSReplacedComponent *component)
{
	if (!is_null())
		static_cast<CSSBoxObject*>(impl->box_node)->set_component(component);
	else
		delete component;
}

}
