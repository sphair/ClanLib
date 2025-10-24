
#pragma once

class TextEdit : public CL_GUIComponent
{
public:
	TextEdit(CL_GUIComponent *parent);

	CL_String get_text() const;
	void set_text(const CL_String &text);
	size_t get_cursor_pos() const;

	int get_text_height(int width);

	CL_Callback_1<bool, CL_InputEvent> &func_unhandled_event() { return cb_unhandled_event; }

private:
	void on_render(CL_GraphicContext &gc, const CL_Rect &clip_rect);
	void on_resized();
	bool on_focus_gained();
	bool on_focus_lost();
	bool on_pointer_enter();
	bool on_pointer_exit();
	bool on_input_pressed(const CL_InputEvent &input_event);
	bool on_input_released(const CL_InputEvent &input_event);
	bool on_input_keyboard_pressed(const CL_InputEvent &input_event);
	bool on_input_pointer_pressed(const CL_InputEvent &input_event);
	bool on_input_pointer_released(const CL_InputEvent &input_event);
	bool on_input_doubleclick(const CL_InputEvent &input_event);
	bool on_input_pointer_moved(const CL_InputEvent &input_event);
	void on_blink_cursor();

	void populate_span();
	bool filter_event(const CL_InputEvent &input_event);
	void show_cursor_on_activity();
	void move(int direction, const CL_InputEvent &e);
	void backspace(const CL_InputEvent &e);
	void del(const CL_InputEvent &e);
	void home(const CL_InputEvent &e);
	void end(const CL_InputEvent &e);
	void select_all();
	void copy_to_clipboard();
	void cut_to_clipboard();
	void paste_from_clipboard();
	void insert_text(const CL_String &text);
	void erase_selection();
	int find_next_break_character(int search_start);
	int find_previous_break_character(int search_start);

	CL_GUIThemePart part_background;
	CL_GUIThemePart part_cursor;
	CL_SpanLayout span;
	CL_String text;
	bool mouse_down;
	size_t cursor_pos;
	CL_Timer cursor_timer;
	bool cursor_visible;
	int blink_duration;
	int selection_start, selection_length;

	CL_Callback_1<bool, CL_InputEvent> cb_unhandled_event;

	static CL_String break_characters;
};
