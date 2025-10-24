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
#include "API/GUI/progressbar.h"
#include "API/GUI/stylemanager.h"
#include "API/Core/XML/dom_element.h"
#include "progressbar_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_ProgressBar::CL_ProgressBar(
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(NULL)
{
	impl = new CL_ProgressBar_Generic(this, 0);
	get_style_manager()->connect_styles("progressbar", this);
	find_preferred_size();
}

CL_ProgressBar::CL_ProgressBar(
	int steps,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(NULL)
{
	impl = new CL_ProgressBar_Generic(this, steps);
	get_style_manager()->connect_styles("progressbar", this);
	find_preferred_size();
}

CL_ProgressBar::CL_ProgressBar(
	const CL_Rect &pos,
	int steps,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), impl(NULL)
{
	impl = new CL_ProgressBar_Generic(this, steps);
	get_style_manager()->connect_styles("progressbar", this);
}

CL_ProgressBar::~CL_ProgressBar()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

int CL_ProgressBar::get_progress() const
{
	return impl->progress;
}

int CL_ProgressBar::get_steps() const
{
	return impl->steps;
}

float CL_ProgressBar::get_percentage() const
{
	return impl->get_percentage();
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_ProgressBar::set_steps(int total_steps)
{
	impl->set_steps(total_steps);
}

void CL_ProgressBar::set_progress(int progress)
{
	impl->set_progress(progress);
}

void CL_ProgressBar::increase(int steps)
{
	impl->increase(steps);
}

void CL_ProgressBar::reset()
{
	impl->reset();
}
