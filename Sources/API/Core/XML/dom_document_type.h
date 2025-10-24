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

/// \addtogroup clanCore_XML clanCore XML
/// \{

#pragma once

#include "../api_core.h"
#include "dom_node.h"

class CL_DomNamedNodeMap;

/// \brief DOM Document Type class.
///
///    <p>Each Document has a doctype attribute whose value is either null
///    or a DocumentType object. The DocumentType interface in the DOM Level 1
///    Core provides an interface to the list of entities that are defined for
///    the document, and little else because the effect of namespaces and the
///    various XML scheme efforts on DTD representation are not clearly understood
///    as of this writing.</p>
///    <p>The DOM Level 1 doesn't support editing DocumentType nodes.</p> 
/// \xmlonly !group=Core/XML! !header=core.h! \endxmlonly
class CL_API_CORE CL_DomDocumentType : public CL_DomNode
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a DOM Document Type handle.
	CL_DomDocumentType();

	CL_DomDocumentType(
		const CL_DomString &qualified_name,
		const CL_DomString &public_id,
		const CL_DomString &system_id);

	/// \brief Constructs a DomDocumentType
	///
	/// \param doc = Dom Document
	CL_DomDocumentType(CL_DomDocument &doc);

	/// \brief Constructs a DomDocumentType
	///
	/// \param impl = Shared Ptr
	CL_DomDocumentType(const CL_SharedPtr<CL_DomNode_Generic> &impl);

	~CL_DomDocumentType();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief The name of DTD; i.e., the name immediately following the DOCTYPE keyword.
	CL_DomString get_name() const;

	/// \brief A NamedNodeMap containing the general entities, both external and internal, declared in the DTD.
	CL_DomNamedNodeMap get_entities() const;

	/// \brief A NamedNodeMap containing the notations declared in the DTD.
	CL_DomNamedNodeMap get_notations() const;

	/// \brief The public identifier of the external subset.
	CL_DomString get_public_id() const;

	/// \brief The system identifier of the external subset.
	CL_DomString get_system_id() const;

	/// \brief The internal subset as a string.
	/** <p><b>Note:</b> The actual content returned depends on how much
	    information is available to the implementation. This may vary
	    depending on various parameters, including the XML processor
	    used to build the document.</p>*/
	CL_DomString get_internal_subset() const;

/// \}
/// \name Operations
/// \{

public:
/// \}
};

/// \}
