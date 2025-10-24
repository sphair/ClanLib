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

#include "Core/precomp.h"
#include "API/Core/CSS/css_tokenizer.h"
#include "API/Core/CSS/css_token.h"
#include "css_tokenizer_impl.h"

CL_CSSTokenizer::CL_CSSTokenizer(CL_IODevice &device)
: impl(new CL_CSSTokenizer_Impl(device))
{
}

CL_CSSTokenizer::CL_CSSTokenizer(const CL_String &text)
: impl(new CL_CSSTokenizer_Impl(text))
{
}

void CL_CSSTokenizer::read(CL_CSSToken &token, bool eat_whitespace)
{
	do
	{
		impl->read(token);
	} while(eat_whitespace && token.type == CL_CSSToken::type_whitespace);
}
