/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "Core/precomp.h"
#include "API/Core/XML/dom_attr.h"
#include "API/Core/XML/dom_document.h"
#include "dom_node_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DomAttr construction:

CL_DomAttr::CL_DomAttr()
{
}

CL_DomAttr::CL_DomAttr(CL_DomDocument doc, const std::string &name)
: CL_DomNode(doc, ATTRIBUTE_NODE)
{
	impl->node_name = name;
}

CL_DomAttr::CL_DomAttr(const CL_SharedPtr<CL_DomNode_Generic> &impl) : CL_DomNode(impl)
{
}

CL_DomAttr::~CL_DomAttr()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomAttr attributes:

std::string CL_DomAttr::get_name() const
{
	if (impl) return impl->node_name;
	return std::string();
}
	
bool CL_DomAttr::get_specified() const
{
	return false;
}
	
std::string CL_DomAttr::get_value() const
{
	if (impl) return impl->node_value;
	return std::string();
}
	
void CL_DomAttr::set_value(const std::string &value)
{
	if (impl) impl->node_value = value;
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomAttr operations:

/////////////////////////////////////////////////////////////////////////////
// CL_DomAttr implementation:
