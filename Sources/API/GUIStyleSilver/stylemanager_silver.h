/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

//! clanGUI="Style: Silver"
//! header=guistylesilver.h

#ifndef header_stylemanager_silver
#define header_stylemanager_silver

#ifdef CL_API_DLL
#ifdef CL_GUISTYLESILVER_EXPORT
#define CL_API_GUISTYLESILVER __declspec(dllexport)
#else
#define CL_API_GUISTYLESILVER __declspec(dllimport)
#endif
#else
#define CL_API_GUISTYLESILVER
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "../GUI/stylemanager.h"

class CL_ResourceManager;
class CL_Component;
class CL_Rect;
class CL_Color;

//: Style manager that uses the default clanlib gui style.
//- !group=GUI/Style: Silver!
//- !header=guistylesilver.h!
class CL_API_GUISTYLESILVER CL_StyleManager_Silver : public CL_StyleManager
{
//! Construction:
public:
	//: Construct a default style manager using the specified resources.
	CL_StyleManager_Silver(CL_ResourceManager *resources);

	//: Destructor.
	virtual ~CL_StyleManager_Silver();

//! Attributes:
public:
	
//! Operations:
public:

//! Overrideables:
public:
	//: Connect component styles to component.
	//: The 'type' parameter indicates what type the component is.
	virtual void connect_styles(
		const std::string &type,
		CL_Component *component);

//! Implementation:
private:
	// Disallow copy contruction of style managers.
	CL_StyleManager_Silver(const CL_StyleManager_Silver &copy) : CL_StyleManager(0) { return; }
};

#endif
