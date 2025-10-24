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
**    Magnus Norddahl
*/

#include "GL1/precomp.h"
#include "API/GL1/material.h"
#include "API/Display/2D/color.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Material_GL1_Impl Class:

class CL_Material_GL1_Impl
{
public:
	CL_Material_GL1_Impl()
	{
		ambient = CL_Colorf(0.2f, 0.2f, 0.2f, 1.0f);
		diffuse = CL_Colorf(0.8f, 0.8f, 0.8f, 1.0f);
		specular = CL_Colorf(0.0f, 0.0f, 0.0f, 1.0f);
		emission = CL_Colorf(0.0f, 0.0f, 0.0f, 1.0f);
		shininess = 0.0;
	}

	~CL_Material_GL1_Impl()
	{
	}

	CL_Colorf ambient;

	CL_Colorf diffuse;

	CL_Colorf specular;

	CL_Colorf emission;

	float shininess;
};

/////////////////////////////////////////////////////////////////////////////
// CL_Material_GL1 Construction:

CL_Material_GL1::CL_Material_GL1()
: impl(new CL_Material_GL1_Impl)
{
}

CL_Material_GL1::~CL_Material_GL1()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Material_GL1 Attributes:

CL_Colorf CL_Material_GL1::get_ambient() const
{
	return impl->ambient;
}

CL_Colorf CL_Material_GL1::get_diffuse() const
{
	return impl->diffuse;
}

CL_Colorf CL_Material_GL1::get_specular() const
{
	return impl->specular;
}

CL_Colorf CL_Material_GL1::get_emission() const
{
	return impl->emission;
}

float CL_Material_GL1::get_shininess() const
{
	return impl->shininess;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Material_GL1 Operations:

void CL_Material_GL1::set_ambient(const CL_Colorf &color)
{
	impl->ambient = color;
}

void CL_Material_GL1::set_diffuse(const CL_Colorf &color)
{
	impl->diffuse = color;
}

void CL_Material_GL1::set_specular(const CL_Colorf &color)
{
	impl->specular = color;
}

void CL_Material_GL1::set_ambient_and_diffuse(const CL_Colorf &color)
{
	impl->ambient = color;
	impl->diffuse = color;
}

void CL_Material_GL1::set_emission(const CL_Colorf &color)
{
	impl->emission = color;
}

void CL_Material_GL1::set_shininess(float value)
{
	impl->shininess = value;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Material_GL1 Implementation:
