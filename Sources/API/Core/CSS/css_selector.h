/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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


#if _MSC_VER > 1000
#pragma once
#endif

#include "../api_core.h"
#include "../System/sharedptr.h"
#include <vector>

class CL_CSSSelector_Impl;

/// \brief Cascading Style Sheet (CSS) selector.
/** <p>CSS selectors are used to determine whether a CSS rule set apply or
    not.</p>
    !group=Core/CSS! !header=core.h!*/
class CL_API_CORE CL_CSSSelector
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a CSS selector.
	CL_CSSSelector();

	~CL_CSSSelector();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the path elements required to match the selector.
	const std::vector<CL_String> &get_path_elements() const;


/// \}
/// \name Operations
/// \{

public:
	/// \brief Returns true if selector matches a given element path.
	bool select(const std::vector<CL_StringRef> &element, int &out_specificity) const;

	/// \brief Adds another path element to selector.
	void add_path_element(const CL_String &element);

	bool operator ==(const CL_CSSSelector &other) const;


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_CSSSelector_Impl> impl;
/// \}
};


/// \}
