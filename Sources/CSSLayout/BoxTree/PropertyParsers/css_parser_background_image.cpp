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

#include "CSSLayout/precomp.h"
#include "css_parser_background_image.h"
#include "API/CSSLayout/css_box_properties.h"

std::vector<CL_String> CL_CSSParserBackgroundImage::get_names()
{
	std::vector<CL_String> names;
	names.push_back("background-image");
	return names;
}

void CL_CSSParserBackgroundImage::parse(CL_CSSBoxProperties &properties, const CL_String &name, const std::vector<CL_CSSToken> &tokens, std::map<CL_String, CL_CSSBoxProperty *> *out_change_set)
{
	size_t pos = 0;
	CL_CSSToken token = next_token(pos, tokens);

	CL_CSSBoxBackgroundImage background_image;

	if (token.type == CL_CSSToken::type_ident && pos == tokens.size() && equals(token.value, "inherit"))
	{
		background_image.type = CL_CSSBoxBackgroundImage::type_inherit;
	}
	else
	{
		background_image.type = CL_CSSBoxBackgroundImage::type_images;
		background_image.images.clear();
		while (true)
		{
			if (token.type == CL_CSSToken::type_ident && equals(token.value, "none"))
				background_image.images.push_back(CL_CSSBoxBackgroundImage::Image(CL_CSSBoxBackgroundImage::image_type_none));
			else if (token.type == CL_CSSToken::type_uri)
				background_image.images.push_back(CL_CSSBoxBackgroundImage::Image(CL_CSSBoxBackgroundImage::image_type_uri, token.value));
			else
				return;

			if (pos != tokens.size())
			{
				token = next_token(pos, tokens);
				if (token.type != CL_CSSToken::type_delim || token.value != ",")
					return;
				token = next_token(pos, tokens);
			}
			else
			{
				break;
			}
		}
	}

	properties.background_image = background_image;
	if (out_change_set)
	{
		(*out_change_set)["background-image"] = &properties.background_image;
	}
}
