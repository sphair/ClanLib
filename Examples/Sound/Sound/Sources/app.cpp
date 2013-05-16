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
#include "app.h"

int SoundSession::max_unique_id = 0;

App::App()
{
}

// The start of the Application
int App::start(const std::vector<std::string> &args)
{
	sfx_pacman_start = SoundBuffer("../../Game/Pacman/resources/start.wav");
	sfx_cheer = SoundBuffer("Resources/cheer1.ogg");

	std::string theme;
	if (FileHelp::file_exists("../../../Resources/GUIThemeAero/theme.css"))
		theme = "../../../Resources/GUIThemeAero";
	else if (FileHelp::file_exists("../../../Resources/GUIThemeBasic/theme.css"))
		theme = "../../../Resources/GUIThemeBasic";
	else
		throw Exception("No themes found");

	GUIManager gui(theme);

	DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("Sound Example");
	win_desc.set_position(Rect(200, 100, 540, 440), false);
	win_desc.set_visible(false);
	GUIComponent window(&gui, win_desc, "window");
	window.func_close().set(this, &App::on_close, &window);

	GUILayoutCorners layout;
	window.set_layout(layout);

	window.create_components("Resources/layout.xml");

	prepare_gui(window);

	window.set_visible(true);

	set_sound_output();

	initial_time = System::get_time();

	gui.exec();

	return 0;
}

void App::set_sound_output()
{
	soundbuffer_sessions.clear();
	selected_sound_session = SoundSession();

	fill_session_combobox();
	use_selected_session();

	sound_output = SoundOutput(spin_output_frequency->get_value(), spin_output_latency->get_value());

	if (chk_echo_filter->is_checked() || chk_fade_filter->is_checked() || chk_inverse_echo->is_checked() )
	{
		btn_set_soundoutput_filters->set_enabled(false);
	}

}

void App::on_btn_update_soundoutput()
{
	set_sound_output();
	btn_update_soundoutput->set_enabled(false);
}

void App::on_btn_set_soundoutput_filters()
{
	if (!last_soundoutput_echo_filter.is_null())
	{
		sound_output.remove_filter(last_soundoutput_echo_filter);
		last_soundoutput_echo_filter = SoundFilter();
	}

	if (!last_soundoutput_inverse_echo_filter.is_null())
	{
		sound_output.remove_filter(last_soundoutput_inverse_echo_filter);
		last_soundoutput_inverse_echo_filter = SoundFilter();
	}

	if (!last_soundoutput_fade_filter.is_null())
	{
		sound_output.remove_filter(last_soundoutput_fade_filter);
		last_soundoutput_fade_filter = SoundFilter();
	}

	if (chk_echo_filter->is_checked())
	{
		last_soundoutput_echo_filter = EchoFilter(spin_filter_echo_buffer_size->get_value(), spin_filter_echo_shift_factor->get_value_float());
		sound_output.add_filter(last_soundoutput_echo_filter);
	}

	if (chk_fade_filter->is_checked())
	{
		last_soundoutput_fade_filter = FadeFilter(spin_filter_fade_volume->get_value_float());
		sound_output.add_filter(last_soundoutput_fade_filter);
	}

	if (chk_inverse_echo->is_checked())
	{
		last_soundoutput_inverse_echo_filter = InverseEchoFilter(spin_filter_echo_buffer_size->get_value());
		sound_output.add_filter(last_soundoutput_inverse_echo_filter);
	}

	btn_set_soundoutput_filters->set_enabled(false);
}


void App::on_btn_play()
{
	SoundBuffer &buffer = get_selected_soundbuffer();
	buffer.set_volume(spin_initial_volume->get_value_float());
	buffer.set_pan(spin_initial_pan->get_value_float());

	SoundSession sound_session;
	sound_session.is_null = false;
	sound_session.session = buffer.prepare();
	set_soundbuffer_session_filters(sound_session);
	sound_session.session.play();
}

void App::on_btn_prepare()
{
	SoundBuffer &buffer = get_selected_soundbuffer();
	buffer.set_volume(spin_initial_volume->get_value_float());
	buffer.set_pan(spin_initial_pan->get_value_float());
	SoundSession sound_session;
	sound_session.is_null = false;
	sound_session.session = buffer.prepare();
	sound_session.name = string_format("Sound prepared at: %1 seconds", (System::get_time() - initial_time)/1000);
	soundbuffer_sessions.push_front(sound_session);

	set_soundbuffer_session_filters(sound_session);
	selected_sound_session = sound_session;

	fill_session_combobox();
	use_selected_session();
}

void App::on_btn_sess_play()
{
	if (!selected_sound_session.is_null)
	{
		selected_sound_session.session.set_position_relative(0.0f);
		set_spin_value(spin_session_position, selected_sound_session.session.get_position_relative());

		selected_sound_session.session.play();
		btn_sess_play->set_enabled(false);
		btn_sess_stop->set_enabled(true);
	}
}

void App::on_btn_sess_stop()
{
	if (!selected_sound_session.is_null)
	{
		selected_sound_session.session.stop();
		btn_sess_play->set_enabled(true);
		btn_sess_stop->set_enabled(false);
	}
}

void App::on_btn_sess_destroy()
{
	if (!selected_sound_session.is_null)
	{
		std::list<SoundSession>::iterator it;

		for (it = soundbuffer_sessions.begin(); it != soundbuffer_sessions.end(); ++it)
		{
			if (it->unique_id == selected_sound_session.unique_id)
			{
				soundbuffer_sessions.erase(it);
				break;
			}
		}
		selected_sound_session.session.stop();
		selected_sound_session = SoundSession();

		cbox_session->set_text("");
		fill_session_combobox();
		use_selected_session();
	}
}

void App::on_looping_checked()
{
	if (!selected_sound_session.is_null)
	{
		selected_sound_session.session.set_looping(true);
	}
}

void App::on_looping_unchecked()
{
	if (!selected_sound_session.is_null)
	{
		selected_sound_session.session.set_looping(false);
	}
}

void App::on_btn_set_fade_filter()
{
	if (!selected_sound_session.is_null)
	{
		selected_sound_session.fade_filter.fade_to_volume(spin_session_fade_volume->get_value_float(), spin_session_fade_duration->get_value());
	}
}

void App::set_soundbuffer_session_filters(SoundSession &sound_session)
{
	if (chk_apply_session->is_checked())
	{
		if (chk_echo_filter->is_checked())
		{
			sound_session.echo_filter = EchoFilter(spin_filter_echo_buffer_size->get_value(), spin_filter_echo_shift_factor->get_value_float());
			sound_session.session.add_filter(sound_session.echo_filter);
		}

		if (chk_fade_filter->is_checked())
		{
			sound_session.fade_filter = FadeFilter(spin_filter_fade_volume->get_value_float());
			sound_session.session.add_filter(sound_session.fade_filter);
		}

		if (chk_inverse_echo->is_checked())
		{
			sound_session.inverse_echo_filter = InverseEchoFilter(spin_filter_echo_buffer_size->get_value());
			sound_session.session.add_filter(sound_session.inverse_echo_filter);
		}
	}
}
