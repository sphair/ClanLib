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

#ifndef header_menu_node_silver
#define header_menu_node_silver

#ifdef CL_API_DLL
#ifdef CL_GUISTYLESILVER_EXPORT
#define CL_API_GUISTYLESILVER __declspec(dllexport)
#else
#define CL_API_GUISTYLESILVER __declspec(dllimport)
#endif
#else
#define CL_API_GUISTYLESILVER
#endif

//#include "../core.h"
//#include "../gui.h"

#include "../GUI/menu_node.h"
#include "../GUI/component_style.h"

class CL_StyleManager_Silver;
class CL_Surface;

//: MenuNode default style.
//- !group=GUI/Style: Silver!
//- !header=guistylesilver.h!
class CL_API_GUISTYLESILVER CL_MenuNode_Silver : public CL_ComponentStyle
{
//! Construction:
public:
	//: Constructor
	CL_MenuNode_Silver(
		CL_MenuNode *node,
		CL_StyleManager_Silver *style);

	//: Destructor
	virtual ~CL_MenuNode_Silver();

//! Implementation:
private:
	void on_set_options(const CL_DomElement &options);

	void on_paint();

	void on_pressed();

	void on_released();

	void on_reposition();

	void on_get_preferred_size(CL_Size &size);

	CL_SlotContainer slots;

	CL_MenuNode *node;
	
	CL_StyleManager_Silver *style;

	CL_ResourceManager *resources;

	CL_Surface *arrow;
	
	bool down;
};

#endif
