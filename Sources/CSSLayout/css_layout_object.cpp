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

#include "CSSLayout/precomp.h"
#include "API/CSSLayout/css_layout_object.h"
#include "css_layout_node_impl.h"
#include "BoxTree/css_box_object.h"

CL_CSSLayoutObject::CL_CSSLayoutObject()
{
}

CL_CSSLayoutObject::CL_CSSLayoutObject(const CL_SharedPtr<CL_CSSLayoutNode_Impl> &impl)
: CL_CSSLayoutElement(impl)
{
}

void CL_CSSLayoutObject::set_component_private(CL_CSSReplacedComponent *component)
{
	if (!is_null())
		static_cast<CL_CSSBoxObject*>(impl->box_node)->set_component(component);
	else
		delete component;
}
