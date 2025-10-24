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

#include "CSSLayout/precomp.h"
#include "API/CSSLayout/css_property2.h"

/////////////////////////////////////////////////////////////////////////////

class CL_CSSProperty2_Impl
{
public:
	CL_CSSProperty2_Impl() : important(false) { }

	CL_String name;
	std::vector<CL_CSSToken> value_tokens;
	bool important;
};

/////////////////////////////////////////////////////////////////////////////

CL_CSSProperty2::CL_CSSProperty2()
: impl(new CL_CSSProperty2_Impl())
{
}

void CL_CSSProperty2::set_name(const CL_String &name)
{
	impl->name = name;
}

const CL_String &CL_CSSProperty2::get_name() const
{
	return impl->name;
}

std::vector<CL_CSSToken> &CL_CSSProperty2::get_value_tokens()
{
	return impl->value_tokens;
}

const std::vector<CL_CSSToken> &CL_CSSProperty2::get_value_tokens() const
{
	return impl->value_tokens;
}

void CL_CSSProperty2::set_value_tokens(const std::vector<CL_CSSToken> &tokens)
{
	impl->value_tokens = tokens;
}

bool CL_CSSProperty2::is_important() const
{
	return impl->important;
}

void CL_CSSProperty2::set_important(bool flag)
{
	impl->important = flag;
}
