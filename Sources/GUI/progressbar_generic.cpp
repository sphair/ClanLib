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

#include "precomp.h"
#include "progressbar_generic.h"
#include "API/GUI/progressbar.h"
#include "API/GUI/component.h"
#include "API/GUI/stylemanager.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/System/clanstring.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_ProgressBar_Generic::CL_ProgressBar_Generic(
	CL_ProgressBar *self,
	int steps)
:
	progress(0),
	progressbar(self)
{
	set_steps(steps);

	slot_set_options = progressbar->sig_set_options().connect(
		this, &CL_ProgressBar_Generic::on_set_options);
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

float CL_ProgressBar_Generic::get_percentage() const
{
	return (float)progress / steps;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_ProgressBar_Generic::set_steps(int total_steps)
{
	steps = total_steps;

	if(progress > steps)
		progress = steps;
}

void CL_ProgressBar_Generic::set_progress(int new_progress)
{
	progress = new_progress;
	if(progress > steps)
		progress = steps;
	if(progress < 0)
		progress = 0;
}

void CL_ProgressBar_Generic::increase(int steps)
{
	set_progress(progress + steps);
}

void CL_ProgressBar_Generic::reset()
{
	progress = 0;
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_ProgressBar_Generic::on_set_options(const CL_DomElement &options)
{
	if (options.has_attribute("steps"))
		set_steps(CL_String::to_int(options.get_attribute("steps")));
}
