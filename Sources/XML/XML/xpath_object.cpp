/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
**    Thomas Gottschalk Larsen
*/

#include "XML/precomp.h"
#include "API/XML/xpath_object.h"
#include "API/XML/dom_node.h"

namespace clan
{
	class XPathObject_Impl
	{
	public:
		XPathObject_Impl()
			: type(XPathObject::type_null), boolean(false), number(0.0)
		{
		}

		XPathObject::Type type;
		std::vector<DomNode> node_set;
		bool boolean;
		double number;
		std::string string;
	};

	XPathObject::XPathObject()
		: impl(std::make_shared<XPathObject_Impl>())
	{
		impl->type = XPathObject::type_null;
	}

	XPathObject::XPathObject(bool value)
		: impl(std::make_shared<XPathObject_Impl>())
	{
		set_boolean(value);
	}

	XPathObject::XPathObject(double value)
		: impl(std::make_shared<XPathObject_Impl>())
	{
		set_number(value);
	}

	XPathObject::XPathObject(size_t value)
		: impl(std::make_shared<XPathObject_Impl>())
	{
		set_number(value);
	}

	XPathObject::XPathObject(const std::string &value)
		: impl(std::make_shared<XPathObject_Impl>())
	{
		set_string(value);
	}

	XPathObject::XPathObject(const std::vector<DomNode> &value)
		: impl(std::make_shared<XPathObject_Impl>())
	{
		set_node_set(value);
	}

	XPathObject::Type XPathObject::get_type() const
	{
		return impl->type;
	}

	bool XPathObject::is_null() const
	{
		return impl->type == type_null;
	}

	std::vector<DomNode> XPathObject::get_node_set() const
	{
		if (impl->type == type_node_set)
			return impl->node_set;
		else
			return std::vector<DomNode>();
	}

	bool XPathObject::get_boolean() const
	{
		if (impl->type == type_boolean)
			return impl->boolean;
		else
			return false;
	}

	double XPathObject::get_number() const
	{
		if (impl->type == type_number)
			return impl->number;
		else
			return 0.0;
	}

	std::string XPathObject::get_string() const
	{
		if (impl->type == type_string)
			return impl->string;
		else
			return std::string();
	}

	void XPathObject::set_null()
	{
		impl->type = type_null;
		impl->node_set.clear();
	}

	void XPathObject::set_node_set(const std::vector<DomNode> &node_set)
	{
		impl->type = type_node_set;
		impl->node_set = node_set;
	}

	void XPathObject::set_boolean(bool value)
	{
		impl->type = type_boolean;
		impl->boolean = value;
		impl->node_set.clear();
	}

	void XPathObject::set_number(double value)
	{
		impl->type = type_number;
		impl->number = value;
		impl->node_set.clear();
	}

	void XPathObject::set_string(const std::string &str)
	{
		impl->type = type_string;
		impl->string = str;
		impl->node_set.clear();
	}
}
