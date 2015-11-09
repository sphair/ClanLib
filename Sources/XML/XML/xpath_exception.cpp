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
**    Thomas Gottschalk Larsen
*/

#include "XML/precomp.h"
#include "API/Core/Text/string_format.h"
#include "API/XML/xpath_exception.h"
#include "xpath_token.h"

namespace clan
{
	XPathException::XPathException(const std::string &message, const std::string &expression)
		: Exception(message), expression(expression), error_position(std::string::npos)
	{
	}

	XPathException::XPathException(const std::string &message, const std::string &expression, const XPathToken &cur_token)
		: Exception(message), expression(expression), error_position(cur_token.pos)
	{
	}

	std::string XPathException::get_message() const
	{
		std::string result = "Syntax error in XPath expression";
		if (error_position != std::string::npos)
			result += string_format(" at position %1.", error_position);
		else
			result += ".";

		if (!message.empty())
			result += " " + message + ".";

		if (!expression.empty() && error_position != std::string::npos)
		{
			std::string exp = "Expression: ";
			std::string::size_type start_pos = exp.length();
			exp += expression;

			std::string failure_location(start_pos + error_position, '-');
			failure_location += "^";

			result += string_format("\n%1\n%2", exp, failure_location);
		}
		return result;
	}
}
