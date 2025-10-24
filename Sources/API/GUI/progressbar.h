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

#ifndef header_progressbar
#define header_progressbar

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

class CL_DomElement;
class CL_Rect;
class CL_ProgressBar_Generic;

//: ProgressBar component
//- !group=GUI/Controls!
//- !header=gui.h!
class CL_API_GUI CL_ProgressBar : public CL_Component
{
//! Construction:
public:
	//: ProgressBar Constructor
	CL_ProgressBar(
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: ProgressBar Constructor
	CL_ProgressBar(
		int steps,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: ProgressBar Constructor
	CL_ProgressBar(
		const CL_Rect &pos,
		int steps,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: ProgressBar Destructor
	virtual ~CL_ProgressBar();

//! Attributes:
public:
	//: Returns the total number of steps.
	int get_steps() const;

	//: Returns the current amount of progress.
	int get_progress() const;

	//: Returns the current amount of progress in percentage.
	float get_percentage() const;

//! Operations:
public:
	//: Sets the total number of steps to total_steps. 
	void set_steps(int total_steps);

	//: Sets the current amount of progress of the total number of steps. 
	void set_progress(int progress);

	//: Progresses the bar with steps.
	void increase(int steps = 1);

	//: Reset the progress bar. The progress bar `rewinds'. 
	void reset();

//! Signals:
public:

//! Implementation:
private:
	CL_ProgressBar(const CL_ProgressBar &copy) : CL_Component(NULL, NULL) { return; } // disallow copy construction.
	CL_ProgressBar_Generic *impl;
};

#endif
