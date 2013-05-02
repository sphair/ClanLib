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

#pragma once

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gui.h>
#include <ClanLib/sound.h>
#include <list>
using namespace clan;


class SoundSession
{
public:
	SoundSession() : is_null(true), unique_id(++max_unique_id), fade_filter(1.0f)  {}

	int unique_id;
	static int max_unique_id;
	bool is_null;
	SoundBuffer_Session session;
	std::string name;

	EchoFilter echo_filter;
	InverseEchoFilter inverse_echo_filter;
	FadeFilter fade_filter;

};

// This is the Application class (That is instantiated by the Program Class)
class App
{
public:
	App();

	int start(const std::vector<std::string> &args);

private:
	bool on_close(GUIComponent *win);
	void on_btn_play();
	void on_btn_prepare();
	void on_btn_update_soundoutput();
	void prepare_gui(GUIComponent &window);
	Spin *set_slide_and_spin(GUIComponent &window, const char *slider_name, const char *spin_name, int min, int start, int max, bool enabled);
	Spin *set_slide_and_spin(GUIComponent &window, const char *slider_name, const char *spin_name, float min, float start, float max, bool enabled);
	void spin_value_modified(Spin *spin);
	void on_spin_changed_integer(Spin *spin);
	void set_slider_to_spin_integer(Slider *slider, Spin *spin);
	void on_slider_changed_integer(Spin *spin);
	void on_btn_set_soundoutput_filters();
	SoundBuffer &get_selected_soundbuffer();
	void on_spin_changed_float(Spin *spin);
	void set_slider_to_spin_float(Slider *slider, Spin *spin);
	void on_slider_changed_float(Spin *spin);
	void on_btn_set_fade_filter();
	void on_echo_filter_checked();
	void on_echo_filter_unchecked();
	void on_fade_filter_checked();
	void on_fade_filter_unchecked();
	void on_inverse_echo_filter_checked();
	void on_inverse_echo_filter_unchecked();
	void set_sound_output();

	void fill_session_combobox();

	Slider *get_slider(Spin *spin);
	void set_spin_value(Spin *spin, int value);
	void set_spin_value(Spin *spin, float value);

	void use_selected_session();

	void on_btn_sess_play();
	void on_btn_sess_stop();
	void on_btn_sess_destroy();

	void on_cbox_session_selected(int item);

	void on_looping_checked();
	void on_looping_unchecked();
	void set_soundbuffer_session_filters(SoundSession &sound_session);

private:
	SoundBuffer sfx_pacman_start;
	SoundBuffer sfx_cheer;

	ComboBox *cbox_sound;

	PushButton *btn_update_soundoutput;
	PushButton *btn_set_soundoutput_filters;
	PushButton *btn_play;
	PushButton *btn_prepare;
	PushButton *btn_set_fade_filter;
	PushButton *btn_sess_play;
	PushButton *btn_sess_stop;
	PushButton *btn_sess_destroy;
	CheckBox *chk_echo_filter;
	CheckBox *chk_fade_filter;
	CheckBox *chk_inverse_echo;
	CheckBox *chk_apply_session;
	CheckBox *chk_looping;
	ComboBox *cbox_session;

	Spin *spin_output_frequency;
	Spin *spin_output_latency;
	Spin *spin_filter_echo_buffer_size;
	Spin *spin_filter_echo_shift_factor;
	Spin *spin_filter_fade_volume;
	Spin *spin_filter_inverse_echo_buffer_size;
	Spin *spin_initial_volume;
	Spin *spin_initial_pan;
	Spin *spin_session_position;
	Spin *spin_session_frequency;
	Spin *spin_session_volume;
	Spin *spin_session_pan;
	Spin *spin_session_fade_volume;
	Spin *spin_session_fade_duration;

	std::map<Spin *, Slider *> spin_slider_map_integer;
	std::map<Spin *, Slider *> spin_slider_map_float;

	SoundOutput sound_output;

	std::list<SoundSession> soundbuffer_sessions;
	SoundSession selected_sound_session;

	unsigned int initial_time;

	SoundFilter last_soundoutput_echo_filter;
	SoundFilter last_soundoutput_inverse_echo_filter;
	SoundFilter last_soundoutput_fade_filter;
};
