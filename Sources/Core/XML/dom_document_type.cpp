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

#include "Core/precomp.h"
#include "API/Core/XML/dom_document_type.h"
#include "API/Core/XML/dom_document.h"
#include "API/Core/XML/dom_named_node_map.h"
#include "dom_node_generic.h"
#include "dom_document_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DomDocumentType construction:

CL_DomDocumentType::CL_DomDocumentType()
{
}

CL_DomDocumentType::CL_DomDocumentType(
	const CL_DomString &qualified_name,
	const CL_DomString &public_id,
	const CL_DomString &system_id)
: CL_DomNode(CL_DomDocument(), DOCUMENT_TYPE_NODE)
{
	CL_DomDocument_Generic *doc = dynamic_cast<CL_DomDocument_Generic *>(impl->owner_document.get());
	doc->qualified_name = qualified_name;
	doc->public_id = public_id;
	doc->system_id = system_id;
}

CL_DomDocumentType::CL_DomDocumentType(CL_DomDocument &doc)
: CL_DomNode(doc, DOCUMENT_TYPE_NODE)
{
}

CL_DomDocumentType::CL_DomDocumentType(const CL_SharedPtr<CL_DomNode_Generic> &impl) : CL_DomNode(impl)
{
}
	
CL_DomDocumentType::~CL_DomDocumentType()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomDocumentType attributes:

CL_DomString CL_DomDocumentType::get_name() const
{
	if (impl)
	{
		const CL_DomDocument_Generic *doc = dynamic_cast<const CL_DomDocument_Generic *>(impl->owner_document.get());
		if (doc)
			return doc->qualified_name;
	}
	return CL_DomString();
}
	
CL_DomNamedNodeMap CL_DomDocumentType::get_entities() const
{
	return CL_DomNamedNodeMap();
}
	
CL_DomNamedNodeMap CL_DomDocumentType::get_notations() const
{
	return CL_DomNamedNodeMap();
}

CL_DomString CL_DomDocumentType::get_public_id() const
{
	if (impl)
	{
		const CL_DomDocument_Generic *doc = dynamic_cast<const CL_DomDocument_Generic *>(impl->owner_document.get());
		if (doc)
			return doc->public_id;
	}
	return CL_DomString();
}

CL_DomString CL_DomDocumentType::get_system_id() const
{
	if (impl)
	{
		const CL_DomDocument_Generic *doc = dynamic_cast<const CL_DomDocument_Generic *>(impl->owner_document.get());
		if (doc)
			return doc->system_id;
	}
	return CL_DomString();
}

CL_DomString CL_DomDocumentType::get_internal_subset() const
{
	if (impl)
	{
		const CL_DomDocument_Generic *doc = dynamic_cast<const CL_DomDocument_Generic *>(impl->owner_document.get());
		if (doc)
			return doc->internal_subset;
	}
	return CL_DomString();
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomDocumentType operations:

/////////////////////////////////////////////////////////////////////////////
// CL_DomDocumentType implementation:
