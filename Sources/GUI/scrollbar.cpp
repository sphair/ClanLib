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
#include "API/GUI/scrollbar.h"
#include "API/GUI/stylemanager.h"
#include "API/Core/XML/dom_element.h"
#include "scrollbar_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ScrollBar construction:

CL_ScrollBar::CL_ScrollBar(
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(0)
{
	impl = new CL_ScrollBar_Generic(this, 0, 100, 0, false, true);
	get_style_manager()->connect_styles("scrollbar", this);
	find_preferred_size();
}

CL_ScrollBar::CL_ScrollBar(
	int min,
	int max,
	bool orientation,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(0)
{
	impl = new CL_ScrollBar_Generic(this, min, max, 0, orientation, true);
	get_style_manager()->connect_styles("scrollbar", this);
	find_preferred_size();
}

CL_ScrollBar::CL_ScrollBar(
	bool orientation,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(0)
{
	impl = new CL_ScrollBar_Generic(this, 0, 100, 0, orientation, true);
	get_style_manager()->connect_styles("scrollbar", this);
	find_preferred_size();
}

CL_ScrollBar::CL_ScrollBar(
	const CL_Rect &pos,
	int min,
	int max,
	bool orientation,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), impl(0)
{
	impl = new CL_ScrollBar_Generic(this, min, max, 0, orientation, true);
	get_style_manager()->connect_styles("scrollbar", this);
}

CL_ScrollBar::CL_ScrollBar(
	const CL_Rect &pos,
	bool orientation,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), impl(0)
{
	impl = new CL_ScrollBar_Generic(this, 0, 100, 0, orientation, false);
	get_style_manager()->connect_styles("scrollbar", this);
}

CL_ScrollBar::~CL_ScrollBar()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

bool CL_ScrollBar::is_vertical() const
{
	return impl->vertical;
}

bool CL_ScrollBar::is_tracking() const
{
	return impl->tracking;
}

int CL_ScrollBar::get_min_value() const
{
	return impl->min_value;
}

int CL_ScrollBar::get_max_value() const
{
	return impl->max_value;
}

int CL_ScrollBar::get_value() const
{
	return impl->cur_value;
}

int CL_ScrollBar::get_range() const
{
	return impl->get_range();
}

bool CL_ScrollBar::is_dragging_slider() const
{
	return impl->dragging;
}

CL_Rect &CL_ScrollBar::get_slider_rect() const
{
	return impl->rect_slider;
}

bool CL_ScrollBar::is_fixed_length_slider() const
{
	return impl->fixed_length;
}

int CL_ScrollBar::get_min_slider_length() const
{
	return impl->min_slider_length;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_ScrollBar::set_vertical(bool enable)
{
	impl->set_vertical(enable);
}

void CL_ScrollBar::set_tracking(bool enable)
{
	impl->tracking = enable;
}

void CL_ScrollBar::set_range(int min_value, int max_value)
{
	impl->set_range(min_value, max_value);
}

void CL_ScrollBar::set_min_value(int value)
{
	impl->set_min_value(value);
}

void CL_ScrollBar::set_max_value(int value)
{
	impl->set_max_value(value);
}

void CL_ScrollBar::set_value(int value)
{
	impl->set_value(value);
}

void CL_ScrollBar::increase(int step)
{
	impl->set_value(impl->cur_value + step);
}

void CL_ScrollBar::decrease(int step)
{
	impl->set_value(impl->cur_value - step);
}

void CL_ScrollBar::set_fixed_length_mode(bool enable)
{
	impl->fixed_length = enable;
}

void CL_ScrollBar::set_slider_length(int length)
{
	impl->fixed_slider_length = length;
}

void CL_ScrollBar::set_min_slider_length(int length)
{
	impl->min_slider_length = length;
}

/////////////////////////////////////////////////////////////////////////////
// Signals:

CL_Signal_v1<int> &CL_ScrollBar::sig_value_changed()
{
	return impl->sig_value_changed;
}

CL_Signal_v0 &CL_ScrollBar::sig_slider_pressed()
{
	return impl->sig_slider_pressed;
}

CL_Signal_v1<int> &CL_ScrollBar::sig_slider_moved()
{
	return impl->sig_slider_moved;
}

CL_Signal_v0 &CL_ScrollBar::sig_slider_released()
{
	return impl->sig_slider_released;
}
