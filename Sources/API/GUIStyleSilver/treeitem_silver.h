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

#ifndef header_treeitem_silver
#define header_treeitem_silver

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

#include "../GUI/treeitem.h"
#include "../GUI/component_style.h"

class CL_Font;
class CL_Surface;
class CL_StyleManager_Silver;

//: Theme class for CL_TreeItem for Silver theme.
//- !group=GUI/Style: Silver!
//- !header=guistylesilver.h!
class CL_API_GUISTYLESILVER CL_TreeItem_Silver : public CL_ComponentStyle
{
//! Construction:
public:
	//: Constructor
	CL_TreeItem_Silver(
		CL_TreeItem *item,
		CL_StyleManager_Silver *style);

	//: Destructor
	virtual ~CL_TreeItem_Silver();

//! Implementation:
private:
	void on_paint();
	void on_get_preferred_size(CL_Size &size);

	CL_Slot slot_paint;
	CL_Slot slot_get_preferred_size;

	CL_TreeItem *item;

	CL_Font *font;

	CL_StyleManager_Silver *style;
};

#endif
