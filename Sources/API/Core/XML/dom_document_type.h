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

#ifndef header_dom_document_type
#define header_dom_document_type

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

class CL_DomNamedNodeMap;

//: DOM Document Type class.
//- !group=Core/XML!
//- !header=core.h!
//- <p>Each Document has a doctype attribute whose value is either null
//- or a DocumentType object. The DocumentType interface in the DOM Level 1
//- Core provides an interface to the list of entities that are defined for
//- the document, and little else because the effect of namespaces and the
//- various XML scheme efforts on DTD representation are not clearly understood
//- as of this writing.</p>
//- <p>The DOM Level 1 doesn't support editing DocumentType nodes.</p>
class CL_API_CORE CL_DomDocumentType : public CL_DomNode
{
//! Construction:
public:
	//: Constructs a DOM Document Type handle.
	CL_DomDocumentType();

	CL_DomDocumentType(CL_DomDocument &doc);
	
	CL_DomDocumentType(const CL_SharedPtr<CL_DomNode_Generic> &impl);

	~CL_DomDocumentType();

//! Attributes:
public:
	//: The name of DTD; i.e., the name immediately following the DOCTYPE keyword.
	std::string get_name() const;

	//: A NamedNodeMap containing the general entities, both external and internal, declared in the DTD.
	CL_DomNamedNodeMap get_entities() const;

	//: A NamedNodeMap containing the notations declared in the DTD.
	CL_DomNamedNodeMap get_notations() const;

//! Operations:
public:
};

#endif
