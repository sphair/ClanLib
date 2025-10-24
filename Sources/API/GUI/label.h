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

//! clanGUI="Controls"
//! header=gui.h

#ifndef header_label
#define header_label

#ifdef CL_API_DLL
#ifdef CL_GUI_EXPORT
#define CL_API_GUI __declspec(dllexport)
#else
#define CL_API_GUI __declspec(dllimport)
#endif
#else
#define CL_API_GUI
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "component.h"
#include "../Core/Math/origin.h"
#include "../Display/font.h"

class CL_Label_Generic;

//: Label component
//- !group=GUI/Controls!
//- !header=gui.h!
class CL_API_GUI CL_Label : public CL_Component
{
//! Construction:
public:
	//: Label Constructor
	CL_Label(
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Label Constructor
	CL_Label(
		const std::string &text,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Label Constructor
	CL_Label(
		const CL_Point &pos,
		const std::string &text,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Label Constructor
	CL_Label(
		const CL_Rect &pos,
		const std::string &text,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Label Destructor
	virtual ~CL_Label();

//! Attributes:
public:
	//: Returns the labeltext.
	const std::string &get_text() const;

	//: Returns the alignment setting.
	//: See set_alignment() for more info.
	CL_Origin get_alignment() const;
	
	//: Returns the font the label uses.
	CL_Font* get_font();

//! Operations:
public:
	//: Sets the labeltext.
	void set_text(const std::string &text);

	//: Sets the label contents to a plain text containing the printed value of num.
	void set_text(int number);

	//: Sets the label contents to a plain text containing the printed value of num.
	void set_text(double number);

	//: Clears any label contents. Equivalent with setText("").
	void clear();

	//: Sets the alignment of the label contents
	void set_alignment(CL_Origin alignment);
	
	//: Sets the font of the label.
	void set_font(const CL_Font& f);
	

//! Signals:
public:
	//: Called after font is changed.
	CL_Signal_v0 &sig_font_change();

//! Implementation:
private:
	void init(const std::string &text);

	CL_Label(const CL_Label &copy) : CL_Component(NULL, NULL) { return; } // disallow copy construction.
	CL_Label_Generic *impl;
	
};  

#endif
