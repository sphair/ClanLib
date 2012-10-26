/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
#include "API/CSSLayout/css_tokenizer.h"
#include "API/CSSLayout/css_token.h"
#include "css_tokenizer_impl.h"

namespace clan
{

CSSTokenizer::CSSTokenizer(IODevice &device)
: impl(new CSSTokenizer_Impl(device))
{
}

CSSTokenizer::CSSTokenizer(const std::string &text)
: impl(new CSSTokenizer_Impl(text))
{
}

void CSSTokenizer::read(CSSToken &token, bool eat_whitespace, bool eat_comments)
{
	do
	{
		impl->read(token);
		if (eat_comments && (token.type == CSSToken::type_whitespace || token.type == CSSToken::type_comment))
		{
			if (token.type == CSSToken::type_comment)
			{
				token.type = CSSToken::type_whitespace;
				token.value = " ";
			}
			CSSToken next_token;
			while (true)
			{
				impl->peek(next_token);
				if (next_token.type != CSSToken::type_whitespace && next_token.type != CSSToken::type_comment)
					break;
				impl->read(next_token);
				if (next_token.type == CSSToken::type_comment)
				{
					next_token.type = CSSToken::type_whitespace;
					next_token.value = " ";
				}
				token.value += next_token.value;
			}
		}
	} while((eat_whitespace && token.type == CSSToken::type_whitespace));
}

}
