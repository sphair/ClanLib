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
#include "collada_triangles.h"
#include "collada_input_shared.h"
#include "collada_material.h"
#include "collada_source.h"

class CL_Collada_Triangles_Impl
{
public:
	CL_Collada_Triangles_Impl() : triangle_count(0), stride(0) {}

	void load_polylist(CL_DomElement &polylist_element, bool contains_vcount, std::vector<CL_Collada_Source> &sources, CL_Collada_Vertices &vertices, std::vector<CL_Collada_Material> &library_materials);
	CL_Collada_Input_Shared &get_input(const CL_String &semantic);
	void create_vertices(CL_Vec3f *destination, int stride, const CL_String &semantic);
	void create_vertices(CL_Vec2f *destination, int stride, const CL_String &semantic);
	void create_vertices_normal(CL_Vec3f *destination, int stride, const CL_String &semantic, const CL_Angle &smoothing_angle);
	void create_vertices_texcoords(CL_Vec2f *destination, int stride, const CL_String &semantic);

	CL_String name;
	unsigned int triangle_count;
	int stride;
	CL_Collada_Material material;

	std::vector<int> primitive;
	std::vector<CL_Collada_Input_Shared> inputs;

private:
	void load_inputs(CL_DomElement &polylist_element, std::vector<CL_Collada_Source> &sources, CL_Collada_Vertices &vertices);
	void validate_vcount(CL_DomElement &vcount_element);
	void load_primitive(CL_DomElement &primitive_element);
	void calculate_face_normals( std::vector<CL_Vec3f> &normals, std::vector<CL_Vec3f> &pos_array, unsigned int primitive_offset);
	void generate_point_facelist( std::vector<int> &face_offsets, std::vector<int> &faces, unsigned int total_points, unsigned int primitive_offset);
	void calculate_point_normals( std::vector<int> &face_offsets, std::vector<int> &faces, std::vector<CL_Vec3f> &face_normals, CL_Vec3f *destination, int stride, float smooth_threshold, unsigned int primitive_offset );
	void calculate_texcoords( std::vector<int> &face_offsets, std::vector<int> &faces, std::vector<CL_Vec3f> &pos_array, std::vector<CL_Vec3f> &face_normals, CL_Vec2f *destination, int stride, unsigned int primitive_offset );

};

CL_Collada_Triangles::CL_Collada_Triangles() : impl(new CL_Collada_Triangles_Impl())
{
}

CL_Collada_Triangles::CL_Collada_Triangles(CL_DomElement &element, std::vector<CL_Collada_Source> &sources, CL_Collada_Vertices &vertices, std::vector<CL_Collada_Material> &library_materials) : impl(new CL_Collada_Triangles_Impl())
{
	bool contains_vcount;
	if (element.get_tag_name() == "polylist")
	{
		contains_vcount = true;
	}
	else
	{
		contains_vcount = false;
	}

	impl->load_polylist(element, contains_vcount, sources, vertices, library_materials);
}

void CL_Collada_Triangles_Impl::load_polylist(CL_DomElement &polylist_element, bool contains_vcount, std::vector<CL_Collada_Source> &sources, CL_Collada_Vertices &vertices, std::vector<CL_Collada_Material> &library_materials)
{
	name = polylist_element.get_attribute("name");

	// Find material	
	if (polylist_element.has_attribute("material"))
	{
		CL_String material_name = polylist_element.get_attribute("material");

			std::vector<CL_Collada_Material>::size_type size, cnt;
			size = library_materials.size();
			for (cnt=0; cnt< size; cnt++)
			{
				if (library_materials[cnt].get_id() == material_name)
				{
					material = library_materials[cnt];
					break;
				}
			}
	
		if (material.is_null())
			throw CL_Exception("Unable to find material");

	}

	triangle_count = polylist_element.get_attribute_int("count", 0);

	load_inputs(polylist_element, sources, vertices);

	if (contains_vcount)
	{
		CL_DomElement vcount_element = polylist_element.named_item("vcount").to_element();
		if (!vcount_element.is_null())
			validate_vcount(vcount_element);
	}

	CL_DomElement primitive_element = polylist_element.named_item("p").to_element();
	if (!primitive_element.is_null())
		load_primitive(primitive_element);
}

void CL_Collada_Triangles_Impl::validate_vcount(CL_DomElement &vcount_element)
{
	CL_String points = vcount_element.get_text();
	const CL_String::char_type *vcount_text = points.c_str();

	for (unsigned int cnt=0; cnt < triangle_count; cnt++)
	{
		if (!(*vcount_text))
			throw CL_Exception("VCount data count mismatch");

		int value = atoi(vcount_text);

		if (value != 3)
			throw CL_Exception("Only triangles are supported. Export your mesh as triangles please");

		while(*vcount_text)
		{
			if (*(vcount_text++) <= ' ')	// Find whitespace
				break;
		}

		while(*vcount_text)
		{
			if ((*vcount_text) > ' ')	// Find end of whitespace
				break;
			vcount_text++;
		}

	}
	if (*vcount_text)
			throw CL_Exception("VCount data count mismatch (end)");
}


void CL_Collada_Triangles_Impl::load_primitive(CL_DomElement &primitive_element)
{
	unsigned int count = stride * triangle_count * 3;

	primitive.resize(count);

	CL_String points = primitive_element.get_text();
	const CL_String::char_type *primitive_text = points.c_str();

	for (unsigned int cnt=0; cnt < count; cnt++)
	{
		if (!(*primitive_text))
			throw CL_Exception("Primitive data count mismatch");

		int value = atoi(primitive_text);
		primitive[cnt] = value;

		while(*primitive_text)
		{
			if (*(primitive_text++) <= ' ')	// Find whitespace
				break;
		}

		while(*primitive_text)
		{
			if ((*primitive_text) > ' ')	// Find end of whitespace
				break;
			primitive_text++;
		}

	}
	if (*primitive_text)
			throw CL_Exception("Primitive data count mismatch (end)");
}


void CL_Collada_Triangles_Impl::load_inputs(CL_DomElement &polylist_element, std::vector<CL_Collada_Source> &sources, CL_Collada_Vertices &vertices)
{
	stride = 1;
	CL_DomNode cur_child(polylist_element.get_first_child());
	while (!cur_child.is_null())
	{
			if(cur_child.get_node_name() == "input")
			{
				CL_DomElement input_element = cur_child.to_element();
				CL_Collada_Input_Shared new_input(input_element, sources, vertices);

				// Find the stride
				int offset = new_input.get_offset();
				offset++;	// Make comparible with stride (1 to x)
				if (offset > stride)
					stride = offset;

				inputs.push_back( new_input );
			}
			cur_child = cur_child.get_next_sibling();
	}
}

CL_String &CL_Collada_Triangles::get_name()
{
	return impl->name;
}

unsigned int CL_Collada_Triangles::get_count() const
{
	return impl->triangle_count;
}

int CL_Collada_Triangles::get_stride()
{
	return impl->stride;
}

CL_Collada_Material &CL_Collada_Triangles::get_material()
{
	return impl->material;
}

std::vector<int> &CL_Collada_Triangles::get_primitive()
{
	return impl->primitive;
}

std::vector<CL_Collada_Input_Shared> &CL_Collada_Triangles::get_inputs()
{
	return impl->inputs;
}

CL_Collada_Input_Shared &CL_Collada_Triangles::get_input(const CL_String &semantic)
{
	return impl->get_input(semantic);
}

CL_Collada_Input_Shared &CL_Collada_Triangles_Impl::get_input(const CL_String &semantic)
{
	std::vector<CL_Collada_Input_Shared>::size_type size = inputs.size();
	for (unsigned int cnt=0; cnt<size; cnt++)
	{
		if (inputs[cnt].get_semantic() == semantic)
		{
			return inputs[cnt];
		}
	}
	throw CL_Exception("Semantic not found");
}

bool CL_Collada_Triangles::is_semantic(const CL_String &semantic) const
{
	std::vector<CL_Collada_Input_Shared>::size_type size = impl->inputs.size();
	for (unsigned int cnt=0; cnt<size; cnt++)
	{
		if (impl->inputs[cnt].get_semantic() == semantic)
		{
			return true;
		}
	}
	return false;
}

void CL_Collada_Triangles::create_vertices(CL_Vec3f *destination, int stride, const CL_String &semantic)
{
	impl->create_vertices(destination, stride, semantic);
}

void CL_Collada_Triangles::create_vertices(CL_Vec2f *destination, int stride, const CL_String &semantic)
{
	impl->create_vertices(destination, stride, semantic);
}

void CL_Collada_Triangles::create_vertices_normal(CL_Vec3f *destination, int stride, const CL_String &semantic, const CL_Angle &smoothing_angle)
{
	impl->create_vertices_normal(destination, stride, semantic, smoothing_angle);
}

void CL_Collada_Triangles::create_vertices_texcoords(CL_Vec2f *destination, int stride, const CL_String &semantic)
{
	impl->create_vertices_texcoords(destination, stride, semantic);
}

void CL_Collada_Triangles_Impl::create_vertices(CL_Vec3f *destination, int stride, const CL_String &semantic)
{
	CL_Collada_Input_Shared &input = get_input(semantic);

	CL_Collada_Source &source = input.get_source();
	if (source.is_null())
	{
		throw CL_Exception("unsupported situation. fixme");
	}

	std::vector<CL_Vec3f> &pos_array = source.get_vec3f_array();

	if (!stride)
		stride = sizeof(CL_Vec3f);

	char *work_ptr = (char *) destination;

	int primitive_stride = this->stride;
	unsigned int offset = input.get_offset();

	unsigned int max = offset + (triangle_count * 3 * primitive_stride);
	for (unsigned int cnt=offset; cnt < max; cnt+=primitive_stride)
	{
		*( (CL_Vec3f *) work_ptr ) = pos_array[ primitive[cnt] ];
		work_ptr += stride;
	}
}

void CL_Collada_Triangles_Impl::create_vertices(CL_Vec2f *destination, int stride, const CL_String &semantic)
{
	CL_Collada_Input_Shared &input = get_input(semantic);

	CL_Collada_Source &source = input.get_source();
	if (source.is_null())
	{
		throw CL_Exception("unsupported situation. fixme");
	}

	std::vector<CL_Vec2f> &pos_array = source.get_vec2f_array();

	if (!stride)
		stride = sizeof(CL_Vec2f);

	char *work_ptr = (char *) destination;

	int primitive_stride = this->stride;
	unsigned int offset = input.get_offset();

	unsigned int max = offset + (triangle_count * 3 * primitive_stride);
	for (unsigned int cnt=offset; cnt < max; cnt+=primitive_stride)
	{
		*( (CL_Vec2f *) work_ptr ) = pos_array[ primitive[cnt] ];
		work_ptr += stride;
	}
}

// Appends to "normals"
void CL_Collada_Triangles_Impl::calculate_face_normals( std::vector<CL_Vec3f> &normals, std::vector<CL_Vec3f> &pos_array, unsigned int primitive_offset)
{
	CL_Vec3f point_a;
	CL_Vec3f point_b;
	CL_Vec3f point_cross;

	normals.reserve(normals.size() + triangle_count);

	int primitive_stride = this->stride;
	unsigned int offset = primitive_offset;

	unsigned int interval = 3 * primitive_stride;
	unsigned int max = offset + (triangle_count * interval);
	const unsigned int offset_vertex_a = 0;
	const unsigned int offset_vertex_b = primitive_stride;
	const unsigned int offset_vertex_c = primitive_stride*2;

	for (unsigned int vertex_cnt=offset; vertex_cnt < max; vertex_cnt+=interval)
	{
		point_a.x = pos_array[primitive[vertex_cnt + offset_vertex_a] ].x - pos_array[primitive[vertex_cnt + offset_vertex_c] ].x;
		point_a.y = pos_array[primitive[vertex_cnt + offset_vertex_a] ].y - pos_array[primitive[vertex_cnt + offset_vertex_c] ].y;
		point_a.z = pos_array[primitive[vertex_cnt + offset_vertex_a] ].z - pos_array[primitive[vertex_cnt + offset_vertex_c] ].z;

		point_b.x = pos_array[primitive[vertex_cnt + offset_vertex_b] ].x - pos_array[primitive[vertex_cnt + offset_vertex_c] ].x;
		point_b.y = pos_array[primitive[vertex_cnt + offset_vertex_b] ].y - pos_array[primitive[vertex_cnt + offset_vertex_c] ].y;
		point_b.z = pos_array[primitive[vertex_cnt + offset_vertex_b] ].z - pos_array[primitive[vertex_cnt + offset_vertex_c] ].z;

		point_cross.x = point_a.y * point_b.z - point_a.z * point_b.y;
		point_cross.y = point_a.z * point_b.x - point_a.x * point_b.z;
		point_cross.z = point_a.x * point_b.y - point_a.y * point_b.x;

		float size;
		size = (point_cross.x) * (point_cross.x);
		size += (point_cross.y) * (point_cross.y);
		size += (point_cross.z) * (point_cross.z);
		if (size > 0.0f)
		{
			size = sqrt(size);
			point_cross.x /= size;
			point_cross.y /= size;
			point_cross.z /= size;
		}

		normals.push_back(point_cross);
	}
}

// Generates face list from object positions and vertices
// "face_offsets" and "faces" are appended to
// face_offsets (length of total_points) is an offset into "faces"
// faces[offset + 0 ] = Number of connecting faces
// faces[offset + x ] = The connecting faces
void CL_Collada_Triangles_Impl::generate_point_facelist( std::vector<int> &face_offsets, std::vector<int> &faces, unsigned int total_points, unsigned int primitive_offset)
{

	face_offsets.reserve(face_offsets.size() + total_points);

	int primitive_stride = this->stride;
	unsigned int offset = primitive_offset;
	unsigned int max = offset + (primitive_stride * triangle_count * 3);
	unsigned int interval = 3 * primitive_stride;
	const unsigned int offset_vertex_a = 0;
	const unsigned int offset_vertex_b = primitive_stride;
	const unsigned int offset_vertex_c = primitive_stride*2;

	// Count the required storage size
	unsigned int faces_offset = faces.size();
	for (unsigned int point_cnt=0; point_cnt < total_points; point_cnt++)
	{
		int vertex_match = 0;
		for (unsigned int vertex_cnt=offset; vertex_cnt < max; vertex_cnt+=interval)
		{
			if (primitive[vertex_cnt+offset_vertex_a] == (point_cnt))
			{
				vertex_match++;
				continue;
			}
			if (primitive[vertex_cnt+offset_vertex_b] == (point_cnt))
			{
				vertex_match++;
				continue;
			}
			if (primitive[vertex_cnt+offset_vertex_c] == (point_cnt))
			{
				vertex_match++;
				continue;
			}
		}
		face_offsets.push_back(faces_offset);
		faces_offset += vertex_match + 1;	// An extra 1 for the number of faces to a point
	}

	faces.reserve(faces.size() + faces_offset);

	for (unsigned int point_cnt=0; point_cnt < total_points; point_cnt++)
	{
		unsigned int faces_offset = faces.size();
		unsigned int face_count_offset = faces_offset;
		faces.push_back(0);	// Storage for the number of faces connecting to this point

		int vertex_match = 0;
		for (unsigned int vertex_cnt=offset; vertex_cnt < max; vertex_cnt+=interval)
		{
			if (primitive[vertex_cnt+offset_vertex_a] == (point_cnt))
			{
				faces.push_back((vertex_cnt)/interval);	// Face number = vertex count / 3
				vertex_match++;
				continue;
			}
			if (primitive[vertex_cnt+offset_vertex_b] == (point_cnt))
			{
				faces.push_back((vertex_cnt)/interval);	// Face number = vertex count / 3
				vertex_match++;
				continue;
			}
			if (primitive[vertex_cnt+offset_vertex_c] == (point_cnt))
			{
				faces.push_back((vertex_cnt)/interval);	// Face number = vertex count / 3
				vertex_match++;
				continue;
			}
		}
		faces[face_count_offset] = vertex_match;
	}
}

void CL_Collada_Triangles_Impl::calculate_point_normals( std::vector<int> &face_offsets, std::vector<int> &faces, std::vector<CL_Vec3f> &face_normals, CL_Vec3f *destination, int stride, float smooth_threshold, unsigned int primitive_offset )
{
	if (!stride)
		stride = sizeof(CL_Vec3f);

	char *work_ptr = (char *) destination;

	unsigned int num_vertices = triangle_count * 3;
	if (!num_vertices)
		return;

	const int vertex_offset = 0;	// Not required at the moment

	int primitive_stride = this->stride;
	const int *vertices_ptr = &primitive[primitive_offset];
	for(unsigned int vertex_cnt=0; vertex_cnt<num_vertices; vertex_cnt++)
	{
		int face_offset = face_offsets[ *vertices_ptr ];
		vertices_ptr += primitive_stride;

		const int *face_list = &faces[face_offset];
		int num_points_to_face = *(face_list++);

		int current_face = (vertex_cnt+vertex_offset)/3;
		const CL_Vec3f &current_normal  = face_normals[ current_face ];
		CL_Vec3f normal(current_normal);
		//int number_included_faces = 1;

		for (int face_cnt = 0; face_cnt < num_points_to_face; face_cnt++)
		{
			int next_face = *(face_list++);
			if (next_face == current_face)
				continue;

			const CL_Vec3f &next_normal  = face_normals[ next_face ];

			// Ensure the angle is within the smoothing threshold
			if ((acos( next_normal.x * current_normal.x + next_normal.y * current_normal.y + next_normal.z * current_normal.z )) < smooth_threshold)
			{
				normal += next_normal;
			//	number_included_faces++;
			}
		}
		//normal /= (float) number_included_faces;
		normal.normalize();

		*( (CL_Vec3f *) work_ptr ) = normal;
		work_ptr += stride;
	}
}

void CL_Collada_Triangles_Impl::create_vertices_normal(CL_Vec3f *destination, int stride, const CL_String &semantic, const CL_Angle &smoothing_angle)
{
	CL_Collada_Input_Shared &input = get_input(semantic);

	CL_Collada_Source &source = input.get_source();
	if (source.is_null())
	{
		throw CL_Exception("unsupported situation. fixme");
	}

	std::vector<CL_Vec3f> &pos_array = source.get_vec3f_array();

	unsigned int primitive_offset = input.get_offset();
	std::vector<CL_Vec3f> face_normals;
	calculate_face_normals(face_normals, pos_array, primitive_offset);

	std::vector<int> face_offsets;
	std::vector<int> faces;

	generate_point_facelist(face_offsets, faces, pos_array.size(), primitive_offset);

	calculate_point_normals(face_offsets, faces, face_normals, destination, stride, smoothing_angle.to_radians(), primitive_offset );
}

void CL_Collada_Triangles_Impl::create_vertices_texcoords(CL_Vec2f *destination, int stride, const CL_String &semantic)
{
	CL_Collada_Input_Shared &input = get_input(semantic);

	CL_Collada_Source &source = input.get_source();
	if (source.is_null())
	{
		throw CL_Exception("unsupported situation. fixme");
	}

	std::vector<CL_Vec3f> &pos_array = source.get_vec3f_array();

	unsigned int primitive_offset = input.get_offset();
	std::vector<CL_Vec3f> face_normals;
	calculate_face_normals(face_normals, pos_array, primitive_offset);

	std::vector<int> face_offsets;
	std::vector<int> faces;

	generate_point_facelist(face_offsets, faces, pos_array.size(), primitive_offset);

	calculate_texcoords(face_offsets, faces, pos_array, face_normals, destination, stride, primitive_offset );
}

void CL_Collada_Triangles_Impl::calculate_texcoords( std::vector<int> &face_offsets, std::vector<int> &faces, std::vector<CL_Vec3f> &pos_array, std::vector<CL_Vec3f> &face_normals, CL_Vec2f *destination, int stride, unsigned int primitive_offset )
{
	if (!stride)
		stride = sizeof(CL_Vec2f);

	char *work_ptr = (char *) destination;

	unsigned int num_vertices = triangle_count * 3;
	if (!num_vertices)
		return;

	const int vertex_offset = 0;	// Not required at the moment


	int primitive_stride = this->stride;
	const int *vertices_ptr = &primitive[primitive_offset];
	for(unsigned int vertex_cnt=0; vertex_cnt<num_vertices; vertex_cnt++)
	{
		int face_offset = face_offsets[ *vertices_ptr ];
		vertices_ptr += primitive_stride;
		int current_face = (vertex_cnt+vertex_offset)/3;
		const CL_Vec3f &current_normal  = face_normals[ current_face ];

		//FIXME: int projection = ...;
		//FIXME: int axis = ...;
		int axis;
		//FIXME: if (projection == PROJ_CUBIC) ...
		axis = 0;
		if( fabs(current_normal.y) >= fabs(current_normal.x) && fabs(current_normal.y) >= fabs(current_normal.z) ) axis = 1;
		if( fabs(current_normal.z) >= fabs(current_normal.x) && fabs(current_normal.z) >= fabs(current_normal.y) ) axis = 2;

		CL_Vec3f point = pos_array[ primitive[vertex_cnt] ];

		//FIXME: point -= texture map center

		CL_Vec3f tmap_scale(0.5f, 0.5f, 0.5f);	//FIXME
		CL_Vec2f texture_position;
		if (axis == 0) { texture_position.x = (point.z / tmap_scale.z) + 0.5f;  texture_position.y = (point.y / tmap_scale.y) + 0.5f; }
		if (axis == 1) { texture_position.x = (point.x / tmap_scale.x) + 0.5f;  texture_position.y = (point.z / tmap_scale.z) + 0.5f; }
		if (axis == 2) { texture_position.x = (point.x / tmap_scale.x) + 0.5f;  texture_position.y = (point.y / tmap_scale.y) + 0.5f; }

		*( (CL_Vec2f *) work_ptr ) = texture_position;
		work_ptr += stride;
	}
}
