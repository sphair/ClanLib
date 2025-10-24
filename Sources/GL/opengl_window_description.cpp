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
#include "API/GL/opengl_window.h"
#include "API/GL/opengl_window_description.h"
#include "opengl_window_description_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLWindowDescription construction:

CL_OpenGLWindowDescription::CL_OpenGLWindowDescription()
: CL_DisplayWindowDescription(new CL_OpenGLWindowDescription_Generic)
{
}

CL_OpenGLWindowDescription::~CL_OpenGLWindowDescription()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLWindowDescription attributes:

int CL_OpenGLWindowDescription::get_buffer_size() const
{
	return dynamic_cast<const CL_OpenGLWindowDescription_Generic*>(impl.get())->buffer_size;
}

int CL_OpenGLWindowDescription::get_level() const
{
	return dynamic_cast<const CL_OpenGLWindowDescription_Generic*>(impl.get())->level;
}

bool CL_OpenGLWindowDescription::get_rgba() const
{
	return dynamic_cast<const CL_OpenGLWindowDescription_Generic*>(impl.get())->rgba;
}

bool CL_OpenGLWindowDescription::get_doublebuffer() const
{
	return dynamic_cast<const CL_OpenGLWindowDescription_Generic*>(impl.get())->doublebuffer;
}

bool CL_OpenGLWindowDescription::get_stereo() const
{
	return dynamic_cast<const CL_OpenGLWindowDescription_Generic*>(impl.get())->stereo;
}

int CL_OpenGLWindowDescription::get_aux_buffers() const
{
	return dynamic_cast<const CL_OpenGLWindowDescription_Generic*>(impl.get())->aux_buffers;
}

int CL_OpenGLWindowDescription::get_red_size() const
{
	return dynamic_cast<const CL_OpenGLWindowDescription_Generic*>(impl.get())->red_size;
}

int CL_OpenGLWindowDescription::get_green_size() const
{
	return dynamic_cast<const CL_OpenGLWindowDescription_Generic*>(impl.get())->green_size;
}

int CL_OpenGLWindowDescription::get_blue_size() const
{
	return dynamic_cast<const CL_OpenGLWindowDescription_Generic*>(impl.get())->blue_size;
}

int CL_OpenGLWindowDescription::get_alpha_size() const
{
	return dynamic_cast<const CL_OpenGLWindowDescription_Generic*>(impl.get())->alpha_size;
}

int CL_OpenGLWindowDescription::get_depth_size() const
{
	return dynamic_cast<const CL_OpenGLWindowDescription_Generic*>(impl.get())->depth_size;
}

int CL_OpenGLWindowDescription::get_stencil_size() const
{
	return dynamic_cast<const CL_OpenGLWindowDescription_Generic*>(impl.get())->stencil_size;
}

int CL_OpenGLWindowDescription::get_accum_red_size() const
{
	return dynamic_cast<const CL_OpenGLWindowDescription_Generic*>(impl.get())->accum_red_size;
}

int CL_OpenGLWindowDescription::get_accum_green_size() const
{
	return dynamic_cast<const CL_OpenGLWindowDescription_Generic*>(impl.get())->accum_green_size;
}

int CL_OpenGLWindowDescription::get_accum_blue_size() const
{
	return dynamic_cast<const CL_OpenGLWindowDescription_Generic*>(impl.get())->accum_blue_size;
}

int CL_OpenGLWindowDescription::get_accum_alpha_size() const
{
	return dynamic_cast<const CL_OpenGLWindowDescription_Generic*>(impl.get())->accum_alpha_size;
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLWindowDescription operations:

void CL_OpenGLWindowDescription::set_rgba(bool value)
{
	dynamic_cast<CL_OpenGLWindowDescription_Generic*>(impl.get())->rgba = value;
}

void CL_OpenGLWindowDescription::set_doublebuffer(bool value)
{
	dynamic_cast<CL_OpenGLWindowDescription_Generic*>(impl.get())->doublebuffer = value;
}

void CL_OpenGLWindowDescription::set_stereo(bool value)
{
	dynamic_cast<CL_OpenGLWindowDescription_Generic*>(impl.get())->stereo = value;
}

void CL_OpenGLWindowDescription::set_buffer_size(int value)
{
	dynamic_cast<CL_OpenGLWindowDescription_Generic*>(impl.get())->buffer_size = value;
}

void CL_OpenGLWindowDescription::set_level(int value)
{
	dynamic_cast<CL_OpenGLWindowDescription_Generic*>(impl.get())->level = value;
}

void CL_OpenGLWindowDescription::set_aux_buffers(int value)
{
	dynamic_cast<CL_OpenGLWindowDescription_Generic*>(impl.get())->aux_buffers = value;
}

void CL_OpenGLWindowDescription::set_red_size(int value)
{
	dynamic_cast<CL_OpenGLWindowDescription_Generic*>(impl.get())->red_size = value;
}

void CL_OpenGLWindowDescription::set_green_size(int value)
{
	dynamic_cast<CL_OpenGLWindowDescription_Generic*>(impl.get())->green_size = value;
}

void CL_OpenGLWindowDescription::set_blue_size(int value)
{
	dynamic_cast<CL_OpenGLWindowDescription_Generic*>(impl.get())->blue_size = value;
}

void CL_OpenGLWindowDescription::set_alpha_size(int value)
{
	dynamic_cast<CL_OpenGLWindowDescription_Generic*>(impl.get())->alpha_size = value;
}

void CL_OpenGLWindowDescription::set_depth_size(int value)
{
	dynamic_cast<CL_OpenGLWindowDescription_Generic*>(impl.get())->depth_size = value;
}

void CL_OpenGLWindowDescription::set_stencil_size(int value)
{
	dynamic_cast<CL_OpenGLWindowDescription_Generic*>(impl.get())->stencil_size = value;
}

void CL_OpenGLWindowDescription::set_accum_red_size(int value)
{
	dynamic_cast<CL_OpenGLWindowDescription_Generic*>(impl.get())->accum_red_size = value;
}

void CL_OpenGLWindowDescription::set_accum_green_size(int value)
{
	dynamic_cast<CL_OpenGLWindowDescription_Generic*>(impl.get())->accum_green_size = value;
}

void CL_OpenGLWindowDescription::set_accum_blue_size(int value)
{
	dynamic_cast<CL_OpenGLWindowDescription_Generic*>(impl.get())->accum_blue_size = value;
}

void CL_OpenGLWindowDescription::set_accum_alpha_size(int value)
{
	dynamic_cast<CL_OpenGLWindowDescription_Generic*>(impl.get())->accum_alpha_size = value;
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLWindowDescription implementation:
