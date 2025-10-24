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
#include "API/Core/XML/dom_character_data.h"
#include "API/Core/XML/dom_document.h"
#include "dom_node_generic.h"
#include "dom_document_generic.h"
#include "dom_tree_node.h"

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

CL_DomString CL_DomCharacterData::substring_data(unsigned long offset, unsigned long count)
{
	if (impl)
		return get_node_value().substr(offset, count);
	return CL_DomString();
}

void CL_DomCharacterData::append_data(const CL_DomString &arg)
{
	if (impl)
	{
		CL_DomDocument_Generic *doc = static_cast<CL_DomDocument_Generic *>(impl->owner_document.get());
		CL_DomString value = impl->get_tree_node()->get_node_value();
		CL_StringRef s = doc->string_allocator.alloc(value.length() + arg.length());
		memcpy(s.data(), value.data(), value.length() * sizeof(CL_DomString::char_type));
		memcpy(s.data() + value.length(), arg.data(), arg.length() * sizeof(CL_DomString::char_type));
		impl->get_tree_node()->node_value.str = s.data();
		impl->get_tree_node()->node_value.length = s.length();
	}
}

void CL_DomCharacterData::insert_data(unsigned long offset, const CL_DomString &arg)
{
	if (impl)
	{
		CL_DomDocument_Generic *doc = static_cast<CL_DomDocument_Generic *>(impl->owner_document.get());
		CL_DomString value = impl->get_tree_node()->get_node_value();
		if (offset > value.length())
			offset = value.length();
		CL_StringRef s = doc->string_allocator.alloc(value.length() + arg.length());
		memcpy(s.data(), value.data(), offset * sizeof(CL_DomString::char_type));
		memcpy(s.data() + offset, arg.data(), arg.length() * sizeof(CL_DomString::char_type));
		memcpy(s.data() + offset + arg.length(), value.data() + offset, (value.length()-offset) * sizeof(CL_DomString::char_type));
		impl->get_tree_node()->node_value.str = s.data();
		impl->get_tree_node()->node_value.length = s.length();
	}
}

void CL_DomCharacterData::delete_data(unsigned long offset, unsigned long count)
{
	if (impl)
	{
		CL_DomString value = impl->get_tree_node()->get_node_value();
		if (offset > value.length())
			offset = value.length();
		if (offset + count > value.length())
			count = value.length() - offset;
		if (count == 0)
			return;

		if (count < value.length())
		{
			memcpy(value.data() + offset, value.data() + offset + count, count * sizeof(CL_DomString::char_type));
			value.set_length(value.length() - count);
		}
		else
		{
			value = CL_DomString();
		}
		impl->get_tree_node()->node_value.str = value.data();
		impl->get_tree_node()->node_value.length = value.length();
	}
}

void CL_DomCharacterData::replace_data(unsigned long offset, unsigned long count, const CL_DomString &arg)
{
	delete_data(offset, count);
	insert_data(offset, arg);
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomCharacterData implementation:
