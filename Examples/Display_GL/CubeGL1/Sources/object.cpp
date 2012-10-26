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
**    Mark Page
*/

#include "precomp.h"
#include "object.h"

Object::Object()
{
	m_pPolygon = NULL;
	m_NumPolygon = 0;
}

Object::~Object()
{
	Destroy();
}

ObjectPolygon::ObjectPolygon()
{
	m_pVertex = NULL;
	m_NumVertex = 0;
}

ObjectPolygon::~ObjectPolygon()
{
	Destroy();
}

void ObjectPolygon::Destroy()
{
	if (m_pVertex)
	{
		delete[] m_pVertex;
	}
}

ObjectVertex::ObjectVertex()
{
	m_pPoly = NULL;
}

void Object::Destroy()
{
	if (m_pPolygon)
	{
		delete[] m_pPolygon;
	}
}

void Object::Init(int num_polygon)
{
	Destroy();
	m_NumPolygon = num_polygon;
	m_pPolygon = new(ObjectPolygon[num_polygon]);
}

void ObjectPolygon::Init(int num_vertex)
{
	Destroy();
	m_NumVertex = num_vertex;
	m_pVertex = new ObjectVertex[num_vertex];

	for (int cnt=0; cnt<num_vertex; cnt++)
	{
		m_pVertex[cnt].m_pPoly = this;
	}
}

void Object::CreateCube(float size, int num_segments)
{
	const int num_cube_sides = 6;
	Init(num_cube_sides * num_segments * num_segments);

	static signed char face_offset[num_cube_sides][3] =
	{
		{-1, 1, 1},
		{-1, 1, 1},
		{-1, 1, 1},
		{1, -1, -1},
		{1, -1, -1},
		{1, -1, -1}
	};

	static signed char face_delta_horz[num_cube_sides][3] =
	{
		{0, 0, -1},
		{0, -1, 0},
		{1, 0, 0},
		{-1, 0, 0},
		{0, 0, 1},
		{0, 1, 0}
	};

	static signed char face_delta_vert[num_cube_sides][3] =
	{
		{0, -1, 0},
		{1, 0, 0},
		{0, 0, -1},
		{0, 1, 0},
		{-1, 0, 0},
		{0, 0, 1}
	};
	const int texture_face_number = 1;

	static char face_delta[num_cube_sides][3];

	ObjectPolygon *pptr = m_pPolygon;

	// For each cube face
	for (int outer_cnt = 0; outer_cnt< num_cube_sides; outer_cnt++)
	{
		clan::Vec3f offset(face_offset[outer_cnt][0], face_offset[outer_cnt][1],face_offset[outer_cnt][2]);
		offset.x *= size/2.0f;
		offset.y *= size/2.0f;
		offset.z *= size/2.0f;

		// For each vertical segment
		for (int vert_cnt = 0; vert_cnt < num_segments; vert_cnt++)
		{
			// Calculate point vertical position
			float offset_v;
			offset_v = (vert_cnt * size) / (float) num_segments;
			clan::Vec3f vert_offset_a;
			vert_offset_a.x = offset.x + face_delta_vert[outer_cnt][0] * offset_v;
			vert_offset_a.y = offset.y + face_delta_vert[outer_cnt][1] * offset_v;
			vert_offset_a.z = offset.z + face_delta_vert[outer_cnt][2] * offset_v;

			// Calculate point vertical position + 1
			offset_v = ((vert_cnt+1) * size) / (float) num_segments;
			clan::Vec3f vert_offset_b;
			vert_offset_b.x = offset.x + face_delta_vert[outer_cnt][0] * offset_v;
			vert_offset_b.y = offset.y + face_delta_vert[outer_cnt][1] * offset_v;
			vert_offset_b.z = offset.z + face_delta_vert[outer_cnt][2] * offset_v;

			// Calculate texture vertical position
			clan::Vec3f texture_vert_offset_a;
			offset_v = (vert_cnt) / (float) num_segments;
			texture_vert_offset_a.x = face_delta_vert[texture_face_number][0] * offset_v;
			texture_vert_offset_a.y = face_delta_vert[texture_face_number][1] * offset_v;
			texture_vert_offset_a.z = face_delta_vert[texture_face_number][2] * offset_v;

			// Calculate texture vertical position + 1
			clan::Vec3f texture_vert_offset_b;
			offset_v = (vert_cnt+1) / (float) num_segments;
			texture_vert_offset_b.x = face_delta_vert[texture_face_number][0] * offset_v;
			texture_vert_offset_b.y = face_delta_vert[texture_face_number][1] * offset_v;
			texture_vert_offset_b.z = face_delta_vert[texture_face_number][2] * offset_v;

			// For each horizontal segment
			for (int horiz_cnt = 0; horiz_cnt < num_segments; horiz_cnt++, pptr++)
			{
				pptr->Init(4);
				ObjectVertex *vptr = pptr->m_pVertex;
				float offset_h;

				// Top Left point / texture point
				offset_h = ((horiz_cnt) * size) / (float) num_segments;
				vptr->m_Point.x = vert_offset_a.x + face_delta_horz[outer_cnt][0] * offset_h;
				vptr->m_Point.y = vert_offset_a.y + face_delta_horz[outer_cnt][1] * offset_h;
				vptr->m_Point.z = vert_offset_a.z + face_delta_horz[outer_cnt][2] * offset_h;
				vptr->m_OriginalPoint = vptr->m_Point;
				offset_h = -(horiz_cnt) / (float) num_segments;
				vptr->m_TexturePoint.x = texture_vert_offset_a.x + face_delta_horz[texture_face_number][0] * offset_h;
				vptr->m_TexturePoint.y = texture_vert_offset_a.y + face_delta_horz[texture_face_number][1] * offset_h;
				vptr->m_TexturePoint.z = texture_vert_offset_a.z + face_delta_horz[texture_face_number][2] * offset_h;

				vptr++;
				// Top Right point / texture point
				offset_h = ((horiz_cnt+1) * size) / (float) num_segments;
				vptr->m_Point.x = vert_offset_a.x + face_delta_horz[outer_cnt][0] * offset_h;
				vptr->m_Point.y = vert_offset_a.y + face_delta_horz[outer_cnt][1] * offset_h;
				vptr->m_Point.z = vert_offset_a.z + face_delta_horz[outer_cnt][2] * offset_h;
				vptr->m_OriginalPoint = vptr->m_Point;
				offset_h = -(horiz_cnt+1) / (float) num_segments;
				vptr->m_TexturePoint.x = texture_vert_offset_a.x + face_delta_horz[texture_face_number][0] * offset_h;
				vptr->m_TexturePoint.y = texture_vert_offset_a.y + face_delta_horz[texture_face_number][1] * offset_h;
				vptr->m_TexturePoint.z = texture_vert_offset_a.z + face_delta_horz[texture_face_number][2] * offset_h;

				vptr++;
				// Bottom Right point / texture point
				offset_h = ((horiz_cnt+1) * size) / (float) num_segments;
				vptr->m_Point.x = vert_offset_b.x + face_delta_horz[outer_cnt][0] * offset_h;
				vptr->m_Point.y = vert_offset_b.y + face_delta_horz[outer_cnt][1] * offset_h;
				vptr->m_Point.z = vert_offset_b.z + face_delta_horz[outer_cnt][2] * offset_h;
				vptr->m_OriginalPoint = vptr->m_Point;
				offset_h = -(horiz_cnt+1) / (float) num_segments;
				vptr->m_TexturePoint.x = texture_vert_offset_b.x + face_delta_horz[texture_face_number][0] * offset_h;
				vptr->m_TexturePoint.y = texture_vert_offset_b.y + face_delta_horz[texture_face_number][1] * offset_h;
				vptr->m_TexturePoint.z = texture_vert_offset_b.z + face_delta_horz[texture_face_number][2] * offset_h;

				vptr++;
				// Bottom Left point / texture point
				offset_h = ((horiz_cnt) * size) / (float) num_segments;
				vptr->m_Point.x = vert_offset_b.x + face_delta_horz[outer_cnt][0] * offset_h;
				vptr->m_Point.y = vert_offset_b.y + face_delta_horz[outer_cnt][1] * offset_h;
				vptr->m_Point.z = vert_offset_b.z + face_delta_horz[outer_cnt][2] * offset_h;
				vptr->m_OriginalPoint = vptr->m_Point;
				offset_h = -(horiz_cnt) / (float) num_segments;
				vptr->m_TexturePoint.x = texture_vert_offset_b.x + face_delta_horz[texture_face_number][0] * offset_h;
				vptr->m_TexturePoint.y = texture_vert_offset_b.y + face_delta_horz[texture_face_number][1] * offset_h;
				vptr->m_TexturePoint.z = texture_vert_offset_b.z + face_delta_horz[texture_face_number][2] * offset_h;

			}
		}
 	}
}

void Object::Draw(clan::GraphicContext_GL1 &gc, clan::Texture &texture_image)
{
	clan::Material_GL1 material;
	material.set_specular(clan::Colorf(0.0f, 0.0f, 0.0f, 1.0f));
	material.set_diffuse(clan::Colorf(0.0f, 0.0f, 0.0f, 1.0f));
	material.set_shininess(0.5);

	gc.set_material(material);
	gc.set_color_material(clan::cl_color_material_ambient_and_diffuse);

	ObjectPolygon *pptr = m_pPolygon;
	for (int poly_cnt=0; poly_cnt<m_NumPolygon; poly_cnt++, pptr++)
	{
		pptr->Draw(gc, texture_image);
	}
}

void ObjectPolygon::Draw(clan::GraphicContext &gc, clan::Texture &texture_image)
{
	ObjectVertex *vptr = m_pVertex;
	clan::Vec3f positions[6];
	clan::Vec3f texture_positions[6];
	clan::Vec3f normals[6];

	// We convert quads into triangles. This is silly, and this entire example should be rewritten to just use triangles

	if (m_NumVertex!=4) throw clan::Exception("Ooops");

	positions[0] = m_pVertex[0].m_Point;
	texture_positions[0] = m_pVertex[0].m_TexturePoint;
	normals[0] = m_Normal;
	positions[1] = m_pVertex[1].m_Point;
	texture_positions[1] = m_pVertex[1].m_TexturePoint;
	normals[1] = m_Normal;
	positions[2] = m_pVertex[2].m_Point;
	texture_positions[2] = m_pVertex[2].m_TexturePoint;
	normals[2] = m_Normal;

	positions[3] = m_pVertex[2].m_Point;
	texture_positions[3] = m_pVertex[2].m_TexturePoint;
	normals[3] = m_Normal;
	positions[4] = m_pVertex[3].m_Point;
	texture_positions[4] = m_pVertex[3].m_TexturePoint;
	normals[4] = m_Normal;
	positions[5] = m_pVertex[0].m_Point;
	texture_positions[5] = m_pVertex[0].m_TexturePoint;
	normals[5] = m_Normal;


	clan::PrimitivesArray prim_array(gc);

	gc.set_texture(0, texture_image);

	prim_array.set_attributes(clan::cl_attrib_position, positions);
	prim_array.set_attribute(clan::cl_attrib_color, clan::Colorf(1.0f,1.0f,1.0f, 1.0f));
	prim_array.set_attributes(clan::cl_attrib_texture_position, texture_positions);
	prim_array.set_attributes(clan::cl_attrib_normal, normals);
	gc.set_primitives_array(prim_array);

	gc.draw_primitives_array(clan::cl_triangles, 6);

	gc.reset_texture(0);

	gc.reset_primitives_array();
}

void Object::CalcPolygonNormal(void)
{
	ObjectPolygon *pptr = m_pPolygon;
	for (int poly_cnt=0; poly_cnt<m_NumPolygon; poly_cnt++, pptr++)
	{
		pptr->CalcNormal();
	}
}

void ObjectPolygon::CalcNormal(void)
{
	if (m_NumVertex < 3)
	{
		m_Normal = clan::Vec3f(0.0f, 0.0f, 0.0f);
		return;
	}

	clan::Vec3f point_a;
	clan::Vec3f point_b;

	point_a.x = m_pVertex[0].m_Point.x -  m_pVertex[2].m_Point.x;
	point_a.y = m_pVertex[0].m_Point.y -  m_pVertex[2].m_Point.y;
	point_a.z = m_pVertex[0].m_Point.z -  m_pVertex[2].m_Point.z;

	point_b.x = m_pVertex[1].m_Point.x -  m_pVertex[2].m_Point.x;
	point_b.y = m_pVertex[1].m_Point.y -  m_pVertex[2].m_Point.y;
	point_b.z = m_pVertex[1].m_Point.z -  m_pVertex[2].m_Point.z;

	clan::Vec3f point_cross;

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

	m_Normal = point_cross;
}
