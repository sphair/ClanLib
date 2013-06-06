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

#pragma once

#include "../../CommonCode/Sources/scrollbar.h"
#include "../../CommonCode/Sources/pushbutton.h"
#include "../../CommonCode/Sources/lineedit.h"
#include "../../CommonCode/Sources/checkbox.h"
#include "../../CommonCode/Sources/slider.h"
#include "../../CommonCode/Sources/radiobutton.h"
#include "../../CommonCode/Sources/progressbar.h"
#include "../../CommonCode/Sources/tabpage.h"
#include "../../CommonCode/Sources/menubar.h"
#include "../../CommonCode/Sources/spin.h"
#include "../../CommonCode/Sources/combobox.h"
#include "../../CommonCode/Sources/theme.h"
#include "../../CommonCode/Sources/listview.h"
#include "panel3d.h"

class GUI;

class GUI_Layered
{
public:
	GUI_Layered(GUI *gui);
	~GUI_Layered();

	bool run();

	clan::GUIManager &get_gui_manager();
	clan::ResourceManager &get_resources_internal();

private:
	void set_projection_matrix();
	void wm_input_intercept(clan::InputEvent &input_event);
	clan::Vec3f transform_point(const clan::Vec3d &src_point, const clan::Mat4d &matrix);
	float get_zpos_at_position(float xpos, float ypos);
	bool run3d();
	void setup_shader();

private:

	GUI *gui;
	clan::Canvas canvas;
	clan::GUIManager gui_manager;
	clan::GUIWindowManagerTexture wm;

	ScrollBar *scrollbar;
	LineEdit *lineedit;
	Slider *slider;
	RadioButton *radiobutton;
	MenuBar *menubar;
	Spin *spin;
	ComboBox *combobox;
	Panel3D *panel3d;

	struct UniformBlock
	{
		clan::Mat4f world_to_eye;
		clan::Mat4f eye_to_projection;
		clan::Vec4f normal_world_to_eye[3]; // actually a mat3, but OpenGL std140 stores it as 3x vec4
		clan::Vec4f material_emission;
		clan::Vec4f material_specular;
		float material_shininess;
		float material_transparency;
		float padding0;
		float padding1;
		clan::Vec4f light_vector;
		clan::Vec4f light_half_vector;
		clan::Vec4f light_specular;
		clan::Vec4f light_diffuse;
		clan::Vec4f light_ambient;
	};

	static const int num_segments_horizontal = 32;
	static const int num_segments_vertical = 1;
	static const int num_points = 6 * num_segments_horizontal * num_segments_vertical;

	clan::ShaderEffect gui_shader;
	clan::VertexArrayVector<clan::Vec3f> attr_position_in_world;
	clan::VertexArrayVector<clan::Vec3f> attr_normal_in_world;
	clan::VertexArrayVector<clan::Vec2f> attr_tex_coord;
	clan::UniformVector<UniformBlock> gpu_uniform_block;
	clan::Resource<clan::Texture> gui_shader_texture;

	float lens_zoom;
	float lens_aspect;
	float lens_near;
	float lens_far;

	float saved_x1;
	float saved_x2;
	float saved_y1;
	float saved_y2;
	float saved_z1;
	float saved_z2;

	clan::Mat4f eye_to_projection;
	clan::Mat4f world_to_eye;
	clan::Mat4f world_to_projection;

	float wobble_offset;
	clan::ubyte64 last_time;

	float current_zoffset;

};



