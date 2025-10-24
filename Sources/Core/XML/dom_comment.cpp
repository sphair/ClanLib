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
#include "API/Core/XML/dom_comment.h"
#include "dom_node_generic.h"
#include "dom_tree_node.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DomComment construction:

CL_DomComment::CL_DomComment()
{
}

CL_DomComment::CL_DomComment(CL_DomDocument &doc, const CL_DomString &data)
: CL_DomCharacterData(doc, COMMENT_NODE)
{
	CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) impl->owner_document.get();
	impl->get_tree_node()->set_node_value(doc_impl, data);
}

CL_DomComment::CL_DomComment(const CL_SharedPtr<CL_DomNode_Generic> &impl) : CL_DomCharacterData(impl)
{
}

CL_DomComment::~CL_DomComment()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomComment attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_DomComment operations:

/////////////////////////////////////////////////////////////////////////////
// CL_DomComment implementation:
