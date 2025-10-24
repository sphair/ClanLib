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
#include "API/Core/XML/dom_element.h"
#include "API/Display/surface.h"
#include "API/GUI/image.h"
#include "API/GUI/stylemanager.h"
#include "API/Display/surface.h"
#include "image_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_Image::CL_Image(
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(NULL)
{
	impl = new CL_Image_Generic(this, NULL, false);
	get_style_manager()->connect_styles("image", this);
	find_preferred_size();
}

CL_Image::CL_Image(
	const CL_Rect &pos,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), impl(NULL)
{
	impl = new CL_Image_Generic(this, NULL, false);
	get_style_manager()->connect_styles("image", this);
}

CL_Image::CL_Image(
	const CL_Rect &pos,
	CL_Surface *surface,
	bool delete_surface,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), impl(NULL)
{
	impl = new CL_Image_Generic(this, surface, delete_surface);
	get_style_manager()->connect_styles("image", this);
}

CL_Image::CL_Image(
	CL_Surface *surface,
	bool delete_surface,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(NULL)
{
	impl = new CL_Image_Generic(this, surface, delete_surface);
	get_style_manager()->connect_styles("image", this);
	find_preferred_size();
}

CL_Image::CL_Image(
	const CL_Point &pos,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(CL_Rect(pos.x, pos.y, 0, 0), parent, style), impl(NULL)
{
	impl = new CL_Image_Generic(this, NULL, false);
	get_style_manager()->connect_styles("image", this);
	find_preferred_size();
}

CL_Image::CL_Image(
	const CL_Point &pos,
	CL_Surface *surface,
	bool delete_surface,	
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(CL_Rect(pos.x, pos.y, 0, 0), parent, style), impl(NULL)
{
	impl = new CL_Image_Generic(this, surface, delete_surface);
	get_style_manager()->connect_styles("image", this);
	find_preferred_size();
}

CL_Image::~CL_Image()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

CL_Surface *CL_Image::get_surface() const
{
	return impl->surface;
}

CL_Image::Mode CL_Image::get_mode() const
{
	return impl->mode;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_Image::set_surface(CL_Surface *surface, bool delete_surface)
{
	if(impl->surface && impl->delete_surface)
		delete impl->surface;
		
	impl->surface = surface;
	impl->delete_surface = delete_surface;
}

void CL_Image::set_mode(Mode mode)
{
	impl->mode = mode;
}
