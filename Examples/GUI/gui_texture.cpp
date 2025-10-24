/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gui.h>
#include "gui_texture.h"
#include "GUI.h"
#include "app.h"

class LightSource
{
public:
	LightSource()
	: m_Ambient(0.0f, 0.0f, 0.0f, 1.0f), m_Diffuse(1.0f, 1.0f, 1.0f, 1.0f), m_Specular(1.0f, 1.0f, 1.0f, 1.0f),
	  m_Position(0.0f, 0.0f, 1.0f, 0.0f), m_SpotDirection(0.0f, 0.0f, -1.0f), m_SpotExponent(0.0f),
	  m_SpotCutoff(180.0f), m_ConstantAttenuation(1.0f), m_QuadraticAttenuation(0.0f)
	{
	}

	CL_Vec4f m_Ambient;
	CL_Vec4f m_Diffuse;
	CL_Vec4f m_Specular;
	CL_Vec4f m_Position;
	CL_Vec3f m_SpotDirection;
	float m_SpotExponent;
	float m_SpotCutoff;
	float m_ConstantAttenuation;
	float m_LinearAttenuation;
	float m_QuadraticAttenuation;
	
	CL_Vec4f get_halfvector() const { CL_Vec4f p = CL_Vec4f(0.0f, 0.0f, 1.0f, 0.0f) + m_Position; p.normalize3(); return p; }
	float get_spot_cos_cutoff() const { return (float) cos(m_SpotCutoff * 3.14159265f / 180.0f); }
};

const char GUI_Texture_Shader_Vertex[] =
	"#version 120\n"
	"\n"
	"attribute vec4 in_position;"
	"attribute vec3 in_normal;"
	"attribute vec2 in_texcoord;"
	"varying vec3 view_space_normal;"
	"varying vec3 world_space_normal;"
	"varying vec3 world_space_position;"
	"varying vec3 object_position;"
	"varying vec2 texcoord;"
	""
	"void main()"
	"{"
	"	gl_Position = gl_ModelViewProjectionMatrix * in_position;"
	"	world_space_normal = in_normal;"
	"	world_space_position = in_position.xyz;"
	"	view_space_normal = normalize( gl_NormalMatrix * in_normal);"
	"	object_position = vec3(gl_ModelViewMatrix * in_position);"
	"	texcoord.xy = in_texcoord.xy;"
	"}"
	;

const char GUI_Texture_Shader_Fragment[] =
	"#version 120\n"
	"\n"
	"varying vec3 view_space_normal;\n"
	"varying vec3 world_space_normal;\n"
	"varying vec3 world_space_position;\n"
	"varying vec3 object_position;\n"
	"varying vec2 texcoord;\n"
	"\n"
	"uniform sampler2D Texture0;\n"
	"\n"
	"uniform float material_shininess;\n"
	"uniform vec4 material_emission;\n"
	"uniform vec4 material_specular;\n"
	"uniform float material_transparency;\n"
	"uniform vec4 light_position;\n"
	"uniform vec4 light_half_vector;\n"
	"uniform vec4 light_specular;\n"
	"uniform vec4 light_diffuse;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	vec4 texture_color = texture2D(Texture0, texcoord.xy);\n"
	"	if (texture_color.w < 0.01) discard;\n"
	"\n"
	"	vec3 eye = -normalize(object_position);\n"
	"\n"
	"	float nDotL = max(0.0, dot(view_space_normal, light_position.xyz));\n"
	"	float pf;\n"
	"	if (nDotL == 0.0)\n"
	"	{\n"
	"		pf = 0.0;\n"
	"	}\n"
	"	else\n"
	"	{\n"
	"		float nDotHV = max(0.0, dot(view_space_normal, light_half_vector.xyz));\n"
	"		pf = pow(nDotHV, material_shininess);\n"
	"	}\n"
	"\n"
	"	vec4 specular = light_specular * pf;\n"
	"	vec4 diffuse = light_diffuse * nDotL;\n"
	"\n"
	"	vec4 final_color = vec4(texture_color.xyz, 1.0);\n"
	"	const vec4 ambient_light = vec4(0.05, 0.05, 0.05, 1.0);\n"
	"	gl_FragColor = ambient_light * final_color + \n"
	"		(diffuse + material_emission) * final_color + \n"
	"		(specular + material_specular);\n"
	"	gl_FragColor.a = texture_color.w - material_transparency;\n"
	"}\n"
	;

GUI_Texture::GUI_Texture(GUI *gui) : gui(gui), window_ptr(gui->get_app()->get_window()), wm(*window_ptr)
{
	active_3dmode = false;
	shader_set = false;

	CL_GUIManager *gui_manager = &gui->get_gui_manager();

	gui_manager->set_window_manager(&wm);

	wm.func_input_intercept().set(this, &GUI_Texture::wm_input_intercept);

	// Use a texture group to store all the gui textures
	// If this does not work, try 1024x1024 (or below)
	CL_GraphicContext gc = window_ptr->get_gc();
	CL_TextureGroup texture_group(gc, CL_Size(2048, 2048));
//	CL_TextureGroup texture_group(gc, CL_Size(0, 0));	// Use this line to force each gui window texture to be in a seperate CL_Texture
	wm.set_texture_group(texture_group);	// Note: This line is optional

	resources_gui = CL_ResourceManager(gui->get_resources_location());

	theme.set_resources(resources_gui);
	gui_manager->set_theme(&theme); 

	gui_manager->set_css_document(gui->get_theme_location());

	wm.func_repaint().set(this, &GUI_Texture::gui_repaint);

	pushbutton.reset(new PushButton(gui) );
	lineedit.reset(new LineEdit(gui) );
	checkbox.reset(new CheckBox(gui) );
	slider.reset(new Slider(gui) );
	radiobutton.reset(new RadioButton(gui) );
	scrollbar.reset(new ScrollBar(gui) );
	progressbar.reset(new ProgressBar(gui) );
	tabpage.reset(new TabPage(gui) );
	menubar.reset(new MenuBar(gui) );
	spin.reset(new Spin(gui) );
	combobox.reset(new ComboBox(gui) );
	manager.reset(new Manager(gui) );
	listview.reset(new ListView(gui) );
}

GUI_Texture::~GUI_Texture()
{
}

bool GUI_Texture::run()
{
	if (manager.get())
	{
		if (manager->is_3dmode())
		{
			return run3d();
		}
	}

	if (active_3dmode)
	{
		panel3d.reset();
		active_3dmode = false;
	}

	CL_GraphicContext gc = window_ptr->get_gc();
	
	std::vector<CL_GUIWindowManagerTextureWindow> windows = wm.get_windows();
	std::vector<CL_GUIWindowManagerTextureWindow>::size_type index, size;
	size = windows.size();

	for (index = 0; index < size; index++)
	{
		CL_GUIWindowManagerTextureWindow window = windows[index];
		CL_Subtexture texture = window.get_texture();
		CL_Image image(gc, texture);
		image.draw(gc, (float)window.get_geometry().left, (float)window.get_geometry().top);
	}

	return true;
}

bool GUI_Texture::run3d()
{
	// Check for initial setup
	if (!active_3dmode)
	{
		if (!shader_set)
			setup_shader();

		active_3dmode = true;
		wobble_offset = 0.0f;
		last_time = CL_System::get_time();
		lens_zoom = 3.2f;
		lens_near = 0.1f;
		lens_far = 10000.0f;
		lens_aspect = 1.0f;

		panel3d.reset(new Panel3D(gui) );

	}

	// Control the sine wave wobble
	unsigned int current_time = CL_System::get_time();
	if (panel3d->is_animated())
		wobble_offset += ((float) (current_time - last_time)) / 2000.0f;
	last_time = current_time;

	current_zoffset = panel3d->get_zoffset();

	CL_GraphicContext gc = window_ptr->get_gc();

	gc.set_map_mode(cl_user_projection);

	set_projection_matrix();

	modelview_matrix = CL_Mat4f::identity();
	modelview_matrix.matrix[2 + (4*2)] = -1.0f;
	gc.set_modelview(modelview_matrix);

	resultant_matrix = CL_Mat4f::multiply(modelview_matrix, projection_matrix);

	LightSource lightsource;
	lightsource.m_Specular = CL_Vec4f(panel3d->get_light_specular(), panel3d->get_light_specular(), panel3d->get_light_specular(), 1.0f);
	lightsource.m_Diffuse = CL_Vec4f(panel3d->get_light_diffuse(), panel3d->get_light_diffuse(), panel3d->get_light_diffuse(), 1.0f);
	lightsource.m_Position = CL_Vec4f( panel3d->get_light_position_x(), panel3d->get_light_position_y(), panel3d->get_light_position_z(), 0.0f);
	lightsource.m_Position.normalize3();
	lightsource.m_SpotCutoff = 180.0f;

	std::vector<CL_GUIWindowManagerTextureWindow> windows = wm.get_windows();
	std::vector<CL_GUIWindowManagerTextureWindow>::size_type index, size;
	size = windows.size();

	gc.set_program_object(gui_shader);

	gui_shader.set_uniform4f("light_position", lightsource.m_Position); 
	gui_shader.set_uniform4f("light_half_vector", lightsource.get_halfvector()); 
	gui_shader.set_uniform4f("light_specular", lightsource.m_Specular); 
	gui_shader.set_uniform4f("light_diffuse", lightsource.m_Diffuse); 
	gui_shader.set_uniform1f("material_shininess", pow(2.0f, (10.0f * panel3d->get_material_shininess()) + 2.0f)); 
	gui_shader.set_uniform4f("material_emission", CL_Vec4f(panel3d->get_material_emission(), panel3d->get_material_emission(), panel3d->get_material_emission(), 1.0f)); 
	gui_shader.set_uniform4f("material_specular", CL_Vec4f(panel3d->get_material_specular(), panel3d->get_material_specular(), panel3d->get_material_specular(), 1.0f)); 
	gui_shader.set_uniform1f("material_transparency", panel3d->get_material_transparency()); 

	for (index = 0; index < size; index++)
	{
		CL_GUIWindowManagerTextureWindow window = windows[index];
		CL_Subtexture subtexture = window.get_texture();
		CL_Texture texture = subtexture.get_texture();
		texture.set_min_filter(cl_filter_linear);
		texture.set_mag_filter(cl_filter_linear);

		CL_Rect window_geometry = window.get_geometry();
		CL_Rect subtexture_geometry = subtexture.get_geometry();
		CL_Rectf rect((float) window_geometry.left, (float) window_geometry.top, CL_Sizef(subtexture_geometry.get_size()));
		CL_Rectf texture_unit1_coords(subtexture_geometry);

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

		const int num_segments_horizontal = 32;
		const int num_segments_vertical = 1;
		const int num_points = 6 * num_segments_horizontal * num_segments_vertical;

		CL_Vec3f positions[num_points];
		CL_Vec3f normals[num_points];
		CL_Vec2f tex1_coords[num_points];
		CL_Vec3f *pos_ptr = positions;
		CL_Vec2f *tex_ptr = tex1_coords;
		CL_Vec3f *normals_ptr = normals;

		CL_Vec3f point_a;
		CL_Vec3f point_b;
		CL_Vec3f point_c;
		CL_Vec3f last_triangle_normal_left;
		CL_Vec3f last_triangle_normal_right;
		CL_Vec3f this_triangle_normal_left;
		CL_Vec3f this_triangle_normal_right;
		CL_Vec3f next_triangle_normal_left;
		CL_Vec3f next_triangle_normal_right;
		CL_Vec3f normal;

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

			point_a = CL_Vec3f(seg_left, seg_top, get_zpos_at_position(seg_left, seg_top));
			point_b = CL_Vec3f(seg_right, seg_top, get_zpos_at_position(seg_right, seg_top));
			point_c = CL_Vec3f(seg_left, seg_bottom, get_zpos_at_position(seg_left, seg_bottom));
			this_triangle_normal_left = CL_Vec3f::cross(point_b - point_a, point_c - point_a);

			point_a = CL_Vec3f(seg_left, seg_bottom, get_zpos_at_position(seg_left, seg_bottom));
			point_b = CL_Vec3f(seg_right, seg_top, get_zpos_at_position(seg_right, seg_top));
			point_c = CL_Vec3f(seg_right, seg_bottom, get_zpos_at_position(seg_right, seg_bottom));
			this_triangle_normal_right = CL_Vec3f::cross(point_b - point_a, point_c - point_a);

			seg_left = dest_left + (0 * (dest_right- dest_left) / num_segments_horizontal);
			seg_right = dest_left + ((0+1) * (dest_right- dest_left) / num_segments_horizontal);

			point_a = CL_Vec3f(seg_left, seg_top, get_zpos_at_position(seg_left, seg_top));
			point_b = CL_Vec3f(seg_right, seg_top, get_zpos_at_position(seg_right, seg_top));
			point_c = CL_Vec3f(seg_left, seg_bottom, get_zpos_at_position(seg_left, seg_bottom));
			next_triangle_normal_left = CL_Vec3f::cross(point_b - point_a, point_c - point_a);

			point_a = CL_Vec3f(seg_left, seg_bottom, get_zpos_at_position(seg_left, seg_bottom));
			point_b = CL_Vec3f(seg_right, seg_top, get_zpos_at_position(seg_right, seg_top));
			point_c = CL_Vec3f(seg_right, seg_bottom, get_zpos_at_position(seg_right, seg_bottom));
			next_triangle_normal_right = CL_Vec3f::cross(point_b - point_a, point_c - point_a);

			for (int segment_count_horizontal = 0; segment_count_horizontal < num_segments_horizontal; segment_count_horizontal++)
			{
				last_triangle_normal_left = this_triangle_normal_left;
				last_triangle_normal_right = this_triangle_normal_right;
				this_triangle_normal_left = next_triangle_normal_left;
				this_triangle_normal_right = next_triangle_normal_right;

				seg_left = dest_left + (segment_count_horizontal * (dest_right- dest_left) / num_segments_horizontal);
				seg_right = dest_left + ((segment_count_horizontal+1) * (dest_right- dest_left) / num_segments_horizontal);

				*(pos_ptr++) = CL_Vec3f(seg_left, seg_top, get_zpos_at_position(seg_left, seg_top));
				*(pos_ptr++) = CL_Vec3f(seg_right, seg_top, get_zpos_at_position(seg_right, seg_top));
				*(pos_ptr++) = CL_Vec3f(seg_left, seg_bottom, get_zpos_at_position(seg_left, seg_bottom));
				*(pos_ptr++) = CL_Vec3f(seg_left, seg_bottom, get_zpos_at_position(seg_left, seg_bottom));
				*(pos_ptr++) = CL_Vec3f(seg_right, seg_top, get_zpos_at_position(seg_right, seg_top));
				*(pos_ptr++) = CL_Vec3f(seg_right, seg_bottom, get_zpos_at_position(seg_right, seg_bottom));

				seg_left = dest_left + ((segment_count_horizontal+1) * (dest_right- dest_left) / num_segments_horizontal);
				seg_right = dest_left + ((segment_count_horizontal+2) * (dest_right- dest_left) / num_segments_horizontal);

				point_a = CL_Vec3f(seg_left, seg_top, get_zpos_at_position(seg_left, seg_top));
				point_b = CL_Vec3f(seg_right, seg_top, get_zpos_at_position(seg_right, seg_top));
				point_c = CL_Vec3f(seg_left, seg_bottom, get_zpos_at_position(seg_left, seg_bottom));
				next_triangle_normal_left = CL_Vec3f::cross(point_b - point_a, point_c - point_a);

				point_a = CL_Vec3f(seg_left, seg_bottom, get_zpos_at_position(seg_left, seg_bottom));
				point_b = CL_Vec3f(seg_right, seg_top, get_zpos_at_position(seg_right, seg_top));
				point_c = CL_Vec3f(seg_right, seg_bottom, get_zpos_at_position(seg_right, seg_bottom));
				next_triangle_normal_right = CL_Vec3f::cross(point_b - point_a, point_c - point_a);

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

				*(tex_ptr++) = CL_Vec2f(tex_seg_left, tex_seg_top);
				*(tex_ptr++) = CL_Vec2f(tex_seg_right, tex_seg_top);
				*(tex_ptr++) = CL_Vec2f(tex_seg_left, tex_seg_bottom);
				*(tex_ptr++) = CL_Vec2f(tex_seg_left, tex_seg_bottom);
				*(tex_ptr++) = CL_Vec2f(tex_seg_right, tex_seg_top);
				*(tex_ptr++) = CL_Vec2f(tex_seg_right, tex_seg_bottom);
			}
		}
		CL_PrimitivesArray prim_array(gc);
		prim_array.set_attributes(0, positions);
		prim_array.set_attributes(1, normals);
		prim_array.set_attributes(2, tex1_coords);
		gc.set_texture(0, texture);
		gc.draw_primitives(cl_triangles, num_points, prim_array);

	}
	gc.reset_program_object();
	gc.reset_texture(0);

	gc.set_map_mode(CL_MapMode(cl_map_2d_upper_left));

	// Display the point position (for debugging)
	//CL_Font_Texture &font = gui->get_font();
	//CL_String pos = cl_format("(%1, %2, %3), (%4, %5, %6)", saved_x1, saved_y1, saved_z1, saved_x2, saved_y2, saved_z2);
	//font.draw_text(gc, gc.get_width() - 600, 44, pos, CL_Colorf::red);

	return true;
}

float GUI_Texture::get_zpos_at_position(float xpos, float ypos)
{
	float value = xpos;

	value = sin(wobble_offset + 1.5f * CL_PI * (value+CL_PI/4.0f));

	value = current_zoffset - (value / 2.0f);
	return value;
}

void GUI_Texture::gui_repaint()
{
}

CL_ResourceManager &GUI_Texture::get_resources_internal()
{
	return gui->get_resources_internal();
}

CL_ResourceManager &GUI_Texture::get_resources_gui()
{
	return resources_gui;
}

CL_GUIManager &GUI_Texture::get_gui_manager()
{
	return gui->get_gui_manager();
}

void GUI_Texture::set_constant_repaint(bool enable)
{
	pushbutton->set_constant_repaint(enable);
	lineedit->set_constant_repaint(enable);
	checkbox->set_constant_repaint(enable);
	slider->set_constant_repaint(enable);
	radiobutton->set_constant_repaint(enable);
	scrollbar->set_constant_repaint(enable);
	progressbar->set_constant_repaint(enable);
	tabpage->set_constant_repaint(enable);
	menubar->set_constant_repaint(enable);
	spin->set_constant_repaint(enable);
	combobox->set_constant_repaint(enable);
	manager->set_constant_repaint(enable);
	listview->set_constant_repaint(enable);
}

void GUI_Texture::set_projection_matrix()
{
	CL_GraphicContext gc = window_ptr->get_gc();

	float fov = 2.0f * atan2(1.0f, lens_zoom);
	float aspect = 1.0f;
	float width = (float) gc.get_width();
	float height = (float) gc.get_height();

	if (height)
		aspect = ( width * lens_aspect) / height;

	fov = (fov * 180.0f) / CL_PI;
	projection_matrix = CL_Mat4f::perspective( fov, aspect, lens_near, lens_far);

	gc.set_projection(projection_matrix);

}

void GUI_Texture::wm_input_intercept(CL_InputEvent &input_event, CL_InputState &input_state)
{
	if (!active_3dmode)
		return;

	CL_GraphicContext gc = window_ptr->get_gc();
	float gc_width = (float) gc.get_width();
	float gc_height = (float) gc.get_height();

	// Transform points to range -1.0f to 1.0f
	float mouse_x = ((2.0f * input_event.mouse_pos.x) / gc_width) - 1.0f;
	float mouse_y = ((2.0f * input_event.mouse_pos.y) / gc_height) - 1.0f;
	mouse_y = -mouse_y;

	CL_Mat4d resultant_matrix_double = CL_Mat4d(resultant_matrix);
	CL_Mat4d inverse_matrix = resultant_matrix_double.inverse();
	CL_Vec3f point;
	float mouse_z;
	float mouse_low_z = 0.5f;
	float mouse_high_z = 1.5f;

	for (int max_iterations = 0; max_iterations < 100; ++max_iterations)
	{
		// Find the Z position of the panel at this 2d point
		// Keep bisecting, until we have found the correct Z
		mouse_z = ( mouse_high_z + mouse_low_z ) / 2.0f;
		point = transform_point(CL_Vec3d( mouse_x, mouse_y, mouse_z), inverse_matrix);
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

CL_Vec3f GUI_Texture::transform_point(const CL_Vec3d &src_point, const CL_Mat4d &matrix)
{
	CL_Vec3d dest;

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

void GUI_Texture::setup_shader()
{
	CL_GraphicContext gc = window_ptr->get_gc();

	CL_ShaderObject vertex_shader(gc, cl_shadertype_vertex, GUI_Texture_Shader_Vertex);
	if(!vertex_shader.compile())
	{
		throw CL_Exception(cl_format(cl_text("Unable to compile vertex shader object: %1"), vertex_shader.get_info_log()));
	}

	CL_ShaderObject fragment_shader(gc, cl_shadertype_fragment, GUI_Texture_Shader_Fragment);
	if(!fragment_shader.compile())
	{
		throw CL_Exception(cl_format(cl_text("Unable to compile fragment shader object: %1"), fragment_shader.get_info_log()));
	}

	gui_shader = CL_ProgramObject(gc);
	gui_shader.attach(vertex_shader);
	gui_shader.attach(fragment_shader);
	gui_shader.bind_attribute_location(0, "in_position");
	gui_shader.bind_attribute_location(1, "in_normal");
	gui_shader.bind_attribute_location(2, "in_texcoord");
	if (!gui_shader.link())
	{
		throw CL_Exception(cl_format(cl_text("Unable to link program object: %1"), gui_shader.get_info_log()));
	}

	shader_set = true;

}
