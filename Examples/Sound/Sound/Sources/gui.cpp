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
#include "app.h"

void App::spin_value_modified(Spin *spin)
{
	if ( (spin==spin_output_frequency) || (spin==spin_output_latency) )
	{
		btn_update_soundoutput->set_enabled(true);
		return;
	}

	if ( (spin==spin_filter_echo_buffer_size) || (spin==spin_filter_echo_shift_factor) )
	{
		if (chk_echo_filter->is_enabled())
		{
			btn_set_soundoutput_filters->set_enabled(true);
		}
	}

	if ( spin==spin_filter_fade_volume )
	{
		if (chk_fade_filter->is_enabled())
		{
			btn_set_soundoutput_filters->set_enabled(true);
		}
	}

	if ( spin==spin_filter_inverse_echo_buffer_size )
	{
		if (chk_inverse_echo->is_enabled())
		{
			btn_set_soundoutput_filters->set_enabled(true);
		}
	}
}

void App::prepare_gui(Window &window)
{
	btn_update_soundoutput = PushButton::get_named_item(&window, "btn_update_soundoutput");
	btn_update_soundoutput->func_clicked().set(this, &App::on_btn_update_soundoutput);
	btn_update_soundoutput->set_enabled(false);

	btn_set_soundoutput_filters = PushButton::get_named_item(&window, "btn_set_soundoutput_filters");
	btn_set_soundoutput_filters->func_clicked().set(this, &App::on_btn_set_soundoutput_filters);
	btn_set_soundoutput_filters->set_enabled(false);

	btn_play = PushButton::get_named_item(&window, "btn_play");
	btn_play->func_clicked().set(this, &App::on_btn_play);

	btn_prepare = PushButton::get_named_item(&window, "btn_prepare");
	btn_prepare->func_clicked().set(this, &App::on_btn_prepare);

	btn_set_fade_filter = PushButton::get_named_item(&window, "btn_set_fade_filter");
	btn_set_fade_filter->func_clicked().set(this, &App::on_btn_set_fade_filter);
	btn_set_fade_filter->set_enabled(false);

	btn_sess_play = PushButton::get_named_item(&window, "btn_sess_play");
	btn_sess_play->func_clicked().set(this, &App::on_btn_sess_play);
	btn_sess_play->set_enabled(false);

	btn_sess_stop = PushButton::get_named_item(&window, "btn_sess_stop");
	btn_sess_stop->func_clicked().set(this, &App::on_btn_sess_stop);
	btn_sess_stop->set_enabled(false);

	btn_sess_destroy = PushButton::get_named_item(&window, "btn_sess_destroy");
	btn_sess_destroy->func_clicked().set(this, &App::on_btn_sess_destroy);
	btn_sess_destroy->set_enabled(false);

	chk_echo_filter = CheckBox::get_named_item(&window, "chk_echo_filter");
	chk_echo_filter->func_checked().set(this, &App::on_echo_filter_checked);
	chk_echo_filter->func_unchecked().set(this, &App::on_echo_filter_unchecked);

	chk_fade_filter = CheckBox::get_named_item(&window, "chk_fade_filter");
	chk_fade_filter->func_checked().set(this, &App::on_fade_filter_checked);
	chk_fade_filter->func_unchecked().set(this, &App::on_fade_filter_unchecked);

	chk_inverse_echo = CheckBox::get_named_item(&window, "chk_inverse_echo");
	chk_inverse_echo->func_checked().set(this, &App::on_inverse_echo_filter_checked);
	chk_inverse_echo->func_unchecked().set(this, &App::on_inverse_echo_filter_unchecked);

	chk_apply_session = CheckBox::get_named_item(&window, "chk_apply_session");

	chk_looping = CheckBox::get_named_item(&window, "chk_looping");
	chk_looping->set_enabled(false);
	chk_looping->func_checked().set(this, &App::on_looping_checked);
	chk_looping->func_unchecked().set(this, &App::on_looping_unchecked);

	cbox_session = ComboBox::get_named_item(&window, "cbox_session");
	cbox_session->set_enabled(false);
	cbox_session->func_item_selected().set(this, &App::on_cbox_session_selected);

	PopupMenu menu;
	menu.insert_item("Pacman Start Sample(.wav)");
	menu.insert_item("Methane Boss Music (.mod)");
	menu.insert_item("Cheering Sample (.ogg)");

	cbox_sound = ComboBox::get_named_item(&window, "cbox_sound");
	cbox_sound->set_popup_menu(menu);
	cbox_sound->set_selected_item(0);

	spin_output_frequency = set_slide_and_spin(window, "slide_out_mix_freq", "spin_out_mix_freq", 10000, 44100, 60000, true);
	spin_output_latency = set_slide_and_spin(window, "slide_out_mix_latency", "spin_out_mix_latency", 10, 50, 400, true);
	spin_filter_echo_buffer_size = set_slide_and_spin(window, "slide_echo_buffer", "spin_echo_buffer", 1024, 32*1024, 128*1024, false);
	spin_filter_echo_shift_factor = set_slide_and_spin(window, "slide_echo_shift", "spin_echo_shift", 1, 2, 8, false);
	spin_filter_fade_volume = set_slide_and_spin(window, "slide_fade_volume", "spin_fade_volume", 0.0f, 1.0f, 1.0f, false);
	spin_filter_inverse_echo_buffer_size = set_slide_and_spin(window, "slide_inverseecho_buffer", "spin_inverseecho_buffer", 1024, 32*1024, 128*1024, false);
	spin_initial_volume = set_slide_and_spin(window, "slide_initial_volume", "spin_initial_volume", 0.0f, 1.0f, 1.0f, true);
	spin_initial_pan = set_slide_and_spin(window, "slide_initial_pan", "spin_initial_pan", -1.0f, 0.0f, 1.0f, true);
	spin_session_position = set_slide_and_spin(window, "slide_sess_position", "spin_sess_position", 0.0f, 0.0f, 1.0f, false);
	spin_session_frequency = set_slide_and_spin(window, "slide_sess_freq", "spin_sess_freq", 10000, 44100, 60000, false);
	spin_session_volume = set_slide_and_spin(window, "slide_sess_volume", "spin_sess_volume", 0.0f, 1.0f, 1.0f, false);
	spin_session_pan = set_slide_and_spin(window, "slide_sess_pan", "spin_sess_pan", -1.0f, 0.0f, 1.0f, false);
	spin_session_fade_volume = set_slide_and_spin(window, "slide_sess_fade_volume", "spin_sess_fade_volume", 0.0f, 1.0f, 1.0f, false);
	spin_session_fade_duration = set_slide_and_spin(window, "slide_sess_fade_duration", "spin_sess_fade_duration", 1000, 2000, 10000, false);
}

void App::on_echo_filter_checked()
{
	spin_filter_echo_buffer_size->set_enabled(true);
	get_slider(spin_filter_echo_buffer_size)->set_enabled(true);
	spin_filter_echo_shift_factor->set_enabled(true);
	get_slider(spin_filter_echo_shift_factor)->set_enabled(true);
	btn_set_soundoutput_filters->set_enabled(true);

}

void App::on_echo_filter_unchecked()
{
	spin_filter_echo_buffer_size->set_enabled(false);
	get_slider(spin_filter_echo_buffer_size)->set_enabled(false);
	spin_filter_echo_shift_factor->set_enabled(false);
	get_slider(spin_filter_echo_shift_factor)->set_enabled(false);
	btn_set_soundoutput_filters->set_enabled(true);
}

void App::on_fade_filter_checked()
{
	spin_filter_fade_volume->set_enabled(true);
	get_slider(spin_filter_fade_volume)->set_enabled(true);
	btn_set_soundoutput_filters->set_enabled(true);
}

void App::on_fade_filter_unchecked()
{
	spin_filter_fade_volume->set_enabled(false);
	get_slider(spin_filter_fade_volume)->set_enabled(false);
	btn_set_soundoutput_filters->set_enabled(true);
}

void App::on_inverse_echo_filter_checked()
{
	spin_filter_inverse_echo_buffer_size->set_enabled(true);
	get_slider(spin_filter_inverse_echo_buffer_size)->set_enabled(true);
	btn_set_soundoutput_filters->set_enabled(true);
}

void App::on_inverse_echo_filter_unchecked()
{
	spin_filter_inverse_echo_buffer_size->set_enabled(false);
	get_slider(spin_filter_inverse_echo_buffer_size)->set_enabled(false);
	btn_set_soundoutput_filters->set_enabled(true);
}

Spin *App::set_slide_and_spin(Window &window, const char *slider_name, const char *spin_name, int min, int start, int max, bool enabled)
{
	Slider *slider = dynamic_cast<Slider*>(window.get_named_item(slider_name));
	if (!slider)
		throw Exception("Where has the Slider gone?");

	Spin *spin = dynamic_cast<Spin*>(window.get_named_item(spin_name));
	if (!spin)
		throw Exception("Where has the Spin gone?");

	spin_slider_map_integer[spin] = slider;

	spin->set_floating_point_mode(false);
	spin->set_ranges(min, max);
	spin->set_value(start);
	spin->set_step_size(1);
	spin->func_value_changed().set(this, &App::on_spin_changed_integer, spin);

	slider->set_min(0);
	slider->set_max(10000);
	slider->set_tick_count(100);
	slider->set_page_step(1000);
	slider->set_position(50);
	slider->set_lock_to_ticks(false);
	slider->func_value_changed().set(this, &App::on_slider_changed_integer, spin);

	set_slider_to_spin_integer(slider, spin);

	if (!enabled)
	{
		spin->set_enabled(false);
		slider->set_enabled(false);
	}

	return spin;
}

Spin *App::set_slide_and_spin(Window &window, const char *slider_name, const char *spin_name, float min, float start, float max, bool enabled)
{
	Slider *slider = dynamic_cast<Slider*>(window.get_named_item(slider_name));
	if (!slider)
		throw Exception("Where has the Slider gone?");

	Spin *spin = dynamic_cast<Spin*>(window.get_named_item(spin_name));
	if (!spin)
		throw Exception("Where has the Spin gone?");

	spin_slider_map_float[spin] = slider;

	spin->set_floating_point_mode(true);
	spin->set_ranges_float(min, max);
	spin->set_value_float(start);
	spin->set_step_size_float(1.0f);
	spin->func_value_changed().set(this, &App::on_spin_changed_float, spin);

	slider->set_min(0);
	slider->set_max(10000);
	slider->set_tick_count(100);
	slider->set_page_step(1000);
	slider->set_position(50);
	slider->set_lock_to_ticks(false);
	slider->func_value_changed().set(this, &App::on_slider_changed_float, spin);

	set_slider_to_spin_float(slider, spin);

	if (!enabled)
	{
		spin->set_enabled(false);
		slider->set_enabled(false);
	}

	return spin;
}

Slider *App::get_slider(Spin *spin)
{
	std::map<Spin *, Slider *>::iterator it;
	it = spin_slider_map_integer.find(spin);
	if (it == spin_slider_map_integer.end())
	{
		it = spin_slider_map_float.find(spin);
		if (it == spin_slider_map_float.end())
		{
			throw Exception("Humm, strange, The Slider has vanished");
		}
	}

	Slider *slider = it->second;
	if (!slider)
		throw Exception("The Slider is NULL!");

	return slider;
}

void App::on_spin_changed_integer(Spin *spin)
{
	set_slider_to_spin_integer(get_slider(spin), spin);
	spin_value_modified(spin);
}

void App::on_spin_changed_float(Spin *spin)
{
	set_slider_to_spin_float(get_slider(spin), spin);
	spin_value_modified(spin);
}

void App::on_slider_changed_integer(Spin *spin)
{
	Slider *slider = get_slider(spin);

	int position = slider->get_position();

	position -= slider->get_min();
	int max = (slider->get_max() + 1 - slider->get_min());

	int spin_min = spin->get_min();
	int spin_max = spin->get_max();
	int spin_range = spin_max - spin_min;

	int value = (position * spin_range) / max;
	value += spin_min;
	if (value < spin_min)
		value = spin_min;

	if (value > spin_max)
		value = spin_max;

	spin->set_value(value);
	spin_value_modified(spin);
}

void App::on_slider_changed_float(Spin *spin)
{
	Slider *slider = get_slider(spin);

	float position = (float) slider->get_position();

	position -= slider->get_min();
	float max = (float) (slider->get_max() + 1 - slider->get_min());

	float spin_min = spin->get_min_float();
	float spin_max = spin->get_max_float();
	float spin_range = spin_max - spin_min;

	float value = (position * spin_range) / max;
	value += spin_min;
	if (value < spin_min)
		value = spin_min;

	if (value > spin_max)
		value = spin_max;

	spin->set_value_float(value);
	spin_value_modified(spin);

}

void App::set_slider_to_spin_integer(Slider *slider, Spin *spin)
{
	int value = spin->get_value();
	int spin_min = spin->get_min();
	int spin_max = spin->get_max();
	int spin_range = spin_max - spin_min;

	int slider_max = slider->get_max();
	int slider_min = slider->get_min();

	int slider_range = (slider_max + 1 - slider_min);

	int new_value = (int) (((value - spin_min) * slider_range) / spin_range);
	if (new_value < slider_min)
		new_value = slider_min;

	if (new_value > slider_max)
		new_value = slider_max;

	slider->set_position(new_value);

}

void App::set_slider_to_spin_float(Slider *slider, Spin *spin)
{
	float value = spin->get_value_float();
	float spin_min = spin->get_min_float();
	float spin_max = spin->get_max_float();
	float spin_range = spin_max - spin_min;

	int slider_max = slider->get_max();
	int slider_min = slider->get_min();

	float slider_range = (float) (slider_max + 1 - slider_min);

	int new_value = (int) (((value - spin_min) * slider_range) / spin_range);
	if (new_value < slider_min)
		new_value = slider_min;

	if (new_value > slider_max)
		new_value = slider_max;

	slider->set_position(new_value);
}

bool App::on_close(Window *win)
{
	win->exit_with_code(0);
	return true;
}


void App::use_selected_session()
{
	if (selected_sound_session.is_null)
	{
		spin_session_position->set_enabled(false);
		spin_session_frequency->set_enabled(false);
		spin_session_volume->set_enabled(false);
		spin_session_pan->set_enabled(false);
		spin_session_fade_volume->set_enabled(false);
		spin_session_fade_duration->set_enabled(false);

		get_slider(spin_session_position)->set_enabled(false);
		get_slider(spin_session_frequency)->set_enabled(false);
		get_slider(spin_session_volume)->set_enabled(false);
		get_slider(spin_session_pan)->set_enabled(false);
		get_slider(spin_session_fade_volume)->set_enabled(false);
		get_slider(spin_session_fade_duration)->set_enabled(false);

		btn_set_fade_filter->set_enabled(false);
		btn_sess_play->set_enabled(false);
		btn_sess_stop->set_enabled(false);
		btn_sess_destroy->set_enabled(false);
		chk_looping->set_enabled(false);
		return;
	}
	spin_session_position->set_enabled(true);
	spin_session_frequency->set_enabled(true);
	spin_session_volume->set_enabled(true);
	spin_session_pan->set_enabled(true);

	bool fade_enabled = !selected_sound_session.fade_filter.is_null();

	spin_session_fade_volume->set_enabled(fade_enabled);
	spin_session_fade_duration->set_enabled(fade_enabled);

	get_slider(spin_session_position)->set_enabled(true);
	get_slider(spin_session_frequency)->set_enabled(true);
	get_slider(spin_session_volume)->set_enabled(true);
	get_slider(spin_session_pan)->set_enabled(true);
	get_slider(spin_session_fade_volume)->set_enabled(fade_enabled);
	get_slider(spin_session_fade_duration)->set_enabled(fade_enabled);

	set_spin_value(spin_session_position, selected_sound_session.session.get_position_relative());
	set_spin_value(spin_session_frequency, selected_sound_session.session.get_frequency());
	set_spin_value(spin_session_volume, selected_sound_session.session.get_volume());
	set_spin_value(spin_session_pan, selected_sound_session.session.get_pan());

	btn_set_fade_filter->set_enabled(fade_enabled);

	if (selected_sound_session.session.is_playing())
	{
		btn_sess_play->set_enabled(false);
		btn_sess_stop->set_enabled(true);
	}
	else
	{
		btn_sess_play->set_enabled(true);
		btn_sess_stop->set_enabled(false);
	}

	btn_sess_destroy->set_enabled(true);

	chk_looping->set_checked(selected_sound_session.session.get_looping());
	chk_looping->set_enabled(true);
}

void App::set_spin_value(Spin *spin, int value)
{
	spin->set_value(value);
	set_slider_to_spin_integer(get_slider(spin), spin);
}

void App::set_spin_value(Spin *spin, float value)
{
	spin->set_value_float(value);
	set_slider_to_spin_float(get_slider(spin), spin);
}

void App::fill_session_combobox()
{
	std::list<SoundSession>::const_iterator it;

	int num_items = 0;

	PopupMenu menu;
	for (it = soundbuffer_sessions.begin(); it != soundbuffer_sessions.end(); ++it)
	{
		menu.insert_item(it->name);
		num_items++;
	}

	cbox_session->set_popup_menu(menu);

	if (num_items)
	{
		cbox_session->set_enabled(true);
		cbox_session->set_selected_item(0);
	}
	else
	{
		cbox_session->set_enabled(false);
		cbox_session->set_text("");
	}
}

SoundBuffer &App::get_selected_soundbuffer()
{
	int item = cbox_sound->get_selected_item();

	if (item==0)
	{
		return sfx_pacman_start;
	}

	if (item==1)
	{
		return sfx_beast_title;
	}

	if (item==2)
	{
		return sfx_cheer;
	}
	return sfx_pacman_start;
}

void App::on_cbox_session_selected(int item)
{
	std::list<SoundSession>::const_iterator it;

	for (it = soundbuffer_sessions.begin(); it != soundbuffer_sessions.end(); ++it)
	{
		if (item==0)
		{
			selected_sound_session = *it;
			use_selected_session();
			break;
		}
		item--;
	}
}

