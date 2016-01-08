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
#include "API/XML/dom_implementation.h"
#include "API/XML/dom_document.h"
#include "API/XML/dom_document_type.h"
#include "API/Core/Text/string_help.h"

namespace clan
{
	DomImplementation::DomImplementation()
	{
	}

	DomImplementation::DomImplementation(DomDocument &doc)
	{
		throw Exception("FIXME: Create impl and handle doc");
	}

	DomImplementation::~DomImplementation()
	{
	}

	bool DomImplementation::has_feature(
		const DomString &feature,
		const DomString &version)
	{
		if (StringHelp::compare(feature, "xml") == 0 && (version == "1.0" || version.empty()))
			return true;
		if (StringHelp::compare(feature, "xml") == 0 && (version == "2.0" || version.empty()))
			return true;
		return false;
	}

	DomDocumentType DomImplementation::create_document_type(
		const DomString &qualified_name,
		const DomString &public_id,
		const DomString &system_id)
	{
		return DomDocumentType(qualified_name, public_id, system_id);
	}

	DomDocument DomImplementation::create_document(
		const DomString &namespace_uri,
		const DomString &qualified_name,
		const DomDocumentType &doctype)
	{
		return DomDocument(namespace_uri, qualified_name, doctype);
	}
}
