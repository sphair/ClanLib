/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

class CL_Collada_Effect_ColorTexture_Impl;
class CL_Collada_Effect_Texture;

class CL_Collada_Effect_ColorTexture
{
public:
	CL_Collada_Effect_ColorTexture();
	CL_Collada_Effect_ColorTexture(CL_DomElement &color_texture_element, std::vector<CL_Collada_Effect_Texture> &samplers);

	// Returns true if the color is set.
	bool is_color_set();
	CL_Colorf &get_color();	
	CL_Vec4f get_color2();	

	// This may return a NULL implemtation if it is unset (use is_null() is check)
	CL_Collada_Effect_Texture &get_texture();

	bool is_null();

private:
	CL_SharedPtr<CL_Collada_Effect_ColorTexture_Impl> impl;

};


