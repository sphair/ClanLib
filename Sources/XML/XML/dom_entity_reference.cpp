/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
#include "API/XML/dom_entity_reference.h"
#include "API/XML/dom_document.h"
#include "dom_node_generic.h"
#include "dom_tree_node.h"

namespace clan
{
	DomEntityReference::DomEntityReference()
	{
	}

	DomEntityReference::DomEntityReference(DomDocument &doc, const DomString &name)
		: DomNode(doc, ENTITY_REFERENCE_NODE)
	{
		DomDocument_Impl *doc_impl = (DomDocument_Impl *)impl->owner_document.lock().get();
		impl->get_tree_node()->set_node_name(doc_impl, name);
	}

	DomEntityReference::DomEntityReference(const std::shared_ptr<DomNode_Impl> &impl) : DomNode(impl)
	{
	}

	DomEntityReference::~DomEntityReference()
	{
	}
}
