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

#pragma once

#include "api_csslayout.h"

class CL_CSSProperty2;
class CL_CSSSelectNode2;
class CL_CSSPropertyList2;
class CL_CSSDocument2_Impl;

class CL_API_CSSLAYOUT CL_CSSDocument2
{
public:
	CL_CSSDocument2();
	~CL_CSSDocument2();

	void add_sheet(const CL_String &filename, const CL_String &base_uri = CL_String());
	void add_sheet(CL_IODevice &iodevice, const CL_String &base_uri = CL_String());
	CL_CSSPropertyList2 select(const CL_DomElement &node, const CL_String &pseudo_element = CL_String());
	CL_CSSPropertyList2 select(CL_CSSSelectNode2 *node, const CL_String &pseudo_element = CL_String());
	static CL_CSSPropertyList2 get_style_properties(const CL_String &style_string, const CL_String &base_uri = CL_String());

private:
	CL_SharedPtr<CL_CSSDocument2_Impl> impl;
};
