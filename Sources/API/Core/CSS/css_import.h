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

class CL_CSSRuleSet;
class CL_CSSImport_Impl;

/// \brief Cascading Style Sheet (CSS) import directive.
/** <p>CSS import directives specify other CSS documents with further CSS
    rule sets.</p>
    !group=Core/CSS! !header=core.h!*/
class CL_API_CORE CL_CSSImport
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a CSS import directive.
	CL_CSSImport();

	~CL_CSSImport();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the value of the import directive.
	CL_String &get_value();


/// \}
/// \name Operations
/// \{

public:
	/// \brief Sets the value of the import directive.
	void set_value(const CL_String &value);

	bool operator ==(const CL_CSSImport &other) const;


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_CSSImport_Impl> impl;
/// \}
};


/// \}
