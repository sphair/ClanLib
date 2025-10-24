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
	
#include "Display/display_precomp.h"
#include "API/Display/display_window_description.h"
#include "display_window_description_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindowDescription construction:

CL_DisplayWindowDescription::CL_DisplayWindowDescription() : impl(new CL_DisplayWindowDescription_Generic)
{
}

CL_DisplayWindowDescription::CL_DisplayWindowDescription(const CL_DisplayWindowDescription &copy) : impl(copy.impl)
{
}

CL_DisplayWindowDescription::CL_DisplayWindowDescription(CL_DisplayWindowDescription_Generic *impl) : impl(impl)
{
}

CL_DisplayWindowDescription::~CL_DisplayWindowDescription()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindowDescription attributes:

const std::string &CL_DisplayWindowDescription::get_title() const
{
	return impl->title;
}

CL_Size CL_DisplayWindowDescription::get_size() const
{
	return impl->position.get_size();
}

CL_Rect CL_DisplayWindowDescription::get_position() const
{
	return impl->position;
}

bool CL_DisplayWindowDescription::is_fullscreen() const
{
	return impl->fullscreen;
}

int CL_DisplayWindowDescription::get_flipping_buffers() const
{
	return impl->flipping_buffers;
}

bool CL_DisplayWindowDescription::get_allow_resize() const
{
	return impl->allow_resize;
}

#ifdef WIN32
bool CL_DisplayWindowDescription::is_layered() const
{
	return impl->layered;
}
#endif

int CL_DisplayWindowDescription::get_bpp() const
{
	return impl->bpp;
}

int CL_DisplayWindowDescription::get_refresh_rate() const
{
	return impl->refresh_rate;
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindowDescription operations:

CL_DisplayWindowDescription &CL_DisplayWindowDescription::operator =(const CL_DisplayWindowDescription &copy)
{
	impl = copy.impl;
	return *this;
}

void CL_DisplayWindowDescription::set_title(const std::string &title)
{
	impl->title = title;
}

void CL_DisplayWindowDescription::set_size(const CL_Size &size)
{
	impl->position.set_size(size);
}

void CL_DisplayWindowDescription::set_position(const CL_Rect &position)
{
	impl->position = position;
}

void CL_DisplayWindowDescription::set_fullscreen(bool fullscreen)
{
	impl->fullscreen = fullscreen;
}

void CL_DisplayWindowDescription::set_flipping_buffers(int num_buffers)
{
	impl->flipping_buffers = num_buffers;
}

void CL_DisplayWindowDescription::set_allow_resize(bool allow_resize)
{
	impl->allow_resize = allow_resize;
}

void CL_DisplayWindowDescription::set_bpp(int bytes)
{
	impl->bpp = bytes;
}

void CL_DisplayWindowDescription::set_refresh_rate(int refresh_rate)
{
	impl->refresh_rate = refresh_rate;
}

#ifdef WIN32
void CL_DisplayWindowDescription::set_layered(bool layered)
{
	impl->layered = layered;
}
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindowDescription implementation:
