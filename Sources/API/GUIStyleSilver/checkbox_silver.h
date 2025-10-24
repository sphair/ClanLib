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

#ifndef header_checkbox_silver
#define header_checkbox_silver

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

#include "../GUI/checkbox.h"
#include "../GUI/component_style.h"
#include "checkbox_silver.h"

class CL_Font;
class CL_Surface;
class CL_StyleManager_Silver;

//: CheckBox default style.
//- !group=GUI/Style: Silver!
//- !header=guistylesilver.h!
//- <p>Script attributes:</p>
//- <ul>
//- <li>surface_checked: The surface used when button is checked. [String]</li>
//- <li>surface_unchecked: The surface used when button is unchecked. [String]</li>
//- <li>surface_checked_disabled: The surface used when button is checked, but disabled. [String]</li>
//- <li>surface_unchecked_disabled: The surface used when button is unchecked and disabled. [String]</li>
//- <li>font: The font used to draw the label. [String]</li>
//- </ul>
//- <p>Note that it inherits the attributes of CL_CheckBox, CL_Button and CL_Component as well.</p>
class CL_API_GUISTYLESILVER CL_CheckBox_Silver : public CL_ComponentStyle
{
//! Construction:
public:
	//: Constructor
	CL_CheckBox_Silver(
		CL_CheckBox *checkbox,
		CL_StyleManager_Silver *style);
		
	//: Destructor
	virtual ~CL_CheckBox_Silver();

//! Attributes:
public:

//! Operations:
public:
	//: Sets the font used to draw the label.
	//- <p>If 'delete_font' is true, the font will be deleted when the component is deleted.</p>
	void set_font(CL_Font *font, bool delete_font = false);

	//: Sets the font used to draw the label in disabled mode.
	//- <p>If 'delete_font' is true, the font will be deleted when the component is deleted.</p>
	void set_font_disabled(CL_Font *font, bool delete_font = false);

	//: Sets the surface used when button is in the up-state.
	//- <p>If 'delete_surface' is true, the surface will be deleted when the component is deleted.</p>
	void set_surface_checked(CL_Surface *surface, bool delete_surface = false);

	//: Set the surface used when button is in the down-state.
	//- <p>If 'delete_surface' is true, the surface will be deleted when the component is deleted.</p>
	void set_surface_unchecked(CL_Surface *surface, bool delete_surface = false);

	//: Sets the surface used when button is highlighted.
	//- <p>If 'delete_surface' is true, the surface will be deleted when the component is deleted.</p>
	void set_surface_checked_disabled(CL_Surface *surface, bool delete_surface = false);

	//: Sets the surface used when button is disabled.
	//- <p>If 'delete_surface' is true, the surface will be deleted when the component is deleted.</p>
	void set_surface_unchecked_disabled(CL_Surface *surface, bool delete_surface = false);

//! Implementation:
private:
	void on_set_options(const CL_DomElement &options);
	CL_Slot slot_set_options;

	void on_get_preferred_size(CL_Size &size);
	CL_Slot slot_get_preferred_size;

	void on_paint(CL_SlotParent_v0 &super);
	CL_Slot slot_paint;

	CL_CheckBox *checkbox;

	CL_StyleManager_Silver *style;
	CL_ResourceManager *resources;
	
	CL_Font *font;
	CL_Font *font_disabled;
	bool delete_font;
	bool delete_font_disabled;

	CL_Surface *sur_checked;
	CL_Surface *sur_unchecked;
	CL_Surface *sur_checked_disabled;
	CL_Surface *sur_unchecked_disabled;

	bool delete_sur_checked;
	bool delete_sur_unchecked;
	bool delete_sur_checked_disabled;
	bool delete_sur_unchecked_disabled;
};

#endif

