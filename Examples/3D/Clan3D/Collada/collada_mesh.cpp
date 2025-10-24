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

#include "precomp.h"
#include "collada_mesh.h"
#include "collada_source.h"
#include "collada_vertices.h"
#include "collada_triangles.h"

class CL_Collada_Mesh_Impl
{
public:
	CL_Collada_Mesh_Impl() {}

	void load_sources(CL_DomElement &mesh_element);
	void load_vertices(CL_DomElement &mesh_element);
	void load_triangles(CL_DomElement &mesh_element, std::vector<CL_Collada_Material> &library_materials);
	void create_vertices(CL_Vec3f *destination, int stride, const CL_String &semantic, std::vector<CL_Collada_Triangles> &surface_list);
	void create_vertices(CL_Vec2f *destination, int stride, const CL_String &semantic, std::vector<CL_Collada_Triangles> &surface_list);
	void create_vertices_normal(CL_Vec3f *destination, int stride, const CL_String &semantic, const CL_Angle &smoothing_angle);
	void create_vertices_texcoords(CL_Vec2f *destination, int stride, const CL_String &semantic);
	int get_triangle_count(const CL_String &semantic) const;
	std::vector<CL_Collada_Source> sources;
	CL_Collada_Vertices vertices;
	std::vector<CL_Collada_Triangles> triangles;
};

CL_Collada_Mesh::CL_Collada_Mesh()
{
}

CL_Collada_Mesh::CL_Collada_Mesh(CL_DomElement &mesh_element, std::vector<CL_Collada_Material> &library_materials) : impl(new CL_Collada_Mesh_Impl())
{
	impl->load_sources(mesh_element);
	impl->load_vertices(mesh_element);
	impl->load_triangles(mesh_element, library_materials);
}

std::vector<CL_Collada_Source> &CL_Collada_Mesh::get_sources()
{
	return impl->sources;
}

std::vector<CL_Collada_Triangles> &CL_Collada_Mesh::get_triangles()
{
	return impl->triangles;
}

void CL_Collada_Mesh_Impl::load_sources(CL_DomElement &mesh_element)
{
	CL_DomNode cur_child(mesh_element.get_first_child());
	while (!cur_child.is_null())
	{
			if(cur_child.get_node_name() == "source")
			{
				CL_DomElement source_element = cur_child.to_element();
				sources.push_back( CL_Collada_Source(source_element));
			}
			cur_child = cur_child.get_next_sibling();
	}
}

void CL_Collada_Mesh_Impl::load_vertices(CL_DomElement &mesh_element)
{
	CL_DomElement vertices_element = mesh_element.named_item("vertices").to_element();
	if (vertices_element.is_null())
		throw CL_Exception("vertices element missing");

	vertices = CL_Collada_Vertices(vertices_element, sources);
}

void CL_Collada_Mesh_Impl::load_triangles(CL_DomElement &mesh_element, std::vector<CL_Collada_Material> &library_materials)
{
	CL_DomNode cur_child(mesh_element.get_first_child());
	while (!cur_child.is_null())
	{
			CL_DomString node_name = cur_child.get_node_name();
			if((node_name == "polylist") || ( node_name == "triangles"))
			{
				CL_DomElement element = cur_child.to_element();
				triangles.push_back( CL_Collada_Triangles(element, sources, vertices, library_materials));
			}
			cur_child = cur_child.get_next_sibling();
	}
}

bool CL_Collada_Mesh::is_null()
{
	return impl.is_null();
}

void CL_Collada_Mesh::create_vertices(CL_Vec3f *destination, int stride, const CL_String &semantic, std::vector<CL_Collada_Triangles> &surface_list)
{
	impl->create_vertices(destination, stride, semantic, surface_list);
}

void CL_Collada_Mesh::create_vertices(CL_Vec2f *destination, int stride, const CL_String &semantic, std::vector<CL_Collada_Triangles> &surface_list)
{
	impl->create_vertices(destination, stride, semantic, surface_list);
}

void CL_Collada_Mesh::create_vertices_normal(CL_Vec3f *destination, int stride, const CL_String &semantic, const CL_Angle &smoothing_angle)
{
	impl->create_vertices_normal(destination, stride, semantic, smoothing_angle);
}

void CL_Collada_Mesh::create_vertices_texcoords(CL_Vec2f *destination, int stride, const CL_String &semantic)
{
	impl->create_vertices_texcoords(destination, stride, semantic);
}

void CL_Collada_Mesh_Impl::create_vertices(CL_Vec3f *destination, int stride, const CL_String &semantic, std::vector<CL_Collada_Triangles> &surface_list)
{
	std::vector<CL_Collada_Triangles>::size_type triangles_size = triangles.size();
	if (!triangles_size)
		return;

	surface_list.reserve(surface_list.size() + triangles_size);

	if (!stride)
		stride = sizeof(CL_Vec3f);

	char *work_ptr = (char *) destination;

	for (std::vector<CL_Collada_Triangles>::size_type cnt=0; cnt< triangles_size; cnt++)
	{
		CL_Collada_Triangles &triangle = triangles[cnt];
		int actual_num_triangles;
		if (triangle.is_semantic(semantic))
		{
			actual_num_triangles = triangle.get_count();
		}
		else
		{
			actual_num_triangles = 0;
		}

		if (!actual_num_triangles)	// Continue now, if no triangles found
		{
			surface_list.push_back(CL_Collada_Triangles());
			continue;
		}

		surface_list.push_back(triangle);

		triangle.create_vertices( (CL_Vec3f *) work_ptr, stride, semantic);
		work_ptr += stride * actual_num_triangles * 3;
	}

}

void CL_Collada_Mesh_Impl::create_vertices(CL_Vec2f *destination, int stride, const CL_String &semantic, std::vector<CL_Collada_Triangles> &surface_list)
{
	std::vector<CL_Collada_Triangles>::size_type triangles_size = triangles.size();
	if (!triangles_size)
		return;

	surface_list.reserve(surface_list.size() + triangles_size);

	if (!stride)
		stride = sizeof(CL_Vec2f);

	char *work_ptr = (char *) destination;

	for (std::vector<CL_Collada_Triangles>::size_type cnt=0; cnt< triangles_size; cnt++)
	{
		CL_Collada_Triangles &triangle = triangles[cnt];
		int actual_num_triangles;
		if (triangle.is_semantic(semantic))
		{
			actual_num_triangles = triangle.get_count();
		}
		else
		{
			actual_num_triangles = 0;
		}
		if (!actual_num_triangles)	// Continue now, if no triangles found
		{
			surface_list.push_back(CL_Collada_Triangles());
			continue;
		}

		surface_list.push_back(triangle);

		triangle.create_vertices( (CL_Vec2f *) work_ptr, stride, semantic);
		work_ptr += stride * actual_num_triangles * 3;
	}

}

void CL_Collada_Mesh_Impl::create_vertices_normal(CL_Vec3f *destination, int stride, const CL_String &semantic, const CL_Angle &smoothing_angle)
{
	std::vector<CL_Collada_Triangles>::size_type triangles_size = triangles.size();
	if (!triangles_size)
		return;

	if (!stride)
		stride = sizeof(CL_Vec3f);

	char *work_ptr = (char *) destination;

	for (std::vector<CL_Collada_Triangles>::size_type cnt=0; cnt< triangles_size; cnt++)
	{
		CL_Collada_Triangles &triangle = triangles[cnt];
		int actual_num_triangles;
		if (triangle.is_semantic(semantic))
		{
			actual_num_triangles = triangle.get_count();
		}
		else
		{
			actual_num_triangles = 0;
		}
		if (!actual_num_triangles)	// Continue now, if no triangles found
			continue;

		triangle.create_vertices_normal( (CL_Vec3f *) work_ptr, stride, semantic, smoothing_angle);
		work_ptr += stride * actual_num_triangles * 3;
	}

}

void CL_Collada_Mesh_Impl::create_vertices_texcoords(CL_Vec2f *destination, int stride, const CL_String &semantic)
{
	std::vector<CL_Collada_Triangles>::size_type triangles_size = triangles.size();
	if (!triangles_size)
		return;

	if (!stride)
		stride = sizeof(CL_Vec2f);

	char *work_ptr = (char *) destination;

	for (std::vector<CL_Collada_Triangles>::size_type cnt=0; cnt< triangles_size; cnt++)
	{
		CL_Collada_Triangles &triangle = triangles[cnt];
		int actual_num_triangles;
		if (triangle.is_semantic(semantic))
		{
			actual_num_triangles = triangle.get_count();
		}
		else
		{
			actual_num_triangles = 0;
		}
		if (!actual_num_triangles)	// Continue now, if no triangles found
			continue;

		triangle.create_vertices_texcoords( (CL_Vec2f *) work_ptr, stride, semantic);
		work_ptr += stride * actual_num_triangles * 2;
	}
}


int CL_Collada_Mesh::get_triangle_count(const CL_String &semantic) const
{
	return impl->get_triangle_count(semantic);
}

int CL_Collada_Mesh_Impl::get_triangle_count(const CL_String &semantic) const
{
	int total_num_triangles = 0;

	std::vector<CL_Collada_Triangles>::size_type triangles_size = triangles.size();
	if (!triangles_size)
		return 0;

	for (std::vector<CL_Collada_Triangles>::size_type cnt=0; cnt< triangles_size; cnt++)
	{
		const CL_Collada_Triangles &triangle = triangles[cnt];
		if (triangle.is_semantic(semantic))
		{
			total_num_triangles += triangle.get_count();
		}
	}
	return total_num_triangles;
}

