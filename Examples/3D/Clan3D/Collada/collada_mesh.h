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

class CL_Collada_Mesh_Impl;
class CL_Collada_Source;
class CL_Collada_Vertices;
class CL_Collada_Triangles;
class CL_Collada_Material;

class CL_Collada_Mesh
{
public:
	CL_Collada_Mesh();
	CL_Collada_Mesh(CL_DomElement &mesh_element, std::vector<CL_Collada_Material> &library_materials);

	std::vector<CL_Collada_Source> &get_sources();
	std::vector<CL_Collada_Triangles> &get_triangles();

	// Count the total number of triangles from all the surfaces in this mesh with a specified semantic
	int get_triangle_count(const CL_String &semantic) const;

	// Destination must contain get_triangle_count() * 3
	// "surface_list" is appended to
	// If a semantic was not found, CL_Collada_Triangles will contain 0 triangles
	void create_vertices(CL_Vec3f *destination, int stride, const CL_String &semantic, std::vector<CL_Collada_Triangles> &surface_list);
	void create_vertices(CL_Vec2f *destination, int stride, const CL_String &semantic, std::vector<CL_Collada_Triangles> &surface_list);

	// Destination must contain get_triangle_count() * 3
	void create_vertices_normal(CL_Vec3f *destination, int stride, const CL_String &semantic, const CL_Angle &smoothing_angle);

	// Destination must contain get_triangle_count() * 3
	void create_vertices_texcoords(CL_Vec2f *destination, int stride, const CL_String &semantic);

	bool is_null();

private:
	CL_SharedPtr<CL_Collada_Mesh_Impl> impl;

};


