/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Material_GL1_Impl Class:

class Material_GL1_Impl
{
public:
	Material_GL1_Impl()
	{
		ambient = Colorf(0.2f, 0.2f, 0.2f, 1.0f);
		diffuse = Colorf(0.8f, 0.8f, 0.8f, 1.0f);
		specular = Colorf(0.0f, 0.0f, 0.0f, 1.0f);
		emission = Colorf(0.0f, 0.0f, 0.0f, 1.0f);
		shininess = 0.0;
	}

	~Material_GL1_Impl()
	{
	}

	Colorf ambient;

	Colorf diffuse;

	Colorf specular;

	Colorf emission;

	float shininess;
};

/////////////////////////////////////////////////////////////////////////////
// Material_GL1 Construction:

Material_GL1::Material_GL1()
: impl(new Material_GL1_Impl)
{
}

Material_GL1::~Material_GL1()
{
}

/////////////////////////////////////////////////////////////////////////////
// Material_GL1 Attributes:

Colorf Material_GL1::get_ambient() const
{
	return impl->ambient;
}

Colorf Material_GL1::get_diffuse() const
{
	return impl->diffuse;
}

Colorf Material_GL1::get_specular() const
{
	return impl->specular;
}

Colorf Material_GL1::get_emission() const
{
	return impl->emission;
}

float Material_GL1::get_shininess() const
{
	return impl->shininess;
}

/////////////////////////////////////////////////////////////////////////////
// Material_GL1 Operations:

void Material_GL1::set_ambient(const Colorf &color)
{
	impl->ambient = color;
}

void Material_GL1::set_diffuse(const Colorf &color)
{
	impl->diffuse = color;
}

void Material_GL1::set_specular(const Colorf &color)
{
	impl->specular = color;
}

void Material_GL1::set_ambient_and_diffuse(const Colorf &color)
{
	impl->ambient = color;
	impl->diffuse = color;
}

void Material_GL1::set_emission(const Colorf &color)
{
	impl->emission = color;
}

void Material_GL1::set_shininess(float value)
{
	impl->shininess = value;
}

/////////////////////////////////////////////////////////////////////////////
// Material_GL1 Implementation:

}
