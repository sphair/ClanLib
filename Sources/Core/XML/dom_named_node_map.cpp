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
#include "API/Core/System/cl_assert.h"
#include "Core/XML/dom_named_node_map_generic.h"
#include "API/Core/XML/dom_named_node_map.h"
#include "API/Core/XML/dom_attr.h"
#include "API/Core/XML/dom_node.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DomNamedNodeMap construction:

CL_DomNamedNodeMap::CL_DomNamedNodeMap()
{
	impl = new CL_DomNamedNodeMap_Generic();
}

CL_DomNamedNodeMap::CL_DomNamedNodeMap(CL_DomNode &node)
{
	// FIXME: Not sure what this should do
	cl_assert(0);
}

CL_DomNamedNodeMap::~CL_DomNamedNodeMap()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomNamedNodeMap attributes:

int CL_DomNamedNodeMap::get_length() const
{
	return impl->attributes.size();
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomNamedNodeMap operations:

CL_DomNode CL_DomNamedNodeMap::get_named_item(const std::string &name) const
{
	int size = impl->attributes.size();
	for (int i=0; i<size; i++)
	{
		if (impl->attributes[i].first == name)
		{
			return CL_DomNode(impl->attributes[i].second);
		}
	}
	return CL_DomNode();
}

CL_DomNode CL_DomNamedNodeMap::set_named_item(const CL_DomNode &node)
{
	int size = impl->attributes.size();
	for (int i=0; i<size; i++)
	{
		if (impl->attributes[i].first == node.to_attr().get_name())
		{
			CL_DomNode oldnode(impl->attributes[i].second);
			impl->attributes[i].second = node;
			return oldnode;
		}
	}
	
	impl->attributes.push_back(std::pair<std::string, CL_DomNode>(node.to_attr().get_name(), node));

	return CL_DomNode();
}

CL_DomNode CL_DomNamedNodeMap::remove_named_item(const std::string &name)
{
	// FIXME: Implement me
	return CL_DomNode();
}

CL_DomNode CL_DomNamedNodeMap::item(unsigned long index) const
{
	return CL_DomNode(impl->attributes[index].second);
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomNamedNodeMap implementation:
