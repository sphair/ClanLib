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
**    Kenneth Gangstoe
*/

#pragma once

#include "API/Core/System/sharedptr.h"

class CL_DomDocument;
class CL_DomElement;
class CL_VirtualDirectory;
class CL_IODevice;
class CL_CSSLayout;
class CL_CSSLayoutElement;
class CL_CSSDocument2;
class CL_GUIThemeLayoutManager_Impl;

class CL_GUIThemeLayoutManager
{
/// \name Construction
/// \{
public:
	CL_GUIThemeLayoutManager();
	CL_GUIThemeLayoutManager(CL_DomDocument &xml_layout, CL_CSSDocument2 &css);
	CL_GUIThemeLayoutManager(const CL_StringRef &xml_fullname, const CL_StringRef &css_fullname);
	CL_GUIThemeLayoutManager(const CL_StringRef &xml_fullname, const CL_StringRef &css_fullname, const CL_VirtualDirectory &dir);
	CL_GUIThemeLayoutManager(CL_IODevice &xml_file, CL_IODevice &css_file);
	~CL_GUIThemeLayoutManager();
/// \}

/// \name Attributes
/// \{
public:
	bool has_layout(const CL_String &component_type);
/// \}

/// \name Operations
/// \{
public:
	CL_CSSLayout create_layout(const CL_String &component_type);
/// \}

/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_GUIThemeLayoutManager_Impl> impl;
/// \}
};
