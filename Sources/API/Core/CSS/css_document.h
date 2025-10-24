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

/// \addtogroup clanCore_CSS clanCore CSS
/// \{

#pragma once

#include "../api_core.h"
#include "../System/sharedptr.h"
#include <vector>

class CL_IODevice;
class CL_VirtualFileSystem;
class CL_CSSProperty;
class CL_CSSDocument_Impl;
class CL_VirtualDirectory;

/// \brief Cascading Style Sheet (CSS) document.
/// Document class for cascading style sheets (CSS).
/// !group=Core/CSS! !header=core.h!
class CL_API_CORE CL_CSSDocument
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a cascading style sheet document.
	CL_CSSDocument();
	~CL_CSSDocument();
/// \}

/// \name Operations
/// \{
public:
	/// \brief Find rule sets that match a given element.
	/** <p>The syntax of element is the same as specifying contextual elements
	    in a CSS selector, i.e. 'body div.class a#id:link'.</p>
	    <p>The returned list is sorted with the most preferred properties returned
	    first in the list.</p>*/
	std::vector<CL_CSSProperty> select(const CL_StringRef &element);

	/// \brief Load CSS document from an input source.
	void load(const CL_String &filename, const CL_VirtualDirectory &directory);

	/// \brief Load CSS document from disk.
	///
	/// \param fullname = full filename
	void load(const CL_String &fullname);

	/// \brief Parses a text string and returns the CSS properties in it
	static std::vector<CL_CSSProperty> parse_style_properties(const CL_StringRef &style_text);
/// \}

/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_CSSDocument_Impl> impl;
/// \}
};

/// \}
