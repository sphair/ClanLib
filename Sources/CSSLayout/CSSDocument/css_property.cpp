/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
#include "API/CSSLayout/CSSDocument/css_property.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////

class CSSProperty_Impl
{
public:
	CSSProperty_Impl() : important(false) { }

	std::string name;
	std::vector<CSSToken> value_tokens;
	bool important;
};

/////////////////////////////////////////////////////////////////////////////

CSSProperty::CSSProperty()
: impl(new CSSProperty_Impl())
{
}

void CSSProperty::set_name(const std::string &name)
{
	impl->name = name;
}

const std::string &CSSProperty::get_name() const
{
	return impl->name;
}

std::vector<CSSToken> &CSSProperty::get_value_tokens()
{
	return impl->value_tokens;
}

const std::vector<CSSToken> &CSSProperty::get_value_tokens() const
{
	return impl->value_tokens;
}

void CSSProperty::set_value_tokens(const std::vector<CSSToken> &tokens)
{
	impl->value_tokens = tokens;
}

bool CSSProperty::is_important() const
{
	return impl->important;
}

void CSSProperty::set_important(bool flag)
{
	impl->important = flag;
}

}
