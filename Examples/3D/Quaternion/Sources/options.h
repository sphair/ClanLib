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

class Options : public CL_GUIComponent
{
public:
	Options(CL_GUIManager &gui, CL_Rect gui_position);
	virtual ~Options();

	CL_Angle rotation_x;
	CL_Angle rotation_y;
	CL_Angle rotation_z;

	CL_Angle target_x;
	CL_Angle target_y;
	CL_Angle target_z;

	CL_Quaternionf quaternion;

	bool button_lerp_clicked;
	bool button_slerp_clicked;
	bool button_rotate_clicked;

	CL_PushButton *button_lerp;
	CL_PushButton *button_slerp;
	CL_PushButton *button_rotate;

	float max_angle_value;

	void set_new_quaternion(const CL_Quaternionf &new_quaternion);

private:
	void write_quaternion();
	void update_euler();
	void update_quaternion();
	void update_all_slider_text();
	CL_Label *create_slider_label(CL_Slider *slider);
	CL_CheckBox *create_checkbox(int xpos, int ypos, const char *name, bool state);
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	CL_Slider *create_slider(int xpos, int ypos);
	float get_value(CL_Slider *slider, float min_value, float max_value);
	void set_value(CL_Slider *slider, float value, float min_value, float max_value);
	CL_Label *create_combobox_label(CL_ComboBox *combo, const char *text);
	void slider_rotation_x_changed();
	void slider_rotation_y_changed();
	void slider_rotation_z_changed();
	void slider_target_x_changed();
	void slider_target_y_changed();
	void slider_target_z_changed();
	void slider_quaternion_w_changed();
	void slider_quaternion_i_changed();
	void slider_quaternion_j_changed();
	void slider_quaternion_k_changed();
	void on_clicked_button_lerp();
	void on_clicked_button_slerp();
	void on_clicked_button_rotate();
	void set_all_sliders();

private:

	CL_Slider *slider_rotation_x;
	CL_Slider *slider_rotation_y;
	CL_Slider *slider_rotation_z;

	CL_Label *label_rotation_x;
	CL_Label *label_rotation_y;
	CL_Label *label_rotation_z;

	CL_Slider *slider_target_x;
	CL_Slider *slider_target_y;
	CL_Slider *slider_target_z;

	CL_Label *label_target_x;
	CL_Label *label_target_y;
	CL_Label *label_target_z;

	CL_Slider *slider_quaternion_w;
	CL_Slider *slider_quaternion_i;
	CL_Slider *slider_quaternion_j;
	CL_Slider *slider_quaternion_k;

	CL_Label *label_quaternion_w;
	CL_Label *label_quaternion_i;
	CL_Label *label_quaternion_j;
	CL_Label *label_quaternion_k;

};

