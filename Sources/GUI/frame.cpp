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
#include "API/GUI/frame.h"
#include "API/GUI/stylemanager.h"
#include "API/Core/XML/dom_element.h"
#include "frame_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_Frame::CL_Frame(
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(NULL)
{
	impl = new CL_Frame_Generic(this);
	get_style_manager()->connect_styles("frame", this);
	find_preferred_size();
}

CL_Frame::CL_Frame(
	const CL_Rect &pos,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), impl(NULL)
{
	impl = new CL_Frame_Generic(this);
	get_style_manager()->connect_styles("frame", this);
}

CL_Frame::~CL_Frame()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

bool CL_Frame::is_fill_enabled() const
{
	return impl->fill;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_Frame::enable_fill(bool enabled)
{
	impl->fill = enabled;
}

