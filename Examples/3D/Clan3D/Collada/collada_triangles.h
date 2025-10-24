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

class CL_Collada_Triangles_Impl;
class CL_Collada_Source;
class CL_Collada_Vertices;
class CL_Collada_Input_Shared;
class CL_Collada_Material;

class CL_Collada_Triangles
{
public:
	// Contructs a CL_Collada_Triangle containing 0 triangles
	CL_Collada_Triangles();

	// Contructs a CL_Collada_Triangle from a collada element
	CL_Collada_Triangles(CL_DomElement &element, std::vector<CL_Collada_Source> &sources, CL_Collada_Vertices &vertices, std::vector<CL_Collada_Material> &library_materials);

	CL_String &get_name();
	CL_String &get_id();
	unsigned int get_count() const;
	int get_stride();
	
	// Check with ".is_null()" to check material has been set
	CL_Collada_Material &get_material();

	std::vector<int> &get_primitive();
	std::vector<CL_Collada_Input_Shared> &get_inputs();
	CL_Collada_Input_Shared &get_input(const CL_String &semantic);

	// Returns true if a semantic exists
	bool is_semantic(const CL_String &semantic) const;

	// Destination must contain get_count() * 3
	void create_vertices(CL_Vec3f *destination, int stride, const CL_String &semantic);
	void create_vertices(CL_Vec2f *destination, int stride, const CL_String &semantic);

	// Destination must contain get_count() * 3
	void create_vertices_normal(CL_Vec3f *destination, int stride, const CL_String &semantic, const CL_Angle &smoothing_angle);

	// Destination must contain get_count() * 3
	void create_vertices_texcoords(CL_Vec2f *destination, int stride, const CL_String &semantic);

private:
	CL_SharedPtr<CL_Collada_Triangles_Impl> impl;

};


