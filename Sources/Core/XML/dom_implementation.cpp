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
#include "API/Core/XML/dom_implementation.h"
#include "API/Core/XML/dom_document.h"
#include "API/Core/XML/dom_document_type.h"
#include "API/Core/Text/string_help.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DomImplementation construction:

CL_DomImplementation::CL_DomImplementation()
{
}

CL_DomImplementation::CL_DomImplementation(CL_DomDocument &doc)
{
	throw CL_Exception("FIXME: Create impl and handle doc"); 
}

CL_DomImplementation::~CL_DomImplementation()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomImplementation attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_DomImplementation operations:

bool CL_DomImplementation::has_feature(
	const CL_DomString &feature,
	const CL_DomString &version)
{
	if (CL_StringHelp::compare(feature, "xml") == 0 && (version == "1.0" || version.empty()))
		return true;
	if (CL_StringHelp::compare(feature, "xml") == 0 && (version == "2.0" || version.empty()))
		return true;
	return false;
}

CL_DomDocumentType CL_DomImplementation::create_document_type(
	const CL_DomString &qualified_name,
	const CL_DomString &public_id,
	const CL_DomString &system_id)
{
	return CL_DomDocumentType(qualified_name, public_id, system_id);
}

CL_DomDocument CL_DomImplementation::create_document(
	const CL_DomString &namespace_uri,
	const CL_DomString &qualified_name,
	const CL_DomDocumentType &doctype)
{
	return CL_DomDocument(namespace_uri, qualified_name, doctype);
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomImplementation implementation:
