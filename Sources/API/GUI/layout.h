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

//! clanGUI="Framework"
//! header=gui.h

#ifndef header_layout
#define header_layout

#ifdef CL_API_DLL
#ifdef CL_GUI_EXPORT
#define CL_API_GUI __declspec(dllexport)
#else
#define CL_API_GUI __declspec(dllimport)
#endif
#else
#define CL_API_GUI
#endif

#include "../Core/System/sharedptr.h"

class CL_Component;
class CL_Layout_Generic;
class CL_Rect;

//: Layout types for CL_Layout.
//- !group=GUI/Framework!
//- !header=gui.h!
enum CL_LayoutType
{
	layout_vertical,
	layout_horizontal,
	layout_grid
};

//: Layout management class.
//- !group=GUI/Framework!
//- !header=gui.h!
class CL_API_GUI CL_Layout
{
//! Construction:
public:
	CL_Layout();

	CL_Layout(const CL_Layout &copy);

	CL_Layout(CL_LayoutType type);

	virtual ~CL_Layout();

//! Operations:
public:
	void set_grid_columns(int columns);

	void set_margin(int margin);

	void set_spacing(int spacing);

	void add_component(CL_Component *component);

	void add_component(int index, CL_Component *component);

	void add_component(int col, int row, CL_Component *component, int span=1);

	void add_layout(const CL_Layout &layout);

	void add_layout(int index, const CL_Layout &layout);

	void add_layout(int col, int row, const CL_Layout &layout, int span=1);

	void remove_component(CL_Component *component);

	void remove_layout(CL_Layout *layout);

	void set_position(const CL_Rect &pos);

//! Implementation:
private:
	CL_SharedPtr<CL_Layout_Generic> impl;
};

#endif
