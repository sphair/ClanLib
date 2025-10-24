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
#include "API/GUI/layout.h"
#include "layout_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Layout construction:

CL_Layout::CL_Layout()
{
}

CL_Layout::CL_Layout(const CL_Layout &copy) : impl(copy.impl)
{
}

CL_Layout::CL_Layout(CL_LayoutType type)
{
	impl = new CL_Layout_Generic(type);
}

CL_Layout::~CL_Layout()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Layout operations:

void CL_Layout::set_grid_columns(int columns)
{
	if (impl) impl->grid_columns = columns;
}

void CL_Layout::set_margin(int margin)
{
	if (impl) impl->margin = margin;
}

void CL_Layout::set_spacing(int spacing)
{
	if (impl) impl->spacing = spacing;
}

void CL_Layout::add_component(CL_Component *component)
{
	impl->elements.push_back(new CL_LayoutItem(component));
}

void CL_Layout::add_component(int index, CL_Component *component)
{
	impl->elements.push_back(new CL_LayoutItem(component));
}

void CL_Layout::add_component(int col, int row, CL_Component *component, int span)
{
	impl->elements.push_back(new CL_LayoutItem(component));
}

void CL_Layout::add_layout(const CL_Layout &layout)
{
	impl->elements.push_back(new CL_LayoutItem(layout.impl));
}

void CL_Layout::add_layout(int index, const CL_Layout &layout)
{
	impl->elements.push_back(new CL_LayoutItem(layout.impl));
}

void CL_Layout::add_layout(int col, int row, const CL_Layout &layout, int span)
{
	impl->elements.push_back(new CL_LayoutItem(layout.impl));
}

void CL_Layout::remove_component(CL_Component *component)
{
}

void CL_Layout::remove_layout(CL_Layout *layout)
{
}

void CL_Layout::set_position(const CL_Rect &pos)
{
	if (impl == 0) return;
	impl->set_position(pos);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Layout implementation:
