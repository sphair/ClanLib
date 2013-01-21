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

#pragma once

#include "../api_csslayout.h"

namespace clan
{

class CSSSelectResult;
class CSSStyleProperties;
class CSSPropertyValue;
class CSSComputedBox;
class CSSComputedValues_Impl;
class CSSResourceCache;

class CL_API_CSSLAYOUT CSSComputedValues
{
public:
	CSSComputedValues();
	CSSComputedValues(CSSResourceCache *resource_cache);

	bool is_null() const;

	void set_parent(const CSSComputedValues &parent);
	void set_specified_values(const CSSSelectResult &selected_values);
	void set_specified_values(const CSSSelectResult &selected_values, const std::string &style_values);
	void set_specified_values(const CSSSelectResult &selected_values, const CSSStyleProperties &style_values);

	const CSSComputedBox &get_box() const;
	int get_box_generation() const;

private:
	std::shared_ptr<CSSComputedValues_Impl> impl;
};

}
