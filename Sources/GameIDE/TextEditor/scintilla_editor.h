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
**    Magnus Norddahl
*/
#pragma once

namespace Scintilla
{
	class CellBuffer;
	struct Sci_TextToFind;
	struct Sci_TextRange;
	struct Sci_RangeToFormat;
	class Document;
}
namespace clan
{
class ScintillaEditor_Impl;

class ScintillaEditor : public clan::GUIComponent
{
public:
	ScintillaEditor(clan::GUIComponent *parent);

	// Text retrieval and modification

	std::string get_text();
	void set_text(const std::string &text);
	void set_savepoint();
	std::string get_line(int line);
	void replace_sel(const std::string &text);
	void set_readonly(bool readOnly);
	bool get_readonly();
	int get_text_range(Scintilla::Sci_TextRange *tr);
	int get_styled_text(Scintilla::Sci_TextRange *tr);
	void allocate(int bytes);
	void add_text(const std::string &s);
	void add_styled_text(int length, Scintilla::CellBuffer *s);
	int append_text(const std::string &s);
	int insert_text(int pos, const std::string &text);
	void clear_all();
	void clear_document_style();
	int get_char_at(int pos);
	int get_style_at(int pos);
	void set_style_bits(int bits);
	int get_style_bits();

	// Searching

	int find_text(int searchFlags, Scintilla::Sci_TextToFind *ttf);
	int search_anchor();
	int search_next(int searchFlags, const std::string &text);
	int search_prev(int searchFlags, const std::string &text);

	// Search and replace using the target

	void set_target_start(int pos);
	int get_target_start();
	void set_target_end(int pos);
	int get_target_end();
	void target_from_selection();
	int set_search_flags(int searchFlags);
	int get_search_flags();
	int search_in_target(const std::string &text);
	int replace_target(const std::string &text);
	int replace_target_re(const std::string &text);
	std::string get_tag(int tagNumber);

	// Overtype

	int set_overtype(bool overType);
	bool get_overtype();

	// Cut, copy and paste

	void cut();
	void copy();
	void paste();
	void clear();
	bool can_paste();
	void copy_allow_line();
	void copy_range(int start, int end);
	void copy_text(const std::string &text);
	void set_paste_convert_endings(bool convert);
	int get_paste_convert_endings();

	// Undo and Redo

	void undo();
	bool can_undo();
	void redo();
	bool can_redo();
	void empty_undo_buffer();
	void set_undo_collection(bool collectUndo);
	bool get_undo_collection();
	void begin_undo_action();
	void end_undo_action();
	void add_undo_action(int token, int flags);

	// Selection and information

	int get_text_length();
	int get_length();
	int get_line_count();
	void set_first_visible_line(int lineDisplay);
	int get_first_visible_line();
	int line_on_screen();
	bool get_modify();
	void set_sel(int anchorPos, int currentPos);
	void goto_pos(int pos);
	void goto_line(int line);
	void set_current_pos(int pos);
	int get_current_pos();
	void set_anchor(int pos);
	int get_anchor();
	void set_selection_start(int pos);
	void set_selection_end(int pos);
	int get_selection_start();
	int get_selection_end();
	void set_empty_selection(int pos);
	void select_all();
	int line_from_position(int pos);
	int position_from_line(int line);
	int get_line_end_position(int line);
	int line_length(int line);
	std::string get_selection_text();
	std::string get_current_line();
	bool selection_is_rectangle();
	void set_selection_mode(int mode);
	int get_selection_mode();
	int get_line_selection_start_position(int line);
	int get_line_selection_end_position(int line);
	void move_caret_inside_view();
	int word_end_position(int position, bool onlyWordCharacters);
	int word_start_position(int position, bool onlyWordCharacters);
	int position_before(int position);
	int position_after(int position);
	int text_width(int styleNumber, const std::string &text);
	int text_height(int line);
	int get_column(int pos);
	int find_column(int line, int column);
	int position_from_point(int x, int y);
	int position_from_point_close(int x, int y);
	int char_position_from_point(int x, int y);
	int char_position_from_point_close(int x, int y);
	int point_x_from_position(int pos);
	int point_y_from_position(int pos);
	void hide_selection(bool hide);
	void choose_caret_x();

	// Multiple Selection and Virtual Space

	void set_multiple_selection(bool multipleSelection);
	bool get_multiple_selection();
	void set_additional_selection_typing(bool additionalSelectionTyping);
	bool get_additional_selection_typing();
	void set_multi_paste(int multiPaste);
	int get_multi_paste();
	void set_virtual_space_options(int virtualSpace);
	int get_virtual_space_options();
	void set_rectangular_selection_modifier(int modifier);
	int get_rectangular_selection_modifier();
	int get_selections();
	void clear_selections();
	void set_selection(int caret, int anchor);
	void add_selection(int caret, int anchor);
	void set_main_selection(int selection);
	int get_main_selection();
	void set_selection_caret(int selection, int pos);
	int get_selection_caret(int selection);
	void set_selection_caret_virtual_space(int selection, int space);
	int get_selection_caret_virtual_space(int selection);
	void set_selection_anchor(int selection, int posAnchor);
	int get_selection_anchor(int selection);
	void set_selection_anchor_virtual_space(int selection, int space);
	int get_selection_anchor_virtual_space(int selection);
	void set_selection_start(int selection, int pos);
	int get_selection_start(int selection);
	void set_selection_end(int selection, int pos);
	int get_selection_end(int selection);
	void set_rectangular_selection_caret(int pos);
	int get_rectangular_selection_caret();
	void set_rectangular_selection_caret_virtual_space(int space);
	int get_rectangular_selection_caret_virtual_space();
	void set_rectangular_selection_anchor(int posAnchor);
	int get_rectangular_selection_anchor();
	void set_rectangular_selection_anchor_virtual_space(int space);
	int get_rectangular_selection_anchor_virtual_space();
	void set_additional_sel_alpha(int alpha);
	int get_additional_sel_alpha();
	void set_additional_sel_foreground(const clan::Colorf &colour);
	void set_additional_sel_background(const clan::Colorf &colour);
	void set_additional_caret_foreground(const clan::Colorf &colour);
	clan::Colorf get_additional_caret_foreground();
	void set_additional_carets_blink(bool additionalCaretsBlink);
	bool get_additional_carets_blink();
	void set_additional_carets_visible(bool additionalCaretsVisible);
	bool get_additional_carets_visible();
	void swap_main_anchor_caret();
	void rotate_selection();

	// Scrolling and automatic scrolling

	void line_scroll(int column, int line);
	void scroll_caret();
	void set_x_caret_policy(int caretPolicy, int caretSlop);
	void set_y_caret_policy(int caretPolicy, int caretSlop);
	void set_visible_policy(int caretPolicy, int caretSlop);
	void set_horz_scrollbar(bool visible);
	bool get_horz_scrollbar();
	void set_vert_scrollbar(bool visible);
	bool get_vert_scrollbar();
	void set_x_offset(int xOffset);
	int get_x_offset();
	void set_scroll_width(int pixelWidth);
	int get_scroll_width();
	void set_scroll_width_tracking(bool tracking);
	bool get_scroll_width_tracking();
	void set_end_at_last_line(bool endAtLastLine);
	bool get_end_at_last_line();

	// White space

	void set_view_ws(int wsMode);
	int get_view_ws();
	void set_whitespace_foreground(bool useWhitespaceForeColour, const clan::Colorf &colour);
	void set_whitespace_background(bool useWhitespaceBackColour, const clan::Colorf &colour);
	void set_whitespace_size(int size);
	int get_whitespace_size();
	void set_extra_ascent(int extraAscent);
	int get_extra_ascent();
	void set_extra_descent(int extraDescent);
	int get_extra_descent();

	// Cursor

	void set_sci_cursor(int curType);
	int get_sci_cursor();

	// Mouse capture

	void set_mouse_down_captures(bool captures);
	bool get_mouse_down_captures();

	// Line endings

	void set_eol_mode(int eolMode);
	int get_eol_mode();
	void convert_eols(int eolMode);
	void set_view_eol(bool visible);
	bool get_view_eol();

	// Styling

	int get_end_styled();
	void start_styling(int pos, int mask);
	void set_styling(int length, int style);
	void set_styling_ex(const std::string &styles);
	void set_line_state(int line, int value);
	int get_line_state(int line);
	int get_max_line_state();

	// Style definition

	void style_reset_default();
	void style_clear_all();
	void style_set_font(int styleNumber, const std::string &fontName);
	std::string style_get_font(int styleNumber);
	void style_set_size(int styleNumber, int sizeInPoints);
	int style_get_size(int styleNumber);
	void style_set_bold(int styleNumber, bool bold);
	bool style_get_bold(int styleNumber);
	void style_set_italic(int styleNumber, bool italic);
	bool style_get_italic(int styleNumber);
	void style_set_underline(int styleNumber, bool underline);
	bool style_get_underline(int styleNumber);
	void style_set_foreground(int styleNumber, const clan::Colorf &colour);
	clan::Colorf style_get_foreground(int styleNumber);
	void style_set_background(int styleNumber, const clan::Colorf &colour);
	clan::Colorf style_get_background(int styleNumber);
	void style_set_eol_filled(int styleNumber, bool eolFilled);
	bool style_get_eol_filled(int styleNumber);
	void style_set_character_set(int styleNumber, int charSet);
	int style_get_character_set(int styleNumber);
	void style_set_case(int styleNumber, int caseMode);
	int style_get_case(int styleNumber);
	void style_set_visible(int styleNumber, bool visible);
	bool style_get_visible(int styleNumber);
	void style_set_changeable(int styleNumber, bool changeable);
	bool style_get_changeable(int styleNumber);
	void style_set_hotspot(int styleNumber, bool hotspot);
	bool style_get_hotspot(int styleNumber);

	// Caret, selection, and hotspot styles

	void set_sel_foreground(bool useSelectionForeColour, const clan::Colorf &colour);
	void set_sel_background(bool useSelectionBackColour, const clan::Colorf &colour);
	void set_sel_alpha(int alpha);
	int get_sel_alpha();
	void set_sel_eol_filled(bool filled);
	bool get_sel_eol_filled();
	void set_caret_foreground(const clan::Colorf &colour);
	clan::Colorf get_caret_foreground();
	void set_caret_line_visible(bool show);
	bool get_caret_line_visible();
	void set_caret_line_background(const clan::Colorf &colour);
	clan::Colorf get_caret_line_background();
	void set_caret_line_background_alpha(int alpha);
	int get_caret_line_background_alpha();
	void set_caret_period(int milliseconds);
	int get_caret_period();
	void set_caret_style(int style);
	int get_caret_style();
	void set_caret_width(int pixels);
	int get_caret_width();
	void set_hotspot_active_foreground(bool useSetting, const clan::Colorf &colour);
	clan::Colorf get_hotspot_active_foreground();
	void set_hotspot_active_background(bool useSetting, const clan::Colorf &colour);
	clan::Colorf get_hotspot_active_background();
	void set_hotspot_active_underline(bool underline);
	bool get_hotspot_active_underline();
	void set_hotspot_single_line(bool singleLine);
	bool get_hotspot_single_line();
	void set_control_char_symbol(int symbol);
	int get_control_char_symbol();
	void set_caret_sticky(int useCaretStickyBehaviour);
	int get_caret_sticky();
	void toggle_caret_sticky();

	// Margins

	void set_margin_type(int margin, int type);
	int get_margin_type(int margin);
	void set_margin_width(int margin, int pixelWidth);
	int get_margin_width(int margin);
	void set_margin_mask(int margin, int mask);
	int get_margin_mask(int margin);
	void set_margin_sensitive(int margin, bool sensitive);
	bool get_margin_sensitive(int margin);
	void set_margin_cursor(int margin, int cursor);
	int get_margin_cursor(int margin);
	void set_margin_left(int pixels);
	int get_margin_left();
	void set_margin_right(int pixels);
	int get_margin_right();
	void set_fold_margin_color(bool useSetting, const clan::Colorf &colour);
	void set_fold_margin_hi_color(bool useSetting, const clan::Colorf &colour);
	void margin_set_text(int line, const std::string &text);
	std::string margin_get_text(int line);
	void margin_set_style(int line, int style);
	int margin_get_style(int line);
	void margin_set_styles(int line, const std::string &styles);
	std::string margin_get_styles(int line);
	void margin_text_clear_all();
	void margin_set_style_offset(int style);
	int margin_get_style_offset();
	void set_margin_options(int marginOptions);
	int get_margin_options();

	// Annotations

	void annotation_set_text(int line, const std::string &text);
	std::string annotation_get_text(int line);
	void annotation_set_style(int line, int style);
	int annotation_get_style(int line);
	void annotation_set_styles(int line, const std::string &styles);
	std::string annotation_get_styles(int line);
	int annotation_get_lines(int line);
	void annotation_clear_all();
	void annotation_set_visible(int visible);
	int annotation_get_visible();
	void annotation_set_style_offset(int style);
	int annotation_get_style_offset();

	// Other settings

	void set_word_chars(const std::string &chars);
	void set_whitespace_chars(const std::string &chars);
	void set_chars_default();
	void grab_focus();

	// Brace highlighting

	void brace_highlight(int pos1, int pos2);
	void brace_badlight(int pos1);
	void brace_highlight_indicator(bool useBraceHighlightIndicator, int indicatorNumber);
	void brace_badlight_indicator(bool useBraceBadLightIndicator, int indicatorNumber);
	void brace_match(int position, int maxReStyle);

	// Tabs and Indentation Guides

	void set_tab_width(int widthInChars);
	int get_tab_width();
	void set_use_tabs(bool useTabs);
	bool get_use_tabs();
	void set_indent(int widthInChars);
	int get_indent();
	void set_tab_indents(bool tabIndents);
	bool get_tab_indents();
	void set_backspace_unindents(bool bsUnIndents);
	bool get_backspace_unindents();
	void set_line_indentation(int line, int indentation);
	int get_line_indentation(int line);
	int get_line_indent_position(int line);
	void set_indentation_guides(int indentView);
	int get_indentation_guides();
	void set_highlight_guide(int column);
	int get_highlight_guide();

	// Markers

	void marker_define(int markerNumber, int markerSymbols);
	void marker_define_image(int markerNumber, const clan::PixelBuffer &pixels);
	void marker_symbol_defined(int markerNumber);
	void marker_set_foreground(int markerNumber, const clan::Colorf &colour);
	void marker_set_background(int markerNumber, const clan::Colorf &colour);
	void marker_set_background_selected(int markerNumber, const clan::Colorf &colour);
	void marker_enable_highlight(int enabled);
	void marker_set_alpha(int markerNumber, int alpha);
	void marker_add(int line, int markerNumber);
	void marker_add_set(int line, int markerMask);
	void marker_delete(int line, int markerNumber);
	void marker_delete_all(int markerNumber);
	int marker_get(int line);
	int marker_next(int lineStart, int markerMask);
	int marker_previous(int lineStart, int markerMask);
	int marker_line_from_handle(int handle);
	int marker_delete_handle(int handle);

	// Indicators

	void indic_set_style(int indicatorNumber, int indicatorStyle);
	int indic_get_style(int indicatorNumber);
	void indic_set_foreground(int indicatorNumber, const clan::Colorf &colour);
	clan::Colorf indic_get_foreground(int indicatorNumber);
	void indic_set_alpha(int indicatorNumber, int alpha);
	int indic_get_alpha(int indicatorNumber);
	void indic_set_outline_alpha(int indicatorNumber, int alpha);
	int indic_get_outline_alpha(int indicatorNumber);
	void indic_set_underline(int indicatorNumber, bool under);
	bool indic_get_underline(int indicatorNumber);

	// Autocompletion

	void autocomplete_show(int lenEntered, const std::string &list);
	void autocomplete_cancel();
	void autocomplete_active();
	int autocomplete_pos_start();
	void autocomplete_complete();
	void autocomplete_stops(const std::string &chars);
	void autocomplete_set_separator(char separator);
	int autocomplete_get_seperator();
	void autocomplete_select(const std::string &select);
	int autocomplete_get_current();
	std::string autocomplete_get_current_text();
	void autocomplete_set_cancel_at_start(bool cancel);
	bool autocomplete_get_cancel_at_start();
	void autocomplete_set_fill_ups(const std::string &chars);
	void autocomplete_set_choose_single(bool chooseSingle);
	bool autocomplete_get_choose_single();
	void autocomplete_set_ignore_case(bool ignoreCase);
	bool autocomplete_get_ignore_case();
	void autocomplete_set_auto_hide(bool autoHide);
	bool autocomplete_get_auto_hide();
	void autocomplete_set_drop_rest_of_word(bool dropRestOfWord);
	bool autocomplete_get_drop_rest_of_word();
	void autocomplete_register_image(int type, const clan::PixelBuffer &pixels);
	int autocomplete_clear_registered_images();
	void autocomplete_set_type_separator(char separatorCharacter);
	int autocomplete_get_type_separator();
	void autocomplete_set_max_height(int rowCount);
	int autocomplete_get_max_height();
	void autocomplete_set_max_width(int characterCount);
	int autocomplete_get_max_width();

	// Call tips

	void calltip_show(int posStart, const std::string &definition);
	void calltip_cancel();
	void calltip_active();
	int calltip_posstart();
	void calltip_set_highlight(int highlightStart, int highlightEnd);
	void calltip_set_backrgound(const clan::Colorf &colour);
	void calltip_set_foreground(const clan::Colorf &colour);
	void calltip_set_foreground_highlight(const clan::Colorf &colour);
	void calltip_use_style(int tabsize);

	// Keyboard commands

	void line_down();
	void line_down_extend();
	void line_down_rect_extend();
	void line_scroll_down();
	void line_up();
	void line_up_extend();
	void line_up_rect_extend();
	void line_scroll_up();
	void paragraph_down();
	void paragraph_down_extend();
	void paragraph_up();
	void paragraph_up_extend();
	void char_left();
	void char_left_extend();
	void char_left_rect_extend();
	void char_right();
	void char_right_extend();
	void char_right_rect_extend();
	void word_left();
	void word_left_extend();
	void word_right();
	void word_right_extend();
	void word_left_end();
	void word_left_end_extend();
	void word_right_end();
	void word_right_end_extend();
	void word_part_left();
	void word_part_left_extend();
	void word_part_right();
	void word_part_right_extend();
	void home();
	void home_extend();
	void home_rect_extend();
	void home_display();
	void home_display_extend();
	void home_wrap();
	void home_wrap_extend();
	void vc_home();
	void vc_home_extend();
	void vc_home_rect_extend();
	void vc_home_wrap();
	void vc_home_wrap_extend();
	void line_end();
	void line_end_extend();
	void line_end_rect_extend();
	void line_end_display();
	void line_end_display_extend();
	void line_end_wrap();
	void line_end_wrap_extend();
	void document_start();
	void document_start_extend();
	void document_end();
	void document_end_extend();
	void page_up();
	void page_up_extend();
	void page_up_rect_extend();
	void page_down();
	void page_down_extend();
	void page_down_rect_extend();
	void stuttered_page_up();
	void stuttered_page_up_extend();
	void stuttered_page_down();
	void stuttered_page_down_extend();
	void delete_back();
	void delete_back_not_line();
	void del_word_left();
	void del_word_right();
	void del_word_right_end();
	void del_line_left();
	void del_line_right();
	void line_delete();
	void line_cut();
	void line_copy();
	void line_transpose();
	void line_duplicate();
	void lowercase();
	void uppercase();
	void cancel();
	void edit_toggle_overtype();
	void newline();
	void formfeed();
	void tab();
	void backtab();
	void selection_duplicate();
	void vertical_centre_caret();
	void move_selected_lines_up();
	void move_selected_lines_down();
	void scroll_to_start();
	void scroll_to_end();

	// Key bindings

	void assign_cmd_key(int keyDefinition, int sciCommand);
	void clear_cmd_key(int keyDefinition);
	void clear_all_cmd_keys();

	// Popup edit menu

	void use_popup(bool bEnablePopup);

	// Macro recording

	void start_record();
	void stop_record();
	void play_record(const std::string &str);

	// Printing

	void format_range(bool bDraw, Scintilla::Sci_RangeToFormat *pfr);
	void set_print_magnification(int magnification);
	int get_print_magnification();
	void set_print_colou_mode(int mode);
	int get_print_color_mode();
	void set_print_wrap_mode();
	int get_print_wrap_mode();

	// Multiple views

	Scintilla::Document *get_doc_pointer();
	void set_doc_pointer(Scintilla::Document *pDoc);
	Scintilla::Document *create_document();
	void add_ref_document(Scintilla::Document *pDoc);
	void release_document(Scintilla::Document *pDoc);

	// Folding

	int visible_from_doc_line(int docLine);
	int doc_line_from_visible(int displayLine);
	void show_lines(int lineStart, int lineEnd);
	void hide_lines(int lineStart, int lineEnd);
	int get_line_visible(int line);
	void set_fold_level(int line, int level);
	int get_fold_level(int line);
	void set_fold_flags(int flags);
	int get_last_child(int line, int level);
	int get_fold_parent(int line);
	void set_fold_expanded(int line, bool expanded);
	bool get_fold_expanded(int line);
	void contracted_fold_next(int lineStart);
	void toggle_fold(int line);
	void ensure_visible(int line);
	void ensure_visible_enforce_policy(int line);

	// Line wrapping

	void set_wrap_mode(int wrapMode);
	int get_wrap_mode();
	void set_wrap_visual_flags(int wrapVisualFlags);
	int get_wrap_visual_flags();
	void set_wrap_visual_flags_location(int wrapVisualFlagsLocation);
	int get_wrap_visual_flags_location();
	void set_wrap_indent_mode(int indentMode);
	int get_wrap_indent_mode();
	void set_wrap_start_indent(int indent);
	int get_wrap_start_indent();
	void set_layout_cache(int cacheMode);
	int get_layout_cache();
	void set_position_cache(int size);
	int get_position_cache();
	void line_split(int pixelWidth);
	void lines_join();
	int wrap_count(int docLine);

	// Zooming

	void zoom_in();
	void zoom_out();
	void set_zoom(int zoomInPoints);
	int get_zoom();

	// Long lines

	void set_edge_mode(int mode);
	int get_edge_mode();
	void set_edge_column(int column);
	int get_edge_column();
	void set_edge_color(const clan::Colorf &colour);
	clan::Colorf get_edge_color();

	// Lexer

	void set_lexer(int lexer);
	int get_lexer();
	void set_lexer_language(const std::string &name);
	std::string get_lexer_language();
	void load_lexer_library(const std::string &path);
	int colorize(int start, int end);
	int change_lexer_state(int start, int end);
	std::vector<std::string> property_names();
	int property_type(const std::string &name);
	std::string describe_property(const std::string &name);
	void set_property(const std::string &key, const std::string &value);
	std::string get_property(const std::string &key);
	std::string get_property_expanded(const std::string &key);
	int get_property_int(const std::string &key, int default_);
	std::vector<std::string> describe_keyword_sets();
	void set_keywords(int keyWordSet, const std::string &keyWordList);
	int get_style_bits_needed();

	// Notifications

	void set_mod_event_mask(int eventMask);
	int get_mod_event_mask();
	void set_mouse_dwell_time(int milliseconds);
	int get_mouse_dwell_time();
	void set_identifier(int identifier);
	int get_identifier();

	clan::Callback_v0 &func_style_needed();
	clan::Callback_v0 &func_char_added();
	clan::Callback_v0 &func_save_point_reached();
	clan::Callback_v0 &func_save_point_left();
	clan::Callback_v0 &func_modify_attempt_read_only();
	clan::Callback_v0 &func_key();
	clan::Callback_v0 &func_double_click();
	clan::Callback_v0 &func_update_ui();
	clan::Callback_v0 &func_modified();

	// void cb_macro_record(const std::string &macro_command_recorded)
	clan::Callback_v1<const std::string &> &func_macro_record();

	// void cb_margin_click(bool shift, bool ctrl, bool alt, int position, int margin_number)
	clan::Callback_v5<bool, bool, bool, int, int> &func_margin_click();

	// void cb_need_shown(int position, int length)
	clan::Callback_v2<int, int> &func_need_shown();

	clan::Callback_v0 &func_painted();
	clan::Callback_v0 &func_userlist_selection();

	// void dwell_start(int position, clan::Point mouse_pos)
	clan::Callback_v2<int, clan::Point> &func_dwell_start();

	// void dwell_end(int position, clan::Point mouse_pos)
	clan::Callback_v2<int, clan::Point> &func_dwell_end();

	clan::Callback_v0 &func_zoom();
	clan::Callback_v0 &func_hotspot_click();
	clan::Callback_v0 &func_hotspot_double_click();
	clan::Callback_v0 &func_hotspot_release_click();
	clan::Callback_v0 &func_indicator_click();
	clan::Callback_v0 &func_indicator_release();
	clan::Callback_v0 &func_calltip_click();
	clan::Callback_v0 &func_autocomplete_selection();
	clan::Callback_v0 &func_autocomplete_cancelled();
	clan::Callback_v0 &func_autocomplete_char_deleted();
	clan::Callback_v0 &func_change();

private:
	std::shared_ptr<ScintillaEditor_Impl> impl;
};

}

