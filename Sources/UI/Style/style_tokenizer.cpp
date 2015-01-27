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
*/

#include "UI/precomp.h"
#include "API/UI/Style/style_tokenizer.h"
#include "API/UI/Style/style_token.h"
#include "style_tokenizer_impl.h"

namespace clan
{
	StyleTokenizer::StyleTokenizer(const std::string &text)
	: impl(std::make_shared<StyleTokenizer_Impl>(text))
	{
	}

	void StyleTokenizer::read(StyleToken &token, bool eat_whitespace, bool eat_comments)
	{
		do
		{
			impl->read(token);
			if (eat_comments && (token.type == StyleTokenType::whitespace || token.type == StyleTokenType::comment))
			{
				if (token.type == StyleTokenType::comment)
				{
					token.type = StyleTokenType::whitespace;
					token.value = " ";
				}
				StyleToken next_token;
				while (true)
				{
					impl->peek(next_token);
					if (next_token.type != StyleTokenType::whitespace && next_token.type != StyleTokenType::comment)
						break;
					impl->read(next_token);
					if (next_token.type == StyleTokenType::comment)
					{
						next_token.type = StyleTokenType::whitespace;
						next_token.value = " ";
					}
					token.value += next_token.value;
				}
			}
		} while((eat_whitespace && token.type == StyleTokenType::whitespace));
	}

	std::vector<StyleToken> StyleTokenizer::tokenize(const std::string &text)
	{
		std::vector<StyleToken> tokens;
		StyleTokenizer tokenizer(text);
		StyleToken token;
		bool first = true;
		while (true)
		{
			tokenizer.read(token, false);
			if (token.type == StyleTokenType::null)
				break;
			if (!first || token.type != StyleTokenType::whitespace)
			{
				first = false;
				tokens.push_back(token);
			}
		}
		while (!tokens.empty() && tokens.back().type == StyleTokenType::whitespace)
			tokens.pop_back();
		return tokens;
	}
}
