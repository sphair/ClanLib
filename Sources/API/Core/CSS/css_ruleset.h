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

/// \addtogroup clanCore_CSS clanCore CSS
/// \{

#pragma once

#include "../api_core.h"
#include "../System/sharedptr.h"
#include <vector>

class CL_CSSSelector;
class CL_CSSProperty;
class CL_CSSRuleSet_Impl;

/// \brief Cascading Style Sheet (CSS) rule set.
/** <p>A CSS rule set is a container for CSS properties and CSS selectors
    applying to a rule set.</p>
    !group=Core/CSS! !header=core.h!*/
class CL_API_CORE CL_CSSRuleSet
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a CSS rule set.
	CL_CSSRuleSet();

	~CL_CSSRuleSet();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns all CSS selectors defined for this rule set.
	std::vector<CL_CSSSelector> &get_selectors();

	/// \brief Returns all CSS properties for this rule set.
	std::vector<CL_CSSProperty> &get_properties();

/// \}
/// \name Operations
/// \{

public:
	bool operator ==(const CL_CSSRuleSet &other) const;

	/// \brief Adds a CSS selector to the rule set.
	void add_selector(const CL_CSSSelector &selector);

	/// \brief Adds a CSS property to the rule set.
	void add_property(const CL_CSSProperty &property);

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_CSSRuleSet_Impl> impl;
/// \}
};

/// \}
