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
*/

#include "XML/precomp.h"
#include "API/XML/dom_document_type.h"
#include "API/XML/dom_document.h"
#include "API/XML/dom_named_node_map.h"
#include "dom_node_generic.h"
#include "dom_document_generic.h"

namespace clan
{
	DomDocumentType::DomDocumentType()
	{
	}

	DomDocumentType::DomDocumentType(
		const DomString &qualified_name,
		const DomString &public_id,
		const DomString &system_id)
		: DomNode(DomDocument(), DOCUMENT_TYPE_NODE)
	{
		DomDocument_Impl *doc = dynamic_cast<DomDocument_Impl *>(impl->owner_document.lock().get());
		doc->qualified_name = qualified_name;
		doc->public_id = public_id;
		doc->system_id = system_id;
	}

	DomDocumentType::DomDocumentType(DomDocument &doc)
		: DomNode(doc, DOCUMENT_TYPE_NODE)
	{
	}

	DomDocumentType::DomDocumentType(const std::shared_ptr<DomNode_Impl> &impl) : DomNode(impl)
	{
	}

	DomDocumentType::~DomDocumentType()
	{
	}

	DomString DomDocumentType::get_name() const
	{
		if (impl)
		{
			const DomDocument_Impl *doc = dynamic_cast<const DomDocument_Impl *>(impl->owner_document.lock().get());
			if (doc)
				return doc->qualified_name;
		}
		return DomString();
	}

	DomNamedNodeMap DomDocumentType::get_entities() const
	{
		return DomNamedNodeMap();
	}

	DomNamedNodeMap DomDocumentType::get_notations() const
	{
		return DomNamedNodeMap();
	}

	DomString DomDocumentType::get_public_id() const
	{
		if (impl)
		{
			const DomDocument_Impl *doc = dynamic_cast<const DomDocument_Impl *>(impl->owner_document.lock().get());
			if (doc)
				return doc->public_id;
		}
		return DomString();
	}

	DomString DomDocumentType::get_system_id() const
	{
		if (impl)
		{
			const DomDocument_Impl *doc = dynamic_cast<const DomDocument_Impl *>(impl->owner_document.lock().get());
			if (doc)
				return doc->system_id;
		}
		return DomString();
	}

	DomString DomDocumentType::get_internal_subset() const
	{
		if (impl)
		{
			const DomDocument_Impl *doc = dynamic_cast<const DomDocument_Impl *>(impl->owner_document.lock().get());
			if (doc)
				return doc->internal_subset;
		}
		return DomString();
	}
}
