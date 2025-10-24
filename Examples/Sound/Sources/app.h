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

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gui.h>
#include <ClanLib/sound.h>
#include <ClanLib/mikmod.h>
#include <ClanLib/vorbis.h>
#include <list>


class SoundSession
{
public:
	SoundSession() : is_null(true), unique_id(++max_unique_id), fade_filter(1.0f)  {}

	int unique_id;
	static int max_unique_id;
	bool is_null;
	CL_SoundBuffer_Session session;
	CL_String name;

	CL_EchoFilter echo_filter;
	CL_InverseEchoFilter inverse_echo_filter;
	CL_FadeFilter fade_filter;

};

// This is the Application class (That is instantiated by the Program Class)
class App
{
public:
	App();

	int start(const std::vector<CL_String> &args);

private:
	bool on_close(CL_Window *win);
	void on_btn_play();
	void on_btn_prepare();
	void on_btn_update_soundoutput();
	void prepare_gui(CL_Window &window);
	CL_Spin *set_slide_and_spin(CL_Window &window, const char *slider_name, const char *spin_name, int min, int start, int max, bool enabled);
	CL_Spin *set_slide_and_spin(CL_Window &window, const char *slider_name, const char *spin_name, float min, float start, float max, bool enabled);
	void spin_value_modified(CL_Spin *spin);
	void on_spin_changed_integer(CL_Spin *spin);
	void set_slider_to_spin_integer(CL_Slider *slider, CL_Spin *spin);
	void on_slider_changed_integer(CL_Spin *spin);
	void on_btn_set_soundoutput_filters();
	CL_SoundBuffer &get_selected_soundbuffer();
	void on_spin_changed_float(CL_Spin *spin);
	void set_slider_to_spin_float(CL_Slider *slider, CL_Spin *spin);
	void on_slider_changed_float(CL_Spin *spin);
	void on_btn_set_fade_filter();
	void on_echo_filter_checked();
	void on_echo_filter_unchecked();
	void on_fade_filter_checked();
	void on_fade_filter_unchecked();
	void on_inverse_echo_filter_checked();
	void on_inverse_echo_filter_unchecked();
	void set_sound_output();

	void fill_session_combobox();

	CL_Slider *get_slider(CL_Spin *spin);
	void set_spin_value(CL_Spin *spin, int value);
	void set_spin_value(CL_Spin *spin, float value);

	void use_selected_session();

	void on_btn_sess_play();
	void on_btn_sess_stop();
	void on_btn_sess_destroy();

	void on_cbox_session_selected(int item);

	void on_looping_checked();
	void on_looping_unchecked();
	void set_soundbuffer_session_filters(SoundSession &sound_session);

private:
	CL_SoundBuffer sfx_pacman_start;
	CL_SoundBuffer sfx_beast_title;
	CL_SoundBuffer sfx_cheer;

	CL_ComboBox *cbox_sound;

	CL_PushButton *btn_update_soundoutput;
	CL_PushButton *btn_set_soundoutput_filters;
	CL_PushButton *btn_play;
	CL_PushButton *btn_prepare;
	CL_PushButton *btn_set_fade_filter;
	CL_PushButton *btn_sess_play;
	CL_PushButton *btn_sess_stop;
	CL_PushButton *btn_sess_destroy;
	CL_CheckBox *chk_echo_filter;
	CL_CheckBox *chk_fade_filter;
	CL_CheckBox *chk_inverse_echo;
	CL_CheckBox *chk_apply_session;
	CL_CheckBox *chk_looping;
	CL_ComboBox *cbox_session;

	CL_Spin *spin_output_frequency;
	CL_Spin *spin_output_latency;
	CL_Spin *spin_filter_echo_buffer_size;
	CL_Spin *spin_filter_echo_shift_factor;
	CL_Spin *spin_filter_fade_volume;
	CL_Spin *spin_filter_inverse_echo_buffer_size;
	CL_Spin *spin_initial_volume;
	CL_Spin *spin_initial_pan;
	CL_Spin *spin_session_position;
	CL_Spin *spin_session_frequency;
	CL_Spin *spin_session_volume;
	CL_Spin *spin_session_pan;
	CL_Spin *spin_session_fade_volume;
	CL_Spin *spin_session_fade_duration;

	std::map<CL_Spin *, CL_Slider *> spin_slider_map_integer;
	std::map<CL_Spin *, CL_Slider *> spin_slider_map_float;

	CL_SoundOutput sound_output;

	std::list<SoundSession> soundbuffer_sessions;
	SoundSession selected_sound_session;

	unsigned int initial_time;

	CL_SoundFilter last_soundoutput_echo_filter;
	CL_SoundFilter last_soundoutput_inverse_echo_filter;
	CL_SoundFilter last_soundoutput_fade_filter;
};
