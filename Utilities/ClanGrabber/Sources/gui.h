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

class App
{
public:
	int start(const std::vector<CL_String> &args);

private:
	void on_button_clicked(CL_PushButton *button);
	void on_button_clicked2(CL_PushButton *button);
	bool on_close(CL_Window *win);
	void update_layered_worker_thread();
	void update_layered_worker_thread_process();
	void on_combo_selected_size(int value, CL_ComboBox *combo);
	void on_combo_selected_compression(int value, CL_ComboBox *combo);
	void on_checkbox_changed_stretch(CL_CheckBox *checkbox);

private:
	CL_GUIManager gui;
	CL_Window *window;
	CL_Label *selected_window_name;
	CL_PushButton *button_1;
	CL_PushButton *button_2;
	CL_LineEdit *lineedit_left;
	CL_LineEdit *lineedit_right;
	CL_LineEdit *lineedit_top;
	CL_LineEdit *lineedit_bottom;
	CL_LineEdit *lineedit_fps;

	CL_Event update_window_event_stop;
	CL_Event update_window_event_start;
	CL_Event update_window_event_completed;

	CL_Thread update_window_worker_thread;
	bool update_window_worker_thread_started;

	volatile bool abort_writing ;

	int selected_output_width;
	int selected_output_height;
	
	int selected_area_left;
	int selected_area_top;
	int selected_area_right;
	int selected_area_bottom;
	int selected_fps;

	bool use_WMV3_compression;
	bool is_resize_to_fit;

};
