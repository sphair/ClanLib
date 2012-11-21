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
**    Harry Storbacka
**    Magnus Norddahl
*/

#pragma once

namespace clan
{

class LineEdit_Impl
{
public:
	LineEdit_Impl() :
		alignment(LineEdit::align_left),
		cursor_pos(0),
		max_length(-1),
		lowercase(false),
		uppercase(false),
		password_mode(false),
		numeric_mode(false),
		numeric_mode_decimals(false),
		readonly(false),
		mouse_moves_left(false),
		mouse_selecting(false),
		selection_start(-1),
		selection_length(0),
		cursor_blink_visible(true),
		clip_start_offset(0),
		clip_end_offset(0),
		decimal_char("."),
		ignore_mouse_events(false),
		cursor_drawing_enabled_when_parent_focused(false),
		select_all_on_focus_gain(true)
	{
		//prop_text_color = GUIThemePartProperty(CssStr::text_color, "black");
	}

	~LineEdit_Impl()
	{
		timer.stop();
		scroll_timer.stop();
	}

	struct VerticalTextPosition
	{
		float top;
		float baseline;
		float bottom;
	};

	void on_process_message(std::shared_ptr<GUIMessage> &msg);
	void on_render(Canvas &canvas, const Rect &update_rect);
	void on_style_changed();
	void on_timer_expired();
	void on_resized();
	void on_scroll_timer_expired();
	void on_enable_changed();
	void update_text_clipping();
	void create_parts();

	Callback_v1<InputEvent &> func_before_edit_changed;
	Callback_v1<InputEvent &> func_after_edit_changed;
	Callback_v0 func_selection_changed;
	Callback_v0 func_focus_gained;
	Callback_v0 func_focus_lost;
	Callback_v0 func_enter_pressed;

	LineEdit *lineedit;
	Timer timer;
	std::string text;
	LineEdit::Alignment alignment;
	int cursor_pos;
	int max_length;
	bool mouse_selecting;
	bool lowercase;
	bool uppercase;
	bool password_mode;
	bool numeric_mode;
	bool numeric_mode_decimals;
	bool readonly;
	int selection_start;
	int selection_length;
	Colorf text_color;
	std::string input_mask;
	std::string decimal_char;
	bool cursor_drawing_enabled_when_parent_focused;

	static std::string break_characters;

	//GUIComponent *part_component;
	GUIComponent *part_selection;
	GUIComponent *part_cursor;

	void move(int steps, InputEvent &e);
	bool insert_text(int pos, const std::string &str);
	void backspace();
	void del();
	int get_character_index(int mouse_x_wincoords);
	int find_next_break_character(int pos);
	int find_previous_break_character(int pos);
	std::string get_visible_text_before_selection();
	std::string get_visible_text_after_selection();
	std::string get_visible_selected_text();
	std::string create_password(std::string::size_type num_letters) const;
	Size get_visual_text_size(Canvas &canvas, Font &font, int pos, int npos) const;
	Size get_visual_text_size(Canvas &canvas, Font &font) const;
	Rect get_cursor_rect();
	Rect get_selection_rect();
	bool input_mask_accepts_input(int cursor_pos, const std::string &str);
	void set_selection_start(int start);
	void set_selection_length(int length);
	void set_text_selection(int start, int length);
	VerticalTextPosition get_vertical_text_align(Canvas &canvas, Font &font, const Rect &content_rect);

	VerticalTextPosition vertical_text_align;
	Timer scroll_timer;

	bool mouse_moves_left;
	bool cursor_blink_visible;
	unsigned int blink_timer;
	int clip_start_offset;
	int clip_end_offset;
	bool ignore_mouse_events;

	struct UndoInfo
	{
		/* set undo text when:
  		   - added char after moving
		   - destructive block operation (del, cut etc)
		   - beginning erase
		*/

		UndoInfo() : first_erase(0), first_text_insert(0) {}
		std::string undo_text;
		bool first_erase;
		bool first_text_insert;
	} undo_info;

	bool select_all_on_focus_gain;

	static const std::string numeric_mode_characters;

	static const int cursor_blink_rate = 500;

};

}
