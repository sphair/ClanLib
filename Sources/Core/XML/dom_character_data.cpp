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
#include "API/Core/XML/dom_character_data.h"
#include "dom_node_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DomCharacterData construction:

CL_DomCharacterData::CL_DomCharacterData()
{
}

CL_DomCharacterData::CL_DomCharacterData(const CL_SharedPtr<CL_DomNode_Generic> &impl) : CL_DomNode(impl)
{
}

CL_DomCharacterData::CL_DomCharacterData(CL_DomDocument &doc, unsigned short node_type)
: CL_DomNode(doc, node_type)
{
}

CL_DomCharacterData::~CL_DomCharacterData()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomCharacterData attributes:

unsigned long CL_DomCharacterData::get_length()
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomCharacterData operations:

std::string CL_DomCharacterData::substring_data(unsigned long offset, unsigned long count)
{
	if (impl) return impl->node_value.substr(offset, count);
	return std::string();
}

void CL_DomCharacterData::append_data(const std::string &arg)
{
	if (impl) impl->node_value.append(arg);
}

void CL_DomCharacterData::insert_data(unsigned long offset, const std::string &arg)
{
	if (impl) impl->node_value.insert(offset, arg);
}

void CL_DomCharacterData::delete_data(unsigned long offset, unsigned long count)
{
	if (impl) impl->node_value.erase(offset, count);
}

void CL_DomCharacterData::replace_data(unsigned long offset, unsigned long count, const std::string &arg)
{
	if (impl) impl->node_value.replace(offset, count, arg);
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomCharacterData implementation:
