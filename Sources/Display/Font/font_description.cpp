/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
**    Mark Page
**    Harry Storbacka
*/
	
#include "Display/precomp.h"
#include "API/Display/Font/font_description.h"
#include "API/Core/Text/string_help.h"
#include "font_description_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_FontDescription construction:

CL_FontDescription::CL_FontDescription() : impl(new CL_FontDescription_Impl)
{
}

CL_FontDescription::~CL_FontDescription()
{
}

CL_FontDescription CL_FontDescription::create_null_object()
{
	CL_FontDescription fd;
	fd.impl = CL_SharedPtr<CL_FontDescription_Impl>();
	return fd;
}

/////////////////////////////////////////////////////////////////////////////
// CL_FontDescription attributes:


void CL_FontDescription::throw_if_null() const
{
	if (impl.is_null())
		throw CL_Exception("is null");
}

const CL_String &CL_FontDescription::get_typeface_name() const
{
	return impl->typeface_name;
}

int CL_FontDescription::get_height() const
{
	return impl->height;
}

int CL_FontDescription::get_average_width() const
{
	return impl->average_width;
}

float CL_FontDescription::get_escapement() const
{
	return impl->escapement;
}

float CL_FontDescription::get_orientation() const
{
	return impl->orientation;
}

int CL_FontDescription::get_weight() const
{
	return impl->weight;
}

bool CL_FontDescription::get_italic() const
{
	return impl->italic;
}

bool CL_FontDescription::get_underline() const
{
	return impl->underline;
}

bool CL_FontDescription::get_strikeout() const
{
	return impl->strikeout;
}

bool CL_FontDescription::get_fixed_pitch() const
{
	return impl->fixed_pitch;
}

bool CL_FontDescription::get_anti_alias() const
{
	return impl->anti_alias;
}

bool CL_FontDescription::get_anti_alias_set() const
{
	return impl->anti_alias_set;
}

bool CL_FontDescription::operator==(const CL_FontDescription &other) const
{
	if ( (impl->anti_alias_set) && (other.impl->anti_alias_set) )
	{
		if (impl->anti_alias != other.impl->anti_alias)
			return false;
	}

	return impl->typeface_name == other.impl->typeface_name && 
			impl->height == other.impl->height && 
			impl->average_width == other.impl->average_width && 
			impl->escapement == other.impl->escapement && 
			impl->orientation == other.impl->orientation && 
			impl->weight == other.impl->weight && 
			impl->italic == other.impl->italic && 
			impl->underline == other.impl->underline && 
			impl->strikeout == other.impl->strikeout && 
			impl->fixed_pitch == other.impl->fixed_pitch;
}

/////////////////////////////////////////////////////////////////////////////
// CL_FontDescription operations:

CL_FontDescription &CL_FontDescription::operator =(const CL_FontDescription &copy)
{
	impl = copy.impl;
	return *this;
}

void CL_FontDescription::clone(const CL_FontDescription &copy)
{
	if (this != &copy)
	{
		impl->typeface_name = copy.impl->typeface_name;
		impl->height = copy.impl->height;
		impl->average_width = copy.impl->average_width;
		impl->escapement = copy.impl->escapement;
		impl->orientation = copy.impl->orientation;
		impl->weight = copy.impl->weight;
		impl->italic = copy.impl->italic;
		impl->underline = copy.impl->underline;
		impl->strikeout = copy.impl->strikeout;
		impl->fixed_pitch = copy.impl->fixed_pitch;
		impl->anti_alias = copy.impl->anti_alias;
		impl->anti_alias_set = copy.impl->anti_alias_set; 
	}
}

void CL_FontDescription::set_typeface_name(const CL_String &name)
{
	impl->typeface_name = name;
}

void CL_FontDescription::set_height(int value)
{
	impl->height = value;
}

void CL_FontDescription::set_average_width(int value)
{
	impl->average_width = value;
}

void CL_FontDescription::set_escapement(float value)
{
	impl->escapement = value;
}

void CL_FontDescription::set_orientation(float value)
{
	impl->orientation = value;
}

void CL_FontDescription::set_weight(int value)
{
	impl->weight = value;
}

void CL_FontDescription::set_italic(bool setting)
{
	impl->italic = setting;
}

void CL_FontDescription::set_underline(bool setting)
{
	impl->underline = setting;
}

void CL_FontDescription::set_strikeout(bool setting)
{
	impl->strikeout = setting;
}

void CL_FontDescription::set_fixed_pitch(bool setting)
{
	impl->fixed_pitch = setting;
}

void CL_FontDescription::set_anti_alias(bool setting)
{
	impl->anti_alias = setting;
	impl->anti_alias_set = true;
}

/////////////////////////////////////////////////////////////////////////////
// CL_FontDescription implementation:

