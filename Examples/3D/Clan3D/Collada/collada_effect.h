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
*/

#pragma once

class CL_Collada_Effect_Impl;
class CL_Collada_Effect_ColorTexture;
class CL_Collada_Effect_FloatParam;
class CL_Collada_Image;

class CL_Collada_Effect
{
public:
	CL_Collada_Effect();
	CL_Collada_Effect(CL_DomElement &effect_element, std::vector<CL_Collada_Image> &library_images);

	CL_String &get_name();
	CL_String &get_id();

	// These may return a NULL implemtation if they were unset (use is_null() is check)
	CL_Collada_Effect_ColorTexture &get_emission();
	CL_Collada_Effect_ColorTexture &get_ambient();
	CL_Collada_Effect_ColorTexture &get_diffuse();
	CL_Collada_Effect_ColorTexture &get_specular();
	CL_Collada_Effect_FloatParam &get_shininess();
	CL_Collada_Effect_ColorTexture &get_reflective();
	CL_Collada_Effect_FloatParam &get_reflectivity();
	CL_Collada_Effect_ColorTexture &get_transparent();
	CL_Collada_Effect_FloatParam &get_transparency();
	CL_Collada_Effect_FloatParam &get_index_of_refraction();

	bool is_null();

private:
	CL_SharedPtr<CL_Collada_Effect_Impl> impl;

};
