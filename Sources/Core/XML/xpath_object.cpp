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
**    Magnus Norddahl
*/

#include "Core/precomp.h"
#include "API/Core/XML/xpath_object.h"
#include "API/Core/XML/dom_node.h"

/////////////////////////////////////////////////////////////////////////////
// CL_XPathObject_Impl Class:

class CL_XPathObject_Impl
{
public:
	CL_XPathObject_Impl()
	: type(CL_XPathObject::type_null), boolean(false), number(0.0)
	{
	}

	CL_XPathObject::Type type;
	std::vector<CL_DomNode> node_set;
	bool boolean;
	double number;
	CL_String string;
};

/////////////////////////////////////////////////////////////////////////////
// CL_XPathObject Constuction:

CL_XPathObject::CL_XPathObject()
: impl(new CL_XPathObject_Impl)
{
	impl->type = CL_XPathObject::type_null;
}

/////////////////////////////////////////////////////////////////////////////
// CL_XPathObject Attributes:

CL_XPathObject::Type CL_XPathObject::get_type() const
{
	return impl->type;
}

bool CL_XPathObject::is_null() const
{
	return impl->type == type_null;
}

std::vector<CL_DomNode> CL_XPathObject::get_node_set() const
{
	if (impl->type == type_node_set)
		return impl->node_set;
	else
		return std::vector<CL_DomNode>();
}

bool CL_XPathObject::get_boolean() const
{
	if (impl->type == type_boolean)
		return impl->boolean;
	else
		return false;
}

double CL_XPathObject::get_number() const
{
	if (impl->type == type_number)
		return impl->number;
	else
		return 0.0;
}

CL_StringRef CL_XPathObject::get_string() const
{
	if (impl->type == type_string)
		return impl->string;
	else
		return CL_StringRef();
}

/////////////////////////////////////////////////////////////////////////////
// CL_XPathObject Operations:

void CL_XPathObject::set_null()
{
	impl->type = type_null;
	impl->node_set.clear();
}

void CL_XPathObject::set_node_set(const std::vector<CL_DomNode> &node_set)
{
	impl->type = type_node_set;
	impl->node_set = node_set;
}

void CL_XPathObject::set_boolean(bool value)
{
	impl->type = type_boolean;
	impl->boolean = value;
	impl->node_set.clear();
}

void CL_XPathObject::set_number(double value)
{
	impl->type = type_number;
	impl->number = value;
	impl->node_set.clear();
}

void CL_XPathObject::set_string(const CL_StringRef &str)
{
	impl->type = type_string;
	impl->string = str;
	impl->node_set.clear();
}

/////////////////////////////////////////////////////////////////////////////
// CL_XPathObject Implementation:
