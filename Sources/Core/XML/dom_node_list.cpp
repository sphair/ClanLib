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
#include "API/Core/XML/dom_node_list.h"
#include "API/Core/XML/dom_node.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DomNodeList construction:

CL_DomNodeList::CL_DomNodeList()
{
}

CL_DomNodeList::CL_DomNodeList(CL_DomNode &node, const std::string &tag_name)
{
	CL_DomNode current_child=node.get_first_child();
	while(!current_child.is_null())
	{
		if(current_child.get_node_name()==tag_name)
		{
			add_item(current_child);
		}
		current_child=current_child.get_next_sibling();
	}
}

CL_DomNodeList::~CL_DomNodeList()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomNodeList attributes:

int CL_DomNodeList::get_length() const
{
	return (node_list.size());
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomNodeList operations:

CL_DomNode CL_DomNodeList::item(unsigned long index) const
{
	if( index < 0 || index >= node_list.size() )
	{//if the given index is not valid
		CL_DomNode empty_node;
		return(empty_node);
	}
	std::vector<CL_DomNode>::const_iterator i=node_list.begin()+index;
	return(*i);
}

void CL_DomNodeList::add_item(CL_DomNode &to_add)
{
	node_list.push_back(to_add);
}
/////////////////////////////////////////////////////////////////////////////
// CL_DomNodeList implementation:
