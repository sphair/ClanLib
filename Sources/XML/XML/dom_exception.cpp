/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
#include "API/XML/dom_exception.h"
#include "API/Core/Text/string_format.h"

namespace clan
{
	static std::string code_to_message(unsigned short code)
	{
		switch (code)
		{
		case DomException::DOMSTRING_SIZE_ERR:
			return "DOMSTRING_SIZE_ERR: Specified range of text does not fit into a DOMString";

		case DomException::HIERARCHY_REQUEST_ERR:
			return "HIERARCHY_REQUEST_ERR: Attemped to insert node where it doesn't belong";

		case DomException::INDEX_SIZE_ERR:
			return "INDEX_SIZE_ERR: Index or size is negative, or greater than the allowed value";

		case DomException::INUSE_ATTRIBUTE_ERR:
			return "INUSE_ATTRIBUTE_ERR: An attempt was made to add an attribute that is already in use elsewhere";

		case DomException::INVALID_ACCESS_ERR:
			return "INVALID_ACCESS_ERR: A parameter or an operation is not supported by the underlying object.";

		case DomException::INVALID_CHARACTER_ERR:
			return "INVALID_CHARACTER_ERR: An invalid or illegal character was specified, such as in a name";

		case DomException::INVALID_MODIFICATION_ERR:
			return "INVALID_MODIFICATION_ERR: An attempt is made to modify the type of the underlying object.";

		case DomException::INVALID_STATE_ERR:
			return "INVALID_STATE_ERR: An attempt is made to use an object that is not, or is no longer, usable.";

		case DomException::NAMESPACE_ERR:
			return "NAMESPACE_ERR: An attempt is made to create or change an object in a way which is incorrect with regard to namespaces.";

		case DomException::NOT_FOUND_ERR:
			return "NOT_FOUND_ERR: An attempt was made to reference a node in a context where it does not exist";

		case DomException::NOT_SUPPORTED_ERR:
			return "NOT_SUPPORTED_ERR: The implementation does not support the type of object requested";

		case DomException::NO_DATA_ALLOWED_ERR:
			return "NO_DATA_ALLOWED_ERR: Data was specified for a node which does not support data";

		case DomException::NO_MODIFICATION_ALLOWED_ERR:
			return "NO_MODIFICATION_ALLOWED_ERR: An attempt was made to modify an object where modifications are not allowed";

		case DomException::SYNTAX_ERR:
			return "SYNTAX_ERR: An invalid or illegal string is specified.";

		case DomException::WRONG_DOCUMENT_ERR:
			return "WRONG_DOCUMENT_ERR: A node was used in a different document than the one that created it";
		}

		return string_format("Unknown DOM exception code %1", code);
	}

	DomException::DomException(unsigned short code) : Exception(code_to_message(code)), code(code)
	{
	}

	DomException::DomException(const std::string &message, unsigned short code) : Exception(message), code(code)
	{
	}
}
