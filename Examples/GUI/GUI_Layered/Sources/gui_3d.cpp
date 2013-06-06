/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
#include "gui_layered.h"
#include "GUI.h"
#include "app.h"

const char GUI_Texture_Shader_Vertex[] =
	"#version 150\n"
	"\n"
	"layout(std140) uniform UniformBlock \n"
	"{\n"
	"	uniform mat4 WorldToEye;\n"
	"	uniform mat4 EyeToProjection;\n"
	"	uniform mat3 NormalWorldToEye;\n"
	""
	"	uniform vec4 MaterialEmission;\n"
	"	uniform vec4 MaterialSpecular;\n"
	"	uniform float MaterialShininess;\n"
	"	uniform float MaterialTransparency;\n"
	"\n"
	"	uniform vec4 LightVector;\n"
	"	uniform vec4 LightHalfVector;\n"
	"	uniform vec4 LightSpecular;\n"
	"	uniform vec4 LightDiffuse;\n"
	"	uniform vec4 LightAmbient;\n"
	"};\n"
	"\n"
	"in vec3 AttrPositionInWorld;"
	"in vec3 AttrNormalInWorld;"
	"in vec2 AttrTexCoord;"
	"\n"
	"out vec3 NormalInEye; \n"
	"out vec3 PositionInWorld; \n"
	"out vec4 PositionInEye;\n"
	"out vec2 TexCoord;\n"
	""
	"void main()"
	"{"
	"	PositionInEye = WorldToEye * vec4(AttrPositionInWorld.xyz, 1);\n"
	"	NormalInEye = normalize( NormalWorldToEye * AttrNormalInWorld);\n"
	"	PositionInWorld = AttrPositionInWorld;\n"
	"	TexCoord = AttrTexCoord;"
	"	gl_Position = EyeToProjection * PositionInEye;\n"
	"}"
	;

const char GUI_Texture_Shader_Fragment[] =
	"#version 150\n"
	"\n"
	"layout(std140) uniform UniformBlock \n"
	"{\n"
	"	uniform mat4 WorldToEye;\n"
	"	uniform mat4 EyeToProjection;\n"
	"	uniform mat3 NormalWorldToEye;\n"
	""
	"	uniform vec4 MaterialEmission;\n"
	"	uniform vec4 MaterialSpecular;\n"
	"	uniform float MaterialShininess;\n"
	"	uniform float MaterialTransparency;\n"
	"\n"
	"	uniform vec4 LightVector;\n"
	"	uniform vec4 LightHalfVector;\n"
	"	uniform vec4 LightSpecular;\n"
	"	uniform vec4 LightDiffuse;\n"
	"	uniform vec4 LightAmbient;\n"
	"};\n"
	"\n"
	"uniform sampler2D Texture0;\n"
	"\n"
	"in vec3 NormalInEye; \n"
	"in vec3 PositionInWorld; \n"
	"in vec4 PositionInEye;\n"
	"in vec2 TexCoord;\n"
	"\n"
	"out vec4 FragColor;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	vec4 texture_color = texture2D(Texture0, TexCoord.xy);\n"
	"	if (texture_color.w < 0.01) discard;\n"
	"\n"
	"	vec3 eye = -normalize(PositionInEye.xyz); \n"
	"	vec4 diff = vec4(0); \n"
	"	vec4 spec = vec4(0); \n"
	"\n"
	"	vec3 eye_space_normal = normalize(NormalInEye);\n"
	"	float nDotL = max(0.0, dot(eye_space_normal, LightVector.xyz)); \n"
	"	float pf; \n"
	"	if (nDotL == 0.0)\n"
	"	{\n"
	"		pf = 0.0; \n"
	"	}else\n"
	"	{\n"
	"		float nDotHV = max(0.0, dot(eye_space_normal, LightHalfVector.xyz));\n"
	"		pf = pow(nDotHV, MaterialShininess);\n"
	"	}\n"
	"	spec += LightSpecular * pf; \n"
	"	diff += LightDiffuse * nDotL;\n"
	"	vec4 final_texture_color = texture_color;\n"
	"	FragColor = LightAmbient * final_texture_color + (diff + MaterialEmission) * final_texture_color +spec * MaterialSpecular;\n"
	"	FragColor.a = texture_color.w - MaterialTransparency;\n"
	"	FragColor.b = FragColor.b + 0.2;"	// Add a blue hue
	"}\n"
	;

bool GUI_Layered::run3d()
{
	canvas.flush();

	clan::GraphicContext gc = canvas.get_gc();

	// Control the sine wave wobble
	clan::ubyte64 current_time = clan::System::get_time();
	if (panel3d->is_animated())
		wobble_offset += ((float) (current_time - last_time)) / 2000.0f;
	last_time = current_time;

	current_zoffset = panel3d->get_zoffset();

	set_projection_matrix();

	world_to_eye = clan::Mat4f::identity();
	world_to_projection = eye_to_projection * world_to_eye;

	std::vector<clan::GUIWindowManagerTextureWindow> windows = wm.get_windows();
	std::vector<clan::GUIWindowManagerTextureWindow>::size_type index, size;
	size = windows.size();

	UniformBlock uniform_block;

	uniform_block.world_to_eye = world_to_eye;
	uniform_block.eye_to_projection = eye_to_projection;

	clan::Mat3f normal_world_to_eye = clan::Mat3f(world_to_eye);
	normal_world_to_eye.inverse();
	normal_world_to_eye.transpose();

	uniform_block.normal_world_to_eye[0] = clan::Vec4f(normal_world_to_eye[0], normal_world_to_eye[1], normal_world_to_eye[2], 0.0f);
	uniform_block.normal_world_to_eye[1] = clan::Vec4f(normal_world_to_eye[3], normal_world_to_eye[4], normal_world_to_eye[5], 0.0f);
	uniform_block.normal_world_to_eye[2] = clan::Vec4f(normal_world_to_eye[6], normal_world_to_eye[7], normal_world_to_eye[8], 0.0f);

	uniform_block.light_vector = clan::Vec4f(panel3d->get_light_position_x(), panel3d->get_light_position_y(), panel3d->get_light_position_z(), 0.0f).normalize3();
	uniform_block.light_half_vector = (uniform_block.light_vector + clan::Vec4f(0.0f, 0.0f, 1.0f, 0.0f)).normalize3();
	uniform_block.light_ambient = clan::Vec4f(0.2f, 0.2f, 0.2f, 1.0f);
	uniform_block.light_diffuse = clan::Vec4f(panel3d->get_light_diffuse(), panel3d->get_light_diffuse(), panel3d->get_light_diffuse(), 1.0f);
	uniform_block.light_specular = clan::Vec4f(panel3d->get_light_specular(), panel3d->get_light_specular(), panel3d->get_light_specular(), 1.0f);

	uniform_block.material_shininess = std::pow(2.0f, (10.0f * panel3d->get_material_shininess()) + 2.0f);
	uniform_block.material_emission = clan::Vec4f(panel3d->get_material_emission(), panel3d->get_material_emission(), panel3d->get_material_emission(), 1.0f);
	uniform_block.material_specular = clan::Vec4f(panel3d->get_material_specular(), panel3d->get_material_specular(), panel3d->get_material_specular(), 1.0f);
	uniform_block.material_transparency = panel3d->get_material_transparency();

	gpu_uniform_block.upload_data(gc, &uniform_block, 1);

	for (index = 0; index < size; index++)
	{
		clan::GUIWindowManagerTextureWindow window = windows[index];
		clan::Subtexture subtexture = window.get_texture();
		clan::Texture2D texture = subtexture.get_texture();
		texture.set_min_filter(clan::filter_linear);
		texture.set_mag_filter(clan::filter_linear);

		//clan::PNGProvider::save(texture.get_pixeldata(gc, clan::tf_rgba8), "save.png");

		clan::Rect window_geometry = window.get_geometry();
		clan::Rect subtexture_geometry = subtexture.get_geometry();
		clan::Rectf rect((float) window_geometry.left, (float) window_geometry.top, clan::Sizef(subtexture_geometry.get_size()));
		clan::Rectf texture_unit1_coords(subtexture_geometry);

		float gc_width = (float) gc.get_width();
		float gc_height = (float) gc.get_height();

		// Transform 2d geometry to 3d
		rect.left = ((rect.left * 2.0f) / gc_width) - 1.0f;
		rect.right = ((rect.right * 2.0f) / gc_width) - 1.0f;
		rect.top = ((rect.top * 2.0f) / gc_height) - 1.0f;
		rect.bottom = ((rect.bottom * 2.0f) / gc_height) - 1.0f;

		float dest_left = rect.left;
		float dest_right = rect.right;
		float dest_top = -rect.top;
		float dest_bottom = -rect.bottom;

		float texture_width = (float) texture.get_width();
		float texture_height = (float) texture.get_height();

		float texel_centre = 0.375f;

		float src_left = (texture_unit1_coords.left + texel_centre) / texture_width;
		float src_right = (texture_unit1_coords.right + texel_centre) / texture_width;
		float src_top = (texture_unit1_coords.top + texel_centre) / texture_height;
		float src_bottom = (texture_unit1_coords.bottom + texel_centre) / texture_height;

		clan::Vec3f positions[num_points];
		clan::Vec3f normals[num_points];
		clan::Vec2f tex1_coords[num_points];
		clan::Vec3f *pos_ptr = positions;
		clan::Vec2f *tex_ptr = tex1_coords;
		clan::Vec3f *normals_ptr = normals;

		clan::Vec3f point_a;
		clan::Vec3f point_b;
		clan::Vec3f point_c;
		clan::Vec3f last_triangle_normal_left;
		clan::Vec3f last_triangle_normal_right;
		clan::Vec3f this_triangle_normal_left;
		clan::Vec3f this_triangle_normal_right;
		clan::Vec3f next_triangle_normal_left;
		clan::Vec3f next_triangle_normal_right;
		clan::Vec3f normal;

		for (int segment_count_vertical = 0; segment_count_vertical < num_segments_vertical; segment_count_vertical++)
		{
			float seg_top = dest_top + (segment_count_vertical * (dest_bottom - dest_top) / num_segments_vertical);
			float seg_bottom = dest_top + ((segment_count_vertical+1) * (dest_bottom - dest_top) / num_segments_vertical);

			float tex_seg_top = src_top + (segment_count_vertical * (src_bottom- src_top) / num_segments_vertical);
			float tex_seg_bottom = src_top + ((segment_count_vertical+1) * (src_bottom- src_top) / num_segments_vertical);

			float seg_left;
			float seg_right;

			seg_left = dest_left + (-1 * (dest_right- dest_left) / num_segments_horizontal);
			seg_right = dest_left + ((-1+1) * (dest_right- dest_left) / num_segments_horizontal);

			point_a = clan::Vec3f(seg_left, seg_top, get_zpos_at_position(seg_left, seg_top));
			point_b = clan::Vec3f(seg_right, seg_top, get_zpos_at_position(seg_right, seg_top));
			point_c = clan::Vec3f(seg_left, seg_bottom, get_zpos_at_position(seg_left, seg_bottom));
			this_triangle_normal_left = clan::Vec3f::cross(point_b - point_a, point_c - point_a);

			point_a = clan::Vec3f(seg_left, seg_bottom, get_zpos_at_position(seg_left, seg_bottom));
			point_b = clan::Vec3f(seg_right, seg_top, get_zpos_at_position(seg_right, seg_top));
			point_c = clan::Vec3f(seg_right, seg_bottom, get_zpos_at_position(seg_right, seg_bottom));
			this_triangle_normal_right = clan::Vec3f::cross(point_b - point_a, point_c - point_a);

			seg_left = dest_left + (0 * (dest_right- dest_left) / num_segments_horizontal);
			seg_right = dest_left + ((0+1) * (dest_right- dest_left) / num_segments_horizontal);

			point_a = clan::Vec3f(seg_left, seg_top, get_zpos_at_position(seg_left, seg_top));
			point_b = clan::Vec3f(seg_right, seg_top, get_zpos_at_position(seg_right, seg_top));
			point_c = clan::Vec3f(seg_left, seg_bottom, get_zpos_at_position(seg_left, seg_bottom));
			next_triangle_normal_left = clan::Vec3f::cross(point_b - point_a, point_c - point_a);

			point_a = clan::Vec3f(seg_left, seg_bottom, get_zpos_at_position(seg_left, seg_bottom));
			point_b = clan::Vec3f(seg_right, seg_top, get_zpos_at_position(seg_right, seg_top));
			point_c = clan::Vec3f(seg_right, seg_bottom, get_zpos_at_position(seg_right, seg_bottom));
			next_triangle_normal_right = clan::Vec3f::cross(point_b - point_a, point_c - point_a);

			for (int segment_count_horizontal = 0; segment_count_horizontal < num_segments_horizontal; segment_count_horizontal++)
			{
				last_triangle_normal_left = this_triangle_normal_left;
				last_triangle_normal_right = this_triangle_normal_right;
				this_triangle_normal_left = next_triangle_normal_left;
				this_triangle_normal_right = next_triangle_normal_right;

				seg_left = dest_left + (segment_count_horizontal * (dest_right- dest_left) / num_segments_horizontal);
				seg_right = dest_left + ((segment_count_horizontal+1) * (dest_right- dest_left) / num_segments_horizontal);

				*(pos_ptr++) = clan::Vec3f(seg_left, seg_top, get_zpos_at_position(seg_left, seg_top));
				*(pos_ptr++) = clan::Vec3f(seg_right, seg_top, get_zpos_at_position(seg_right, seg_top));
				*(pos_ptr++) = clan::Vec3f(seg_left, seg_bottom, get_zpos_at_position(seg_left, seg_bottom));
				*(pos_ptr++) = clan::Vec3f(seg_left, seg_bottom, get_zpos_at_position(seg_left, seg_bottom));
				*(pos_ptr++) = clan::Vec3f(seg_right, seg_top, get_zpos_at_position(seg_right, seg_top));
				*(pos_ptr++) = clan::Vec3f(seg_right, seg_bottom, get_zpos_at_position(seg_right, seg_bottom));

				seg_left = dest_left + ((segment_count_horizontal+1) * (dest_right- dest_left) / num_segments_horizontal);
				seg_right = dest_left + ((segment_count_horizontal+2) * (dest_right- dest_left) / num_segments_horizontal);

				point_a = clan::Vec3f(seg_left, seg_top, get_zpos_at_position(seg_left, seg_top));
				point_b = clan::Vec3f(seg_right, seg_top, get_zpos_at_position(seg_right, seg_top));
				point_c = clan::Vec3f(seg_left, seg_bottom, get_zpos_at_position(seg_left, seg_bottom));
				next_triangle_normal_left = clan::Vec3f::cross(point_b - point_a, point_c - point_a);

				point_a = clan::Vec3f(seg_left, seg_bottom, get_zpos_at_position(seg_left, seg_bottom));
				point_b = clan::Vec3f(seg_right, seg_top, get_zpos_at_position(seg_right, seg_top));
				point_c = clan::Vec3f(seg_right, seg_bottom, get_zpos_at_position(seg_right, seg_bottom));
				next_triangle_normal_right = clan::Vec3f::cross(point_b - point_a, point_c - point_a);

				normal = last_triangle_normal_left + last_triangle_normal_right + this_triangle_normal_left;
				*(normals_ptr++) = normal.normalize();

				normal = this_triangle_normal_left + this_triangle_normal_right + next_triangle_normal_left;
				*(normals_ptr++) = normal.normalize();

				normal = last_triangle_normal_right + this_triangle_normal_left + this_triangle_normal_right;
				*(normals_ptr++) = normal.normalize();

				normal = last_triangle_normal_right + this_triangle_normal_left + this_triangle_normal_right;
				*(normals_ptr++) = normal.normalize();

				normal = this_triangle_normal_left + this_triangle_normal_right + next_triangle_normal_left;
				*(normals_ptr++) = normal.normalize();

				normal = this_triangle_normal_right + next_triangle_normal_left + next_triangle_normal_right;
				*(normals_ptr++) = normal.normalize();

				float tex_seg_left = src_left + (segment_count_horizontal * (src_right- src_left) / num_segments_horizontal);
				float tex_seg_right = src_left + ((segment_count_horizontal+1) * (src_right- src_left) / num_segments_horizontal);

				*(tex_ptr++) = clan::Vec2f(tex_seg_left, tex_seg_top);
				*(tex_ptr++) = clan::Vec2f(tex_seg_right, tex_seg_top);
				*(tex_ptr++) = clan::Vec2f(tex_seg_left, tex_seg_bottom);
				*(tex_ptr++) = clan::Vec2f(tex_seg_left, tex_seg_bottom);
				*(tex_ptr++) = clan::Vec2f(tex_seg_right, tex_seg_top);
				*(tex_ptr++) = clan::Vec2f(tex_seg_right, tex_seg_bottom);
			}
		}

		attr_position_in_world.upload_data(gc, 0, positions, num_points);
		attr_normal_in_world.upload_data(gc, 0, normals, num_points);
		attr_tex_coord.upload_data(gc, 0, tex1_coords, num_points);

		gui_shader_texture.set(texture);
		gui_shader.draw(gc);
	}

	return true;
}

float GUI_Layered::get_zpos_at_position(float xpos, float ypos)
{
	float value = xpos;

	value = sin(wobble_offset + 1.5f * clan::PI * (value+clan::PI/4.0f));

	value = current_zoffset - (value / 2.0f);
	return value;
}

void GUI_Layered::set_projection_matrix()
{
	float fov = 2.0f * atan2(1.0f, lens_zoom);
	float aspect = 1.0f;
	float width = (float) canvas.get_width();
	float height = (float) canvas.get_height();

	if (height)
		aspect = ( width * lens_aspect) / height;

	fov = (fov * 180.0f) / clan::PI;
	eye_to_projection = clan::Mat4f::perspective( fov, aspect, lens_near, lens_far, clan::handed_left, canvas.get_gc().get_clip_z_range());
}

void GUI_Layered::wm_input_intercept(clan::InputEvent &input_event)
{
	float gc_width = (float) canvas.get_width();
	float gc_height = (float) canvas.get_height();

	// Transform points to range -1.0f to 1.0f
	float mouse_x = ((2.0f * input_event.mouse_pos.x) / gc_width) - 1.0f;
	float mouse_y = ((2.0f * input_event.mouse_pos.y) / gc_height) - 1.0f;
	mouse_y = -mouse_y;

	clan::Mat4d world_to_projection_double = clan::Mat4d(world_to_projection);
	clan::Mat4d inverse_matrix = world_to_projection_double.inverse();
	clan::Vec3f point;
	float mouse_z;
	float mouse_low_z = 0.5f;
	float mouse_high_z = 1.5f;

	for (int max_iterations = 0; max_iterations < 100; ++max_iterations)
	{
		// Find the Z position of the panel at this 2d point
		// Keep bisecting, until we have found the correct Z
		mouse_z = ( mouse_high_z + mouse_low_z ) / 2.0f;
		point = transform_point(clan::Vec3d( mouse_x, mouse_y, mouse_z), inverse_matrix);
		float point_z = get_zpos_at_position(point.x, point.y);

		float diff = point_z - point.z;
		if (diff < 0.0f)
			diff = -diff;

		if (diff <= 0.00001)
			break;

		if (point_z > point.z)
		{
			mouse_low_z = mouse_z;
		}
		else
		{
			mouse_high_z = mouse_z;
		}
	}

	saved_x1 = mouse_x;
	saved_x2 = point.x;
	saved_y1 = mouse_y;
	saved_y2 = point.y;
	saved_z1 = mouse_z;
	saved_z2 = point.z;

	input_event.mouse_pos.x = (int) ((((saved_x2) + 1.0f) * gc_width ) / 2.0f);
	input_event.mouse_pos.y = (int) ((((-saved_y2) + 1.0f) * gc_height ) / 2.0f);
}

clan::Vec3f GUI_Layered::transform_point(const clan::Vec3d &src_point, const clan::Mat4d &matrix)
{
	clan::Vec3d dest;

	dest.x = src_point.x * matrix.matrix[0 + 0*4] +
			src_point.y * matrix.matrix[0 + 1*4] +
			src_point.z * matrix.matrix[0 + 2*4] +
			matrix.matrix[0 + 3*4];

	dest.y = src_point.x * matrix.matrix[1 + 0*4] +
			src_point.y * matrix.matrix[1 + 1*4] +
			src_point.z * matrix.matrix[1 + 2*4] +
			matrix.matrix[1 + 3*4];

	dest.z = src_point.x * matrix.matrix[2 + 0*4] +
			src_point.y * matrix.matrix[2 + 1*4] +
			src_point.z * matrix.matrix[2 + 2*4] +
			matrix.matrix[2 + 3*4];

	double w = src_point.x * matrix.matrix[3 + 0*4] +
			src_point.y * matrix.matrix[3 + 1*4] +
			src_point.z * matrix.matrix[3 + 2*4] +
			matrix.matrix[3 + 3*4];

	if (w != 0.0)
	{
		dest.x /= w;
		dest.y /= w;
		dest.z /= w;
	}

	return dest;
}

void GUI_Layered::setup_shader()
{
	clan::GraphicContext gc = canvas.get_gc();

	gpu_uniform_block = clan::UniformVector<UniformBlock>(gc, 1);

	attr_position_in_world = clan::VertexArrayVector<clan::Vec3f>(gc, num_points);
	attr_normal_in_world = clan::VertexArrayVector<clan::Vec3f>(gc, num_points);
	attr_tex_coord = clan::VertexArrayVector<clan::Vec2f>(gc, num_points);

	clan::ShaderEffectDescription desc;

	desc.set_vertex_shader(GUI_Texture_Shader_Vertex);
	desc.set_fragment_shader(GUI_Texture_Shader_Fragment);
	desc.set_uniform_block("UniformBlock", gpu_uniform_block);
	desc.set_attribute("AttrPositionInWorld", attr_position_in_world);
	desc.set_attribute("AttrNormalInWorld", attr_normal_in_world);
	desc.set_attribute("AttrTexCoord", attr_tex_coord);
	desc.set_frag_data_to_back_buffer("FragColor");
	desc.set_texture("Texture0", gui_shader_texture);
	desc.set_draw_count(num_points);

	gui_shader = clan::ShaderEffect(gc, desc);
}
