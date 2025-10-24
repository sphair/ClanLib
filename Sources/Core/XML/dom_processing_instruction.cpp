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
#include "API/Core/XML/dom_processing_instruction.h"
#include "API/Core/XML/dom_document.h"
#include "dom_node_generic.h"
#include "dom_tree_node.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DomProcessingInstruction construction:

CL_DomProcessingInstruction::CL_DomProcessingInstruction()
{
}

CL_DomProcessingInstruction::CL_DomProcessingInstruction(CL_DomDocument &doc, const CL_DomString &target, const CL_DomString &data)
: CL_DomNode(doc, PROCESSING_INSTRUCTION_NODE)
{
	CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) impl->owner_document.lock().get();
	impl->get_tree_node()->set_node_name(doc_impl, target);
	impl->get_tree_node()->set_node_value(doc_impl, data);
}

CL_DomProcessingInstruction::CL_DomProcessingInstruction(const CL_SharedPtr<CL_DomNode_Generic> &impl) : CL_DomNode(impl)
{
}

CL_DomProcessingInstruction::~CL_DomProcessingInstruction()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomProcessingInstruction attributes:

CL_DomString CL_DomProcessingInstruction::get_target() const
{
	if (impl)
		return impl->get_tree_node()->get_node_name();
	else
		return CL_DomString();
}

CL_DomString CL_DomProcessingInstruction::get_data() const
{
	if (impl)
		return impl->get_tree_node()->get_node_value();
	else
		return CL_DomString();
}

void CL_DomProcessingInstruction::set_data(const CL_DomString &data)
{
	if (impl)
	{
		CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) impl->owner_document.lock().get();
		impl->get_tree_node()->set_node_value(doc_impl, data);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomProcessingInstruction operations:

/////////////////////////////////////////////////////////////////////////////
// CL_DomProcessingInstruction implementation:
