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

//! clanCore="XML"
//! header=core.h

#ifndef header_dom_document_fragment
#define header_dom_document_fragment

#ifdef CL_API_DLL
#ifdef CL_CORE_EXPORT
#define CL_API_CORE __declspec(dllexport)
#else
#define CL_API_CORE __declspec(dllimport)
#endif
#else
#define CL_API_CORE
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "dom_node.h"

//: DOM Document Fragment class.
//- !group=Core/XML!
//- !header=core.h!
//- <p>DocumentFragment is a "lightweight" or "minimal" Document object. It
//- is very common to want to be able to extract a portion of a document's
//- tree or to create a new fragment of a document. Imagine implementing a
//- user command like cut or rearranging a document by moving fragments
//- around. It is desirable to have an object which can hold such fragments
//- and it is quite natural to use a Node for this purpose. While it is true
//- that a Document object could fulfil this role, a Document object can
//- potentially be a heavyweight object, depending on the underlying
//- implementation. What is really needed for this is a very lightweight
//- object. DocumentFragment is such an object.</p>
//- <p>Furthermore, various operations -- such as inserting nodes as children
//- of another Node -- may take DocumentFragment objects as arguments; this
//- results in all the child nodes of the DocumentFragment being moved to the
//- child list of this node.</p>
//- <p>The children of a DocumentFragment node are zero or more nodes
//- representing the tops of any sub-trees defining the structure of the
//- document. DocumentFragment nodes do not need to be well-formed XML documents
//- (although they do need to follow the rules imposed upon well-formed XML
//- parsed entities, which can have multiple top nodes). For example, a
//- DocumentFragment might have only one child and that child node could be
//- a Text node. Such a structure model represents neither an HTML document
//- nor a well-formed XML document.</p>
//- <p>When a DocumentFragment is inserted into a Document (or indeed any
//- other Node that may take children) the children of the DocumentFragment
//- and not the DocumentFragment itself are inserted into the Node. This
//- makes the DocumentFragment very useful when the user wishes to create
//- nodes that are siblings; the DocumentFragment acts as the parent of these
//- nodes so that the user can use the standard methods from the Node interface,
//- such as insertBefore() and appendChild().</p>
class CL_API_CORE CL_DomDocumentFragment : public CL_DomNode
{
//! Construction:
public:
	//: Constructs a DOM Document Fragment handle.
	CL_DomDocumentFragment();
	
	CL_DomDocumentFragment(CL_DomDocument &doc);

	CL_DomDocumentFragment(const CL_SharedPtr<CL_DomNode_Generic> &impl);

	~CL_DomDocumentFragment();
};

#endif
