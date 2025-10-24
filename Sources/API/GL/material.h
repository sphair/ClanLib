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

//! clanGL="Display 3D"
//! header=gl.h

#ifndef header_material
#define header_material

#ifdef CL_API_DLL
#ifdef CL_GL_EXPORT
#define CL_API_GL __declspec(dllexport)
#else
#define CL_API_GL __declspec(dllimport)
#endif
#else
#define CL_API_GL
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "../Core/System/sharedptr.h"

class CL_Material_Generic;

//: glMaterial container.
class CL_API_GL CL_Material
{
//! Construction:
public:
	//: Constructs a material.
	CL_Material();

	~CL_Material();

//! Attributes:
public:
	//: Returns the ambient intensity of the material.
	void get_ambient(float &red, float &green, float &blue, float &alpha) const;

	//: Returns the diffuse (spread over wide area) intensity of the material.
	void get_diffuse(float &red, float &green, float &blue, float &alpha) const;

	//: Returns the specular (mirror reflective) intensity of the material.
	void get_specular(float &red, float &green, float &blue, float &alpha) const;

	//: Returns the combined ambient and diffuse intensity of the material.
	void get_ambient_and_diffuse(float &red, float &green, float &blue, float &alpha) const;

	//: Returns the emission of the material.
	void get_emission(float &red, float &green, float &blue, float &alpha) const;

	//: Returns the shininess of the material.
	float get_shininess() const;

//! Operations:
public:
	//: Set the ambient intensity of the material.
	void set_ambient(float red, float green, float blue, float alpha);

	//: Set the diffuse (spread over wide area) intensity of the material.
	void set_diffuse(float red, float green, float blue, float alpha);

	//: Set the specular (mirror reflective) intensity of the material.
	void set_specular(float red, float green, float blue, float alpha);

	//: Set the combined ambient and diffuse intensity of the material.
	void set_ambient_and_diffuse(float red, float green, float blue, float alpha);

	//: Set the emission of the material.
	void set_emission(float red, float green, float blue, float alpha);

	//: Set the shininess of the material.
	void set_shininess(float value);

	//: Send the material configuration to OpenGL.
	void setup_material(CLenum face);

//! Implementation:
private:
	CL_SharedPtr<CL_Material_Generic> impl;
};

#endif
