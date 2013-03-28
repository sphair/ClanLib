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

#include "GameIDE/precomp.h"
#include "scintilla_editor.h"
#include "scintilla_editor_impl.h"

namespace clan
{

ScintillaEditor::ScintillaEditor(GUIComponent *parent)
: GUIComponent(parent, "scintilla")
{
	impl = std::shared_ptr<ScintillaEditor_Impl>(new ScintillaEditor_Impl(this));
	impl->set_code_page(SC_CP_UTF8);
	impl->set_keys_unicode(true);
	impl->set_buffered_draw(false);
}

std::string ScintillaEditor::get_text()
{
	int length = impl->get_text(0, 0) + 1;
	DataBuffer buffer(length);
	length = clamp(impl->get_text(buffer.get_size(), buffer.get_data()), 1, length) - 1;
	return std::string(buffer.get_data(), length);
}

void ScintillaEditor::set_text(const std::string &text)
{
	impl->set_text(text.c_str());
}

void ScintillaEditor::set_savepoint()
{
	impl->set_savepoint();
}

std::string ScintillaEditor::get_line(int line)
{
	int length = impl->get_line(line, 0) + 1;
	DataBuffer buffer(length);
	length = clamp(impl->get_line(line, buffer.get_data()), 1, length) - 1;
	return std::string(buffer.get_data(), length);
}

void ScintillaEditor::replace_sel(const std::string &text)
{
	impl->replace_sel(text.c_str());
}

void ScintillaEditor::set_readonly(bool readOnly)
{
	impl->set_readonly(readOnly);
}

bool ScintillaEditor::get_readonly()
{
	return impl->get_readonly();
}

int ScintillaEditor::get_text_range(Scintilla::Sci_TextRange *tr)
{
	return impl->get_text_range(tr);
}

int ScintillaEditor::get_styled_text(Scintilla::Sci_TextRange *tr)
{
	return impl->get_styled_text(tr);
}

void ScintillaEditor::allocate(int bytes)
{
	impl->allocate(bytes);
}

void ScintillaEditor::add_text(const std::string &s)
{
	impl->add_text(s.length(), s.c_str());
}

void ScintillaEditor::add_styled_text(int length, Scintilla::CellBuffer *s)
{
	impl->add_styled_text(length, s);
}

int ScintillaEditor::append_text(const std::string &s)
{
	return impl->append_text(s.length(), s.c_str());
}

int ScintillaEditor::insert_text(int pos, const std::string &text)
{
	return impl->insert_text(pos, text.c_str());
}

void ScintillaEditor::clear_all()
{
	impl->clear_all();
}

void ScintillaEditor::clear_document_style()
{
	impl->clear_document_style();
}

int ScintillaEditor::get_char_at(int pos)
{
	return impl->get_char_at(pos);
}

int ScintillaEditor::get_style_at(int pos)
{
	return impl->get_style_at(pos);
}

void ScintillaEditor::set_style_bits(int bits)
{
	impl->set_style_bits(bits);
}

int ScintillaEditor::get_style_bits()
{
	return impl->get_style_bits();
}

int ScintillaEditor::find_text(int searchFlags, Scintilla::Sci_TextToFind *ttf)
{
	return impl->find_text(searchFlags, ttf);
}

int ScintillaEditor::search_anchor()
{
	return impl->search_anchor();
}

int ScintillaEditor::search_next(int searchFlags, const std::string &text)
{
	return impl->search_next(searchFlags, text.c_str());
}

int ScintillaEditor::search_prev(int searchFlags, const std::string &text)
{
	return impl->search_prev(searchFlags, text.c_str());
}

void ScintillaEditor::set_target_start(int pos)
{
	impl->set_target_start(pos);
}

int ScintillaEditor::get_target_start()
{
	return impl->get_target_start();
}

void ScintillaEditor::set_target_end(int pos)
{
	impl->set_target_end(pos);
}

int ScintillaEditor::get_target_end()
{
	return impl->get_target_end();
}

void ScintillaEditor::target_from_selection()
{
	impl->target_from_selection();
}

int ScintillaEditor::set_search_flags(int searchFlags)
{
	return impl->set_search_flags(searchFlags);
}

int ScintillaEditor::get_search_flags()
{
	return impl->get_search_flags();
}

int ScintillaEditor::search_in_target(const std::string &text)
{
	return impl->search_in_target(text.length(), text.c_str());
}

int ScintillaEditor::replace_target(const std::string &text)
{
	return impl->replace_target(text.length(), text.c_str());
}

int ScintillaEditor::replace_target_re(const std::string &text)
{
	return impl->replace_target_re(text.length(), text.c_str());
}

std::string ScintillaEditor::get_tag(int tagNumber)
{
	int length = impl->get_tag(tagNumber, 0) + 1;
	DataBuffer buffer(length);
	length = clamp(impl->get_tag(tagNumber, buffer.get_data()), 1, length) - 1;
	return std::string(buffer.get_data(), length);
}

int ScintillaEditor::set_overtype(bool overType)
{
	return impl->set_overtype(overType);
}

bool ScintillaEditor::get_overtype()
{
	return impl->get_overtype();
}

void ScintillaEditor::cut()
{
	impl->cut();
}

void ScintillaEditor::copy()
{
	impl->copy();
}

void ScintillaEditor::paste()
{
	impl->paste();
}

void ScintillaEditor::clear()
{
	impl->clear();
}

bool ScintillaEditor::can_paste()
{
	return impl->can_paste();
}

void ScintillaEditor::copy_allow_line()
{
	impl->copy_allow_line();
}

void ScintillaEditor::copy_range(int start, int end)
{
	impl->copy_range(start, end);
}

void ScintillaEditor::copy_text(const std::string &text)
{
	impl->copy_text(text.length(), text.c_str());
}

void ScintillaEditor::set_paste_convert_endings(bool convert)
{
	impl->set_paste_convert_endings(convert);
}

int ScintillaEditor::get_paste_convert_endings()
{
	return impl->get_paste_convert_endings();
}

void ScintillaEditor::undo()
{
	impl->undo();
}

bool ScintillaEditor::can_undo()
{
	return impl->can_undo();
}

void ScintillaEditor::redo()
{
	impl->redo();
}

bool ScintillaEditor::can_redo()
{
	return impl->can_redo();
}

void ScintillaEditor::empty_undo_buffer()
{
	impl->empty_undo_buffer();
}

void ScintillaEditor::set_undo_collection(bool collectUndo)
{
	impl->set_undo_collection(collectUndo);
}

bool ScintillaEditor::get_undo_collection()
{
	return impl->get_undo_collection();
}

void ScintillaEditor::begin_undo_action()
{
	impl->begin_undo_action();
}

void ScintillaEditor::end_undo_action()
{
	impl->end_undo_action();
}

void ScintillaEditor::add_undo_action(int token, int flags)
{
	impl->add_undo_action(token, flags);
}

int ScintillaEditor::get_text_length()
{
	return impl->get_text_length();
}

int ScintillaEditor::get_length()
{
	return impl->get_length();
}

int ScintillaEditor::get_line_count()
{
	return impl->get_line_count();
}

void ScintillaEditor::set_first_visible_line(int lineDisplay)
{
	impl->set_first_visible_line(lineDisplay);
}

int ScintillaEditor::get_first_visible_line()
{
	return impl->get_first_visible_line();
}

int ScintillaEditor::line_on_screen()
{
	return impl->line_on_screen();
}

bool ScintillaEditor::get_modify()
{
	return impl->get_modify();
}

void ScintillaEditor::set_sel(int anchorPos, int currentPos)
{
	impl->set_sel(anchorPos, currentPos);
}

void ScintillaEditor::goto_pos(int pos)
{
	impl->goto_pos(pos);
}

void ScintillaEditor::goto_line(int line)
{
	impl->goto_line(line);
}

void ScintillaEditor::set_current_pos(int pos)
{
	impl->set_current_pos(pos);
}

int ScintillaEditor::get_current_pos()
{
	return impl->get_current_pos();
}

void ScintillaEditor::set_anchor(int pos)
{
	impl->set_anchor(pos);
}

int ScintillaEditor::get_anchor()
{
	return impl->get_anchor();
}

void ScintillaEditor::set_selection_start(int pos)
{
	impl->set_selection_start(pos);
}

void ScintillaEditor::set_selection_end(int pos)
{
	impl->set_selection_end(pos);
}

int ScintillaEditor::get_selection_start()
{
	return impl->get_selection_start();
}

int ScintillaEditor::get_selection_end()
{
	return impl->get_selection_end();
}

void ScintillaEditor::set_empty_selection(int pos)
{
	impl->set_empty_selection(pos);
}

void ScintillaEditor::select_all()
{
	impl->select_all();
}

int ScintillaEditor::line_from_position(int pos)
{
	return impl->line_from_position(pos);
}

int ScintillaEditor::position_from_line(int line)
{
	return impl->position_from_line(line);
}

int ScintillaEditor::get_line_end_position(int line)
{
	return impl->get_line_end_position(line);
}

int ScintillaEditor::line_length(int line)
{
	return impl->line_length(line);
}

std::string ScintillaEditor::get_selection_text()
{
	int length = impl->get_selection_text(0) + 1;
	DataBuffer buffer(length);
	length = clamp(impl->get_selection_text(buffer.get_data()), 1, length) - 1;
	return std::string(buffer.get_data(), length);
}

std::string ScintillaEditor::get_current_line()
{
	int length = impl->get_current_line(0, 0) + 1;
	DataBuffer buffer(length);
	length = clamp(impl->get_current_line(buffer.get_size(), buffer.get_data()), 1, length) - 1;
	return std::string(buffer.get_data(), length);
}

bool ScintillaEditor::selection_is_rectangle()
{
	return impl->selection_is_rectangle();
}

void ScintillaEditor::set_selection_mode(int mode)
{
	impl->set_selection_mode(mode);
}

int ScintillaEditor::get_selection_mode()
{
	return impl->get_selection_mode();
}

int ScintillaEditor::get_line_selection_start_position(int line)
{
	return impl->get_line_selection_start_position(line);
}

int ScintillaEditor::get_line_selection_end_position(int line)
{
	return impl->get_line_selection_end_position(line);
}

void ScintillaEditor::move_caret_inside_view()
{
	impl->move_caret_inside_view();
}

int ScintillaEditor::word_end_position(int position, bool onlyWordCharacters)
{
	return impl->word_end_position(position, onlyWordCharacters);
}

int ScintillaEditor::word_start_position(int position, bool onlyWordCharacters)
{
	return impl->word_start_position(position, onlyWordCharacters);
}

int ScintillaEditor::position_before(int position)
{
	return impl->position_before(position);
}

int ScintillaEditor::position_after(int position)
{
	return impl->position_after(position);
}

int ScintillaEditor::text_width(int styleNumber, const std::string &text)
{
	return impl->text_width(styleNumber, text.c_str());
}

int ScintillaEditor::text_height(int line)
{
	return impl->text_height(line);
}

int ScintillaEditor::get_column(int pos)
{
	return impl->get_column(pos);
}

int ScintillaEditor::find_column(int line, int column)
{
	return impl->find_column(line, column);
}

int ScintillaEditor::position_from_point(int x, int y)
{
	return impl->position_from_point(x, y);
}

int ScintillaEditor::position_from_point_close(int x, int y)
{
	return impl->position_from_point_close(x, y);
}

int ScintillaEditor::char_position_from_point(int x, int y)
{
	return impl->char_position_from_point(x, y);
}

int ScintillaEditor::char_position_from_point_close(int x, int y)
{
	return impl->char_position_from_point_close(x, y);
}

int ScintillaEditor::point_x_from_position(int pos)
{
	return impl->point_x_from_position(pos);
}

int ScintillaEditor::point_y_from_position(int pos)
{
	return impl->point_y_from_position(pos);
}

void ScintillaEditor::hide_selection(bool hide)
{
	impl->hide_selection(hide);
}

void ScintillaEditor::choose_caret_x()
{
	impl->choose_caret_x();
}

void ScintillaEditor::set_multiple_selection(bool multipleSelection)
{
	impl->set_multiple_selection(multipleSelection);
}

bool ScintillaEditor::get_multiple_selection()
{
	return impl->get_multiple_selection();
}

void ScintillaEditor::set_additional_selection_typing(bool additionalSelectionTyping)
{
	impl->set_additional_selection_typing(additionalSelectionTyping);
}

bool ScintillaEditor::get_additional_selection_typing()
{
	return impl->get_additional_selection_typing();
}

void ScintillaEditor::set_multi_paste(int multiPaste)
{
	impl->set_multi_paste(multiPaste);
}

int ScintillaEditor::get_multi_paste()
{
	return impl->get_multi_paste();
}

void ScintillaEditor::set_virtual_space_options(int virtualSpace)
{
	impl->set_virtual_space_options(virtualSpace);
}

int ScintillaEditor::get_virtual_space_options()
{
	return impl->get_virtual_space_options();
}

void ScintillaEditor::set_rectangular_selection_modifier(int modifier)
{
	impl->set_rectangular_selection_modifier(modifier);
}

int ScintillaEditor::get_rectangular_selection_modifier()
{
	return impl->get_rectangular_selection_modifier();
}

int ScintillaEditor::get_selections()
{
	return impl->get_selections();
}

void ScintillaEditor::clear_selections()
{
	impl->clear_selections();
}

void ScintillaEditor::set_selection(int caret, int anchor)
{
	impl->set_selection(caret, anchor);
}

void ScintillaEditor::add_selection(int caret, int anchor)
{
	impl->add_selection(caret, anchor);
}

void ScintillaEditor::set_main_selection(int selection)
{
	impl->set_main_selection(selection);
}

int ScintillaEditor::get_main_selection()
{
	return impl->get_main_selection();
}

void ScintillaEditor::set_selection_caret(int selection, int pos)
{
	impl->set_selection_caret(selection, pos);
}

int ScintillaEditor::get_selection_caret(int selection)
{
	return impl->get_selection_caret(selection);
}

void ScintillaEditor::set_selection_caret_virtual_space(int selection, int space)
{
	impl->set_selection_caret_virtual_space(selection, space);
}

int ScintillaEditor::get_selection_caret_virtual_space(int selection)
{
	return impl->get_selection_caret_virtual_space(selection);
}

void ScintillaEditor::set_selection_anchor(int selection, int posAnchor)
{
	impl->set_selection_anchor(selection, posAnchor);
}

int ScintillaEditor::get_selection_anchor(int selection)
{
	return impl->get_selection_anchor(selection);
}

void ScintillaEditor::set_selection_anchor_virtual_space(int selection, int space)
{
	impl->set_selection_anchor_virtual_space(selection, space);
}

int ScintillaEditor::get_selection_anchor_virtual_space(int selection)
{
	return impl->get_selection_anchor_virtual_space(selection);
}

void ScintillaEditor::set_selection_start(int selection, int pos)
{
	impl->set_selection_start(selection, pos);
}

int ScintillaEditor::get_selection_start(int selection)
{
	return impl->get_selection_start(selection);
}

void ScintillaEditor::set_selection_end(int selection, int pos)
{
	impl->set_selection_end(selection, pos);
}

int ScintillaEditor::get_selection_end(int selection)
{
	return impl->get_selection_end(selection);
}

void ScintillaEditor::set_rectangular_selection_caret(int pos)
{
	impl->set_rectangular_selection_caret(pos);
}

int ScintillaEditor::get_rectangular_selection_caret()
{
	return impl->get_rectangular_selection_caret();
}

void ScintillaEditor::set_rectangular_selection_caret_virtual_space(int space)
{
	impl->set_rectangular_selection_caret_virtual_space(space);
}

int ScintillaEditor::get_rectangular_selection_caret_virtual_space()
{
	return impl->get_rectangular_selection_caret_virtual_space();
}

void ScintillaEditor::set_rectangular_selection_anchor(int posAnchor)
{
	impl->set_rectangular_selection_anchor(posAnchor);
}

int ScintillaEditor::get_rectangular_selection_anchor()
{
	return impl->get_rectangular_selection_anchor();
}

void ScintillaEditor::set_rectangular_selection_anchor_virtual_space(int space)
{
	impl->set_rectangular_selection_anchor_virtual_space(space);
}

int ScintillaEditor::get_rectangular_selection_anchor_virtual_space()
{
	return impl->get_rectangular_selection_caret_virtual_space();
}

void ScintillaEditor::set_additional_sel_alpha(int alpha)
{
	impl->set_additional_sel_alpha(alpha);
}

int ScintillaEditor::get_additional_sel_alpha()
{
	return impl->get_additional_sel_alpha();
}

void ScintillaEditor::set_additional_sel_foreground(const Colorf &colour)
{
	impl->set_additional_sel_foreground(impl->to_int_color(colour));
}

void ScintillaEditor::set_additional_sel_background(const Colorf &colour)
{
	impl->set_additional_sel_background(impl->to_int_color(colour));
}

void ScintillaEditor::set_additional_caret_foreground(const Colorf &colour)
{
	impl->set_additional_caret_foreground(impl->to_int_color(colour));
}

Colorf ScintillaEditor::get_additional_caret_foreground()
{
	return impl->from_int_color(impl->get_additional_caret_foreground());
}

void ScintillaEditor::set_additional_carets_blink(bool additionalCaretsBlink)
{
	impl->set_additional_carets_blink(additionalCaretsBlink);
}

bool ScintillaEditor::get_additional_carets_blink()
{
	return impl->get_additional_carets_blink();
}

void ScintillaEditor::set_additional_carets_visible(bool additionalCaretsVisible)
{
	impl->set_additional_carets_visible(additionalCaretsVisible);
}

bool ScintillaEditor::get_additional_carets_visible()
{
	return impl->get_additional_carets_visible();
}

void ScintillaEditor::swap_main_anchor_caret()
{
	impl->swap_main_anchor_caret();
}

void ScintillaEditor::rotate_selection()
{
	impl->rotate_selection();
}

void ScintillaEditor::line_scroll(int column, int line)
{
	impl->line_scroll(column, line);
}

void ScintillaEditor::scroll_caret()
{
	impl->scroll_caret();
}

void ScintillaEditor::set_x_caret_policy(int caretPolicy, int caretSlop)
{
	impl->set_x_caret_policy(caretPolicy, caretSlop);
}

void ScintillaEditor::set_y_caret_policy(int caretPolicy, int caretSlop)
{
	impl->set_y_caret_policy(caretPolicy, caretSlop);
}

void ScintillaEditor::set_visible_policy(int caretPolicy, int caretSlop)
{
	impl->set_visible_policy(caretPolicy, caretSlop);
}

void ScintillaEditor::set_horz_scrollbar(bool visible)
{
	impl->set_horz_scrollbar(visible);
}

bool ScintillaEditor::get_horz_scrollbar()
{
	return impl->get_horz_scrollbar();
}

void ScintillaEditor::set_vert_scrollbar(bool visible)
{
	impl->set_vert_scrollbar(visible);
}

bool ScintillaEditor::get_vert_scrollbar()
{
	return impl->get_vert_scrollbar();
}

void ScintillaEditor::set_x_offset(int xOffset)
{
	impl->set_x_offset(xOffset);
}

int ScintillaEditor::get_x_offset()
{
	return impl->get_x_offset();
}

void ScintillaEditor::set_scroll_width(int pixelWidth)
{
	impl->set_scroll_width(pixelWidth);
}

int ScintillaEditor::get_scroll_width()
{
	return impl->get_scroll_width();
}

void ScintillaEditor::set_scroll_width_tracking(bool tracking)
{
	impl->set_scroll_width_tracking(tracking);
}

bool ScintillaEditor::get_scroll_width_tracking()
{
	return impl->get_scroll_width_tracking();
}

void ScintillaEditor::set_end_at_last_line(bool endAtLastLine)
{
	impl->set_end_at_last_line(endAtLastLine);
}

bool ScintillaEditor::get_end_at_last_line()
{
	return impl->get_end_at_last_line();
}

void ScintillaEditor::set_view_ws(int wsMode)
{
	impl->set_view_ws(wsMode);
}

int ScintillaEditor::get_view_ws()
{
	return impl->get_view_ws();
}

void ScintillaEditor::set_whitespace_foreground(bool useWhitespaceForeColour, const Colorf &colour)
{
	impl->set_whitespace_foreground(useWhitespaceForeColour, impl->to_int_color(colour));
}

void ScintillaEditor::set_whitespace_background(bool useWhitespaceBackColour, const Colorf &colour)
{
	impl->set_whitespace_background(useWhitespaceBackColour, impl->to_int_color(colour));
}

void ScintillaEditor::set_whitespace_size(int size)
{
	impl->set_whitespace_size(size);
}

int ScintillaEditor::get_whitespace_size()
{
	return impl->get_whitespace_size();
}

void ScintillaEditor::set_extra_ascent(int extraAscent)
{
	impl->set_extra_ascent(extraAscent);
}

int ScintillaEditor::get_extra_ascent()
{
	return impl->get_extra_ascent();
}

void ScintillaEditor::set_extra_descent(int extraDescent)
{
	impl->set_extra_descent(extraDescent);
}

int ScintillaEditor::get_extra_descent()
{
	return impl->get_extra_descent();
}

void ScintillaEditor::set_sci_cursor(int curType)
{
	impl->set_cursor(curType);
}

int ScintillaEditor::get_sci_cursor()
{
	return impl->get_cursor();
}

void ScintillaEditor::set_mouse_down_captures(bool captures)
{
	impl->set_mouse_down_captures(captures);
}

bool ScintillaEditor::get_mouse_down_captures()
{
	return impl->get_mouse_down_captures();
}

void ScintillaEditor::set_eol_mode(int eolMode)
{
	impl->set_eol_mode(eolMode);
}

int ScintillaEditor::get_eol_mode()
{
	return impl->get_eol_mode();
}

void ScintillaEditor::convert_eols(int eolMode)
{
	impl->convert_eols(eolMode);
}

void ScintillaEditor::set_view_eol(bool visible)
{
	impl->set_view_eol(visible);
}

bool ScintillaEditor::get_view_eol()
{
	return impl->get_view_eol();
}

int ScintillaEditor::get_end_styled()
{
	return impl->get_end_styled();
}

void ScintillaEditor::start_styling(int pos, int mask)
{
	impl->start_styling(pos, mask);
}

void ScintillaEditor::set_styling(int length, int style)
{
	impl->set_styling(length, style);
}

void ScintillaEditor::set_styling_ex(const std::string &styles)
{
	impl->set_styling_ex(styles.length(), styles.c_str());
}

void ScintillaEditor::set_line_state(int line, int value)
{
	impl->set_line_state(line, value);
}

int ScintillaEditor::get_line_state(int line)
{
	return impl->get_line_state(line);
}

int ScintillaEditor::get_max_line_state()
{
	return impl->get_max_line_state();
}

void ScintillaEditor::style_reset_default()
{
	return impl->style_reset_default();
}

void ScintillaEditor::style_clear_all()
{
	return impl->style_clear_all();
}

void ScintillaEditor::style_set_font(int styleNumber, const std::string &fontName)
{
	impl->style_set_font(styleNumber, fontName.c_str());
}

std::string ScintillaEditor::style_get_font(int styleNumber)
{
	int length = impl->style_get_font(styleNumber, 0) + 1;
	DataBuffer buffer(length);
	length = clamp(impl->style_get_font(styleNumber, buffer.get_data()), 1, length) - 1;
	return std::string(buffer.get_data(), length);
}

void ScintillaEditor::style_set_size(int styleNumber, int sizeInPoints)
{
	impl->style_set_size(styleNumber, sizeInPoints);
}

int ScintillaEditor::style_get_size(int styleNumber)
{
	return impl->style_get_size(styleNumber);
}

void ScintillaEditor::style_set_bold(int styleNumber, bool bold)
{
	impl->style_set_bold(styleNumber, bold);
}

bool ScintillaEditor::style_get_bold(int styleNumber)
{
	return impl->style_get_bold(styleNumber);
}

void ScintillaEditor::style_set_italic(int styleNumber, bool italic)
{
	impl->style_set_italic(styleNumber, italic);
}

bool ScintillaEditor::style_get_italic(int styleNumber)
{
	return impl->style_get_italic(styleNumber);
}

void ScintillaEditor::style_set_underline(int styleNumber, bool underline)
{
	impl->style_set_underline(styleNumber, underline);
}

bool ScintillaEditor::style_get_underline(int styleNumber)
{
	return impl->style_get_underline(styleNumber);
}

void ScintillaEditor::style_set_foreground(int styleNumber, const Colorf &colour)
{
	impl->style_set_foreground(styleNumber, impl->to_int_color(colour));
}

Colorf ScintillaEditor::style_get_foreground(int styleNumber)
{
	return impl->from_int_color(impl->style_get_foreground(styleNumber));
}

void ScintillaEditor::style_set_background(int styleNumber, const Colorf &colour)
{
	impl->style_set_background(styleNumber, impl->to_int_color(colour));
}

Colorf ScintillaEditor::style_get_background(int styleNumber)
{
	return impl->from_int_color(impl->style_get_background(styleNumber));
}

void ScintillaEditor::style_set_eol_filled(int styleNumber, bool eolFilled)
{
	impl->style_set_eol_filled(styleNumber, eolFilled);
}

bool ScintillaEditor::style_get_eol_filled(int styleNumber)
{
	return impl->style_get_eol_filled(styleNumber);
}

void ScintillaEditor::style_set_character_set(int styleNumber, int charSet)
{
	impl->style_set_character_set(styleNumber, charSet);
}

int ScintillaEditor::style_get_character_set(int styleNumber)
{
	return impl->style_get_character_set(styleNumber);
}

void ScintillaEditor::style_set_case(int styleNumber, int caseMode)
{
	impl->style_set_case(styleNumber, caseMode);
}

int ScintillaEditor::style_get_case(int styleNumber)
{
	return impl->style_get_case(styleNumber);
}

void ScintillaEditor::style_set_visible(int styleNumber, bool visible)
{
	impl->style_set_visible(styleNumber, visible);
}

bool ScintillaEditor::style_get_visible(int styleNumber)
{
	return impl->style_get_visible(styleNumber);
}

void ScintillaEditor::style_set_changeable(int styleNumber, bool changeable)
{
	impl->style_set_changeable(styleNumber, changeable);
}

bool ScintillaEditor::style_get_changeable(int styleNumber)
{
	return impl->style_get_changeable(styleNumber);
}

void ScintillaEditor::style_set_hotspot(int styleNumber, bool hotspot)
{
	impl->style_set_hotspot(styleNumber, hotspot);
}

bool ScintillaEditor::style_get_hotspot(int styleNumber)
{
	return impl->style_get_hotspot(styleNumber);
}

void ScintillaEditor::set_sel_foreground(bool useSelectionForeColour, const Colorf &colour)
{
	impl->set_sel_foreground(useSelectionForeColour, impl->to_int_color(colour));
}

void ScintillaEditor::set_sel_background(bool useSelectionBackColour, const Colorf &colour)
{
	impl->set_sel_background(useSelectionBackColour, impl->to_int_color(colour));
}

void ScintillaEditor::set_sel_alpha(int alpha)
{
	impl->set_sel_alpha(alpha);
}

int ScintillaEditor::get_sel_alpha()
{
	return impl->get_sel_alpha();
}

void ScintillaEditor::set_sel_eol_filled(bool filled)
{
	impl->set_sel_eol_filled(filled);
}

bool ScintillaEditor::get_sel_eol_filled()
{
	return impl->get_sel_eol_filled();
}

void ScintillaEditor::set_caret_foreground(const Colorf &colour)
{
	impl->set_caret_foreground(impl->to_int_color(colour));
}

Colorf ScintillaEditor::get_caret_foreground()
{
	return impl->from_int_color(impl->get_caret_foreground());
}

void ScintillaEditor::set_caret_line_visible(bool show)
{
	impl->set_caret_line_visible(show);
}

bool ScintillaEditor::get_caret_line_visible()
{
	return impl->get_caret_line_visible();
}

void ScintillaEditor::set_caret_line_background(const Colorf &colour)
{
	impl->set_caret_line_background(impl->to_int_color(colour));
}

Colorf ScintillaEditor::get_caret_line_background()
{
	return impl->from_int_color(impl->get_caret_line_background());
}

void ScintillaEditor::set_caret_line_background_alpha(int alpha)
{
	impl->set_caret_line_background_alpha(alpha);
}

int ScintillaEditor::get_caret_line_background_alpha()
{
	return impl->get_caret_line_background_alpha();
}

void ScintillaEditor::set_caret_period(int milliseconds)
{
	impl->set_caret_period(milliseconds);
}

int ScintillaEditor::get_caret_period()
{
	return impl->get_caret_period();
}

void ScintillaEditor::set_caret_style(int style)
{
	impl->set_caret_style(style);
}

int ScintillaEditor::get_caret_style()
{
	return impl->get_caret_style();
}

void ScintillaEditor::set_caret_width(int pixels)
{
	impl->set_caret_width(pixels);
}

int ScintillaEditor::get_caret_width()
{
	return impl->get_caret_width();
}

void ScintillaEditor::set_hotspot_active_foreground(bool useSetting, const Colorf &colour)
{
	impl->set_hotspot_active_foreground(useSetting, impl->to_int_color(colour));
}

Colorf ScintillaEditor::get_hotspot_active_foreground()
{
	return impl->from_int_color(impl->get_hotspot_active_foreground());
}

void ScintillaEditor::set_hotspot_active_background(bool useSetting, const Colorf &colour)
{
	impl->set_hotspot_active_background(useSetting, impl->to_int_color(colour));
}

Colorf ScintillaEditor::get_hotspot_active_background()
{
	return impl->from_int_color(impl->get_hotspot_active_background());
}

void ScintillaEditor::set_hotspot_active_underline(bool underline)
{
	impl->set_hotspot_active_underline(underline);
}

bool ScintillaEditor::get_hotspot_active_underline()
{
	return impl->get_hotspot_active_underline();
}

void ScintillaEditor::set_hotspot_single_line(bool singleLine)
{
	impl->set_hotspot_single_line(singleLine);
}

bool ScintillaEditor::get_hotspot_single_line()
{
	return impl->get_hotspot_single_line();
}

void ScintillaEditor::set_control_char_symbol(int symbol)
{
	impl->set_control_char_symbol(symbol);
}

int ScintillaEditor::get_control_char_symbol()
{
	return impl->get_control_char_symbol();
}

void ScintillaEditor::set_caret_sticky(int useCaretStickyBehaviour)
{
	impl->set_caret_sticky(useCaretStickyBehaviour);
}

int ScintillaEditor::get_caret_sticky()
{
	return impl->get_caret_sticky();
}

void ScintillaEditor::toggle_caret_sticky()
{
	impl->toggle_caret_sticky();
}

void ScintillaEditor::set_margin_type(int margin, int type)
{
	impl->set_margin_type(margin, type);
}

int ScintillaEditor::get_margin_type(int margin)
{
	return impl->get_margin_type(margin);
}

void ScintillaEditor::set_margin_width(int margin, int pixelWidth)
{
	impl->set_margin_width(margin, pixelWidth);
}

int ScintillaEditor::get_margin_width(int margin)
{
	return impl->get_margin_width(margin);
}

void ScintillaEditor::set_margin_mask(int margin, int mask)
{
	impl->set_margin_mask(margin, mask);
}

int ScintillaEditor::get_margin_mask(int margin)
{
	return impl->get_margin_mask(margin);
}

void ScintillaEditor::set_margin_sensitive(int margin, bool sensitive)
{
	impl->set_margin_sensitive(margin, sensitive);
}

bool ScintillaEditor::get_margin_sensitive(int margin)
{
	return impl->get_margin_sensitive(margin);
}

void ScintillaEditor::set_margin_cursor(int margin, int cursor)
{
	impl->set_margin_cursor(margin, cursor);
}

int ScintillaEditor::get_margin_cursor(int margin)
{
	return impl->get_margin_cursor(margin);
}

void ScintillaEditor::set_margin_left(int pixels)
{
	impl->set_margin_left(pixels);
}

int ScintillaEditor::get_margin_left()
{
	return impl->get_margin_left();
}

void ScintillaEditor::set_margin_right(int pixels)
{
	impl->set_margin_right(pixels);
}

int ScintillaEditor::get_margin_right()
{
	return impl->get_margin_right();
}

void ScintillaEditor::set_fold_margin_color(bool useSetting, const Colorf &colour)
{
	impl->set_fold_margin_color(useSetting, impl->to_int_color(colour));
}

void ScintillaEditor::set_fold_margin_hi_color(bool useSetting, const Colorf &colour)
{
	impl->set_fold_margin_hi_color(useSetting, impl->to_int_color(colour));
}

void ScintillaEditor::margin_set_text(int line, const std::string &text)
{
	impl->margin_set_text(line, (char*)text.c_str());
}

std::string ScintillaEditor::margin_get_text(int line)
{
	int length = impl->margin_get_text(line, 0) + 1;
	DataBuffer buffer(length);
	length = clamp(impl->margin_get_text(line, buffer.get_data()), 1, length) - 1;
	return std::string(buffer.get_data(), length);
}

void ScintillaEditor::margin_set_style(int line, int style)
{
	impl->margin_set_style(line, style);
}

int ScintillaEditor::margin_get_style(int line)
{
	return impl->margin_get_style(line);
}

void ScintillaEditor::margin_set_styles(int line, const std::string &styles)
{
	impl->margin_set_styles(line, (char*)styles.c_str());
}

std::string ScintillaEditor::margin_get_styles(int line)
{
	int length = impl->margin_get_styles(line, 0) + 1;
	DataBuffer buffer(length);
	length = clamp(impl->margin_get_styles(0, buffer.get_data()), 1, length) - 1;
	return std::string(buffer.get_data(), length);
}

void ScintillaEditor::margin_text_clear_all()
{
	impl->margin_text_clear_all();
}

void ScintillaEditor::margin_set_style_offset(int style)
{
	impl->margin_set_style_offset(style);
}

int ScintillaEditor::margin_get_style_offset()
{
	return impl->margin_get_style_offset();
}

void ScintillaEditor::set_margin_options(int marginOptions)
{
	impl->set_margin_options(marginOptions);
}

int ScintillaEditor::get_margin_options()
{
	return impl->get_margin_options();
}

void ScintillaEditor::annotation_set_text(int line, const std::string &text)
{
	impl->annotation_set_text(line, (char*)text.c_str());
}

std::string ScintillaEditor::annotation_get_text(int line)
{
	int length = impl->annotation_get_text(line, 0) + 1;
	DataBuffer buffer(length);
	length = clamp(impl->annotation_get_text(0, buffer.get_data()), 1, length) - 1;
	return std::string(buffer.get_data(), length);
}

void ScintillaEditor::annotation_set_style(int line, int style)
{
	impl->annotation_set_style(line, style);
}

int ScintillaEditor::annotation_get_style(int line)
{
	return impl->annotation_get_style(line);
}

void ScintillaEditor::annotation_set_styles(int line, const std::string &styles)
{
	impl->annotation_set_styles(line, (char*)styles.c_str());
}

std::string ScintillaEditor::annotation_get_styles(int line)
{
	int length = impl->annotation_get_styles(line, 0) + 1;
	DataBuffer buffer(length);
	length = clamp(impl->annotation_get_styles(line, buffer.get_data()), 1, length) - 1;
	return std::string(buffer.get_data(), length);
}

int ScintillaEditor::annotation_get_lines(int line)
{
	return impl->annotation_get_lines(line);
}

void ScintillaEditor::annotation_clear_all()
{
	impl->annotation_clear_all();
}

void ScintillaEditor::annotation_set_visible(int visible)
{
	impl->annotation_set_visible(visible);
}

int ScintillaEditor::annotation_get_visible()
{
	return impl->annotation_get_visible();
}

void ScintillaEditor::annotation_set_style_offset(int style)
{
	impl->annotation_set_style_offset(style);
}

int ScintillaEditor::annotation_get_style_offset()
{
	return impl->annotation_get_style_offset();
}

void ScintillaEditor::set_word_chars(const std::string &chars)
{
	impl->set_word_chars(chars.c_str());
}

void ScintillaEditor::set_whitespace_chars(const std::string &chars)
{
	impl->set_whitespace_chars(chars.c_str());
}

void ScintillaEditor::set_chars_default()
{
	impl->set_chars_default();
}

void ScintillaEditor::grab_focus()
{
	impl->grab_focus();
}

void ScintillaEditor::brace_highlight(int pos1, int pos2)
{
	impl->brace_highlight(pos1, pos2);
}

void ScintillaEditor::brace_badlight(int pos1)
{
	impl->brace_badlight(pos1);
}

void ScintillaEditor::brace_highlight_indicator(bool useBraceHighlightIndicator, int indicatorNumber)
{
	impl->brace_highlight_indicator(useBraceHighlightIndicator, indicatorNumber);
}

void ScintillaEditor::brace_badlight_indicator(bool useBraceBadLightIndicator, int indicatorNumber)
{
	impl->brace_badlight_indicator(useBraceBadLightIndicator, indicatorNumber);
}

void ScintillaEditor::brace_match(int position, int maxReStyle)
{
	impl->brace_match(position, maxReStyle);
}

void ScintillaEditor::set_tab_width(int widthInChars)
{
	impl->set_tab_width(widthInChars);
}

int ScintillaEditor::get_tab_width()
{
	return impl->get_tab_width();
}

void ScintillaEditor::set_use_tabs(bool useTabs)
{
	impl->set_use_tabs(useTabs);
}

bool ScintillaEditor::get_use_tabs()
{
	return impl->get_use_tabs();
}

void ScintillaEditor::set_indent(int widthInChars)
{
	impl->set_indent(widthInChars);
}

int ScintillaEditor::get_indent()
{
	return impl->get_indent();
}

void ScintillaEditor::set_tab_indents(bool tabIndents)
{
	impl->set_tab_indents(tabIndents);
}

bool ScintillaEditor::get_tab_indents()
{
	return impl->get_tab_indents();
}

void ScintillaEditor::set_backspace_unindents(bool bsUnIndents)
{
	impl->set_backspace_unindents(bsUnIndents);
}

bool ScintillaEditor::get_backspace_unindents()
{
	return impl->get_backspace_unindents();
}

void ScintillaEditor::set_line_indentation(int line, int indentation)
{
	impl->set_line_indentation(line, indentation);
}

int ScintillaEditor::get_line_indentation(int line)
{
	return impl->get_line_indentation(line);
}

int ScintillaEditor::get_line_indent_position(int line)
{
	return impl->get_line_indent_position(line);
}

void ScintillaEditor::set_indentation_guides(int indentView)
{
	impl->set_indentation_guides(indentView);
}

int ScintillaEditor::get_indentation_guides()
{
	return impl->get_indentation_guides();
}

void ScintillaEditor::set_highlight_guide(int column)
{
	impl->set_highlight_guide(column);
}

int ScintillaEditor::get_highlight_guide()
{
	return impl->get_highlight_guide();
}

void ScintillaEditor::marker_define(int markerNumber, int markerSymbols)
{
	impl->marker_define(markerNumber, markerSymbols);
}

void ScintillaEditor::marker_define_image(int markerNumber, const PixelBuffer &pixels)
{
	PixelBuffer rgba = pixels.to_format(tf_rgba8);
	impl->rgba_image_set_width(rgba.get_width());
	impl->rgba_image_set_height(rgba.get_height());
	impl->marker_define_rgba_image(markerNumber, (const char*)rgba.get_data());
}

void ScintillaEditor::marker_symbol_defined(int markerNumber)
{
	impl->marker_symbol_defined(markerNumber);
}

void ScintillaEditor::marker_set_foreground(int markerNumber, const Colorf &colour)
{
	impl->marker_set_foreground(markerNumber, impl->to_int_color(colour));
}

void ScintillaEditor::marker_set_background(int markerNumber, const Colorf &colour)
{
	impl->marker_set_background(markerNumber, impl->to_int_color(colour));
}

void ScintillaEditor::marker_set_background_selected(int markerNumber, const Colorf &colour)
{
	impl->marker_set_background_selected(markerNumber, impl->to_int_color(colour));
}

void ScintillaEditor::marker_enable_highlight(int enabled)
{
	impl->marker_enable_highlight(enabled);
}

void ScintillaEditor::marker_set_alpha(int markerNumber, int alpha)
{
	impl->marker_set_alpha(markerNumber, alpha);
}

void ScintillaEditor::marker_add(int line, int markerNumber)
{
	impl->marker_add(line, markerNumber);
}

void ScintillaEditor::marker_add_set(int line, int markerMask)
{
	impl->marker_add_set(line, markerMask);
}

void ScintillaEditor::marker_delete(int line, int markerNumber)
{
	impl->marker_delete(line, markerNumber);
}

void ScintillaEditor::marker_delete_all(int markerNumber)
{
	impl->marker_delete_all(markerNumber);
}

int ScintillaEditor::marker_get(int line)
{
	return impl->marker_get(line);
}

int ScintillaEditor::marker_next(int lineStart, int markerMask)
{
	return impl->marker_next(lineStart, markerMask);
}

int ScintillaEditor::marker_previous(int lineStart, int markerMask)
{
	return impl->marker_previous(lineStart, markerMask);
}

int ScintillaEditor::marker_line_from_handle(int handle)
{
	return impl->marker_line_from_handle(handle);
}

int ScintillaEditor::marker_delete_handle(int handle)
{
	return impl->marker_delete_handle(handle);
}

void ScintillaEditor::indic_set_style(int indicatorNumber, int indicatorStyle)
{
	impl->indic_set_style(indicatorNumber, indicatorStyle);
}

int ScintillaEditor::indic_get_style(int indicatorNumber)
{
	return impl->indic_get_style(indicatorNumber);
}

void ScintillaEditor::indic_set_foreground(int indicatorNumber, const Colorf &colour)
{
	impl->indic_set_foreground(indicatorNumber, impl->to_int_color(colour));
}

Colorf ScintillaEditor::indic_get_foreground(int indicatorNumber)
{
	return impl->from_int_color(impl->indic_get_foreground(indicatorNumber));
}

void ScintillaEditor::indic_set_alpha(int indicatorNumber, int alpha)
{
	impl->indic_set_alpha(indicatorNumber, alpha);
}

int ScintillaEditor::indic_get_alpha(int indicatorNumber)
{
	return impl->indic_get_alpha(indicatorNumber);
}

void ScintillaEditor::indic_set_outline_alpha(int indicatorNumber, int alpha)
{
	impl->indic_set_outline_alpha(indicatorNumber, alpha);
}

int ScintillaEditor::indic_get_outline_alpha(int indicatorNumber)
{
	return impl->indic_get_outline_alpha(indicatorNumber);
}

void ScintillaEditor::indic_set_underline(int indicatorNumber, bool under)
{
	impl->indic_set_underline(indicatorNumber, under);
}

bool ScintillaEditor::indic_get_underline(int indicatorNumber)
{
	return impl->indic_get_underline(indicatorNumber);
}

void ScintillaEditor::autocomplete_show(int lenEntered, const std::string &list)
{
	impl->autocomplete_show(lenEntered, list.c_str());
}

void ScintillaEditor::autocomplete_cancel()
{
	impl->autocomplete_cancel();
}

void ScintillaEditor::autocomplete_active()
{
	impl->autocomplete_active();
}

int ScintillaEditor::autocomplete_pos_start()
{
	return impl->autocomplete_pos_start();
}

void ScintillaEditor::autocomplete_complete()
{
	impl->autocomplete_complete();
}

void ScintillaEditor::autocomplete_stops(const std::string &chars)
{
	impl->autocomplete_stops(chars.c_str());
}

void ScintillaEditor::autocomplete_set_separator(char separator)
{
	impl->autocomplete_set_separator(separator);
}

int ScintillaEditor::autocomplete_get_seperator()
{
	return impl->autocomplete_get_seperator();
}

void ScintillaEditor::autocomplete_select(const std::string &select)
{
	impl->autocomplete_select(select.c_str());
}

int ScintillaEditor::autocomplete_get_current()
{
	return impl->autocomplete_get_current();
}

std::string ScintillaEditor::autocomplete_get_current_text()
{
	int length = impl->autocomplete_get_current_text(0) + 1;
	DataBuffer buffer(length);
	length = clamp(impl->autocomplete_get_current_text(buffer.get_data()), 1, length) - 1;
	return std::string(buffer.get_data(), length);
}

void ScintillaEditor::autocomplete_set_cancel_at_start(bool cancel)
{
	impl->autocomplete_set_cancel_at_start(cancel);
}

bool ScintillaEditor::autocomplete_get_cancel_at_start()
{
	return impl->autocomplete_get_cancel_at_start();
}

void ScintillaEditor::autocomplete_set_fill_ups(const std::string &chars)
{
	impl->autocomplete_set_fill_ups(chars.c_str());
}

void ScintillaEditor::autocomplete_set_choose_single(bool chooseSingle)
{
	impl->autocomplete_set_choose_single(chooseSingle);
}

bool ScintillaEditor::autocomplete_get_choose_single()
{
	return impl->autocomplete_get_choose_single();
}

void ScintillaEditor::autocomplete_set_ignore_case(bool ignoreCase)
{
	impl->autocomplete_set_ignore_case(ignoreCase);
}

bool ScintillaEditor::autocomplete_get_ignore_case()
{
	return impl->autocomplete_get_ignore_case();
}

void ScintillaEditor::autocomplete_set_auto_hide(bool autoHide)
{
	impl->autocomplete_set_auto_hide(autoHide);
}

bool ScintillaEditor::autocomplete_get_auto_hide()
{
	return impl->autocomplete_get_auto_hide();
}

void ScintillaEditor::autocomplete_set_drop_rest_of_word(bool dropRestOfWord)
{
	impl->autocomplete_set_drop_rest_of_word(dropRestOfWord);
}

bool ScintillaEditor::autocomplete_get_drop_rest_of_word()
{
	return impl->autocomplete_get_drop_rest_of_word();
}

void ScintillaEditor::autocomplete_register_image(int type, const PixelBuffer &pixels)
{
	PixelBuffer rgba = pixels.to_format(tf_rgba8);
	impl->rgba_image_set_width(rgba.get_width());
	impl->rgba_image_set_height(rgba.get_height());
	impl->register_rgba_image(type, (const char*)rgba.get_data());
}

int ScintillaEditor::autocomplete_clear_registered_images()
{
	return impl->clear_registered_images();
}

void ScintillaEditor::autocomplete_set_type_separator(char separatorCharacter)
{
	impl->autocomplete_set_type_separator(separatorCharacter);
}

int ScintillaEditor::autocomplete_get_type_separator()
{
	return impl->autocomplete_get_type_separator();
}

void ScintillaEditor::autocomplete_set_max_height(int rowCount)
{
	impl->autocomplete_set_max_height(rowCount);
}

int ScintillaEditor::autocomplete_get_max_height()
{
	return impl->autocomplete_get_max_height();
}

void ScintillaEditor::autocomplete_set_max_width(int characterCount)
{
	impl->autocomplete_set_max_width(characterCount);
}

int ScintillaEditor::autocomplete_get_max_width()
{
	return impl->autocomplete_get_max_width();
}

void ScintillaEditor::calltip_show(int posStart, const std::string &definition)
{
	impl->calltip_show(posStart, definition.c_str());
}

void ScintillaEditor::calltip_cancel()
{
	impl->calltip_cancel();
}

void ScintillaEditor::calltip_active()
{
	impl->calltip_active();
}

int ScintillaEditor::calltip_posstart()
{
	return impl->calltip_posstart();
}

void ScintillaEditor::calltip_set_highlight(int highlightStart, int highlightEnd)
{
	impl->calltip_set_highlight(highlightStart, highlightEnd);
}

void ScintillaEditor::calltip_set_backrgound(const Colorf &colour)
{
	impl->calltip_set_backrgound(impl->to_int_color(colour));
}

void ScintillaEditor::calltip_set_foreground(const Colorf &colour)
{
	impl->calltip_set_foreground(impl->to_int_color(colour));
}

void ScintillaEditor::calltip_set_foreground_highlight(const Colorf &colour)
{
	impl->calltip_set_foreground_highlight(impl->to_int_color(colour));
}

void ScintillaEditor::calltip_use_style(int tabsize)
{
	impl->calltip_use_style(tabsize);
}

void ScintillaEditor::line_down()
{
	impl->line_down();
}

void ScintillaEditor::line_down_extend()
{
	impl->line_down_extend();
}

void ScintillaEditor::line_down_rect_extend()
{
	impl->line_down_rect_extend();
}

void ScintillaEditor::line_scroll_down()
{
	impl->line_scroll_down();
}

void ScintillaEditor::line_up()
{
	impl->line_up();
}

void ScintillaEditor::line_up_extend()
{
	impl->line_up_extend();
}

void ScintillaEditor::line_up_rect_extend()
{
	impl->line_up_rect_extend();
}

void ScintillaEditor::line_scroll_up()
{
	impl->line_scroll_up();
}

void ScintillaEditor::paragraph_down()
{
	impl->paragraph_down();
}

void ScintillaEditor::paragraph_down_extend()
{
	impl->paragraph_down_extend();
}

void ScintillaEditor::paragraph_up()
{
	impl->paragraph_up();
}

void ScintillaEditor::paragraph_up_extend()
{
	impl->paragraph_up_extend();
}

void ScintillaEditor::char_left()
{
	impl->char_left();
}

void ScintillaEditor::char_left_extend()
{
	impl->char_left_extend();
}

void ScintillaEditor::char_left_rect_extend()
{
	impl->char_left_rect_extend();
}

void ScintillaEditor::char_right()
{
	impl->char_right();
}

void ScintillaEditor::char_right_extend()
{
	impl->char_right_extend();
}

void ScintillaEditor::char_right_rect_extend()
{
	impl->char_right_rect_extend();
}

void ScintillaEditor::word_left()
{
	impl->word_left();
}

void ScintillaEditor::word_left_extend()
{
	impl->word_left_extend();
}

void ScintillaEditor::word_right()
{
	impl->word_right();
}

void ScintillaEditor::word_right_extend()
{
	impl->word_right_extend();
}

void ScintillaEditor::word_left_end()
{
	impl->word_left_end();
}

void ScintillaEditor::word_left_end_extend()
{
	impl->word_left_end_extend();
}

void ScintillaEditor::word_right_end()
{
	impl->word_right_end();
}

void ScintillaEditor::word_right_end_extend()
{
	impl->word_right_end_extend();
}

void ScintillaEditor::word_part_left()
{
	impl->word_part_left();
}

void ScintillaEditor::word_part_left_extend()
{
	impl->word_part_left_extend();
}

void ScintillaEditor::word_part_right()
{
	impl->word_part_right();
}

void ScintillaEditor::word_part_right_extend()
{
	impl->word_part_right_extend();
}

void ScintillaEditor::home()
{
	impl->home();
}

void ScintillaEditor::home_extend()
{
	impl->home_extend();
}

void ScintillaEditor::home_rect_extend()
{
	impl->home_rect_extend();
}

void ScintillaEditor::home_display()
{
	impl->home_display();
}

void ScintillaEditor::home_display_extend()
{
	impl->home_display_extend();
}

void ScintillaEditor::home_wrap()
{
	impl->home_wrap();
}

void ScintillaEditor::home_wrap_extend()
{
	impl->home_wrap_extend();
}

void ScintillaEditor::vc_home()
{
	impl->vc_home();
}

void ScintillaEditor::vc_home_extend()
{
	impl->vc_home_extend();
}

void ScintillaEditor::vc_home_rect_extend()
{
	impl->vc_home_rect_extend();
}

void ScintillaEditor::vc_home_wrap()
{
	impl->vc_home_wrap();
}

void ScintillaEditor::vc_home_wrap_extend()
{
	impl->vc_home_wrap_extend();
}

void ScintillaEditor::line_end()
{
	impl->line_end();
}

void ScintillaEditor::line_end_extend()
{
	impl->line_end_extend();
}

void ScintillaEditor::line_end_rect_extend()
{
	impl->line_end_rect_extend();
}

void ScintillaEditor::line_end_display()
{
	impl->line_end_display();
}

void ScintillaEditor::line_end_display_extend()
{
	impl->line_end_display_extend();
}

void ScintillaEditor::line_end_wrap()
{
	impl->line_end_wrap();
}

void ScintillaEditor::line_end_wrap_extend()
{
	impl->line_end_wrap_extend();
}

void ScintillaEditor::document_start()
{
	impl->document_start();
}

void ScintillaEditor::document_start_extend()
{
	impl->document_start_extend();
}

void ScintillaEditor::document_end()
{
	impl->document_end();
}

void ScintillaEditor::document_end_extend()
{
	impl->document_end_extend();
}

void ScintillaEditor::page_up()
{
	impl->page_up();
}

void ScintillaEditor::page_up_extend()
{
	impl->page_up_extend();
}

void ScintillaEditor::page_up_rect_extend()
{
	impl->page_up_rect_extend();
}

void ScintillaEditor::page_down()
{
	impl->page_down();
}

void ScintillaEditor::page_down_extend()
{
	impl->page_down_extend();
}

void ScintillaEditor::page_down_rect_extend()
{
	impl->page_down_rect_extend();
}

void ScintillaEditor::stuttered_page_up()
{
	impl->stuttered_page_up();
}

void ScintillaEditor::stuttered_page_up_extend()
{
	impl->stuttered_page_up_extend();
}

void ScintillaEditor::stuttered_page_down()
{
	impl->stuttered_page_down();
}

void ScintillaEditor::stuttered_page_down_extend()
{
	impl->stuttered_page_down_extend();
}

void ScintillaEditor::delete_back()
{
	impl->delete_back();
}

void ScintillaEditor::delete_back_not_line()
{
	impl->delete_back_not_line();
}

void ScintillaEditor::del_word_left()
{
	impl->del_word_left();
}

void ScintillaEditor::del_word_right()
{
	impl->del_word_right();
}

void ScintillaEditor::del_word_right_end()
{
	impl->del_word_right_end();
}

void ScintillaEditor::del_line_left()
{
	impl->del_line_left();
}

void ScintillaEditor::del_line_right()
{
	impl->del_line_right();
}

void ScintillaEditor::line_delete()
{
	impl->line_delete();
}

void ScintillaEditor::line_cut()
{
	impl->line_cut();
}

void ScintillaEditor::line_copy()
{
	impl->line_copy();
}

void ScintillaEditor::line_transpose()
{
	impl->line_transpose();
}

void ScintillaEditor::line_duplicate()
{
	impl->line_duplicate();
}

void ScintillaEditor::lowercase()
{
	impl->lowercase();
}

void ScintillaEditor::uppercase()
{
	impl->uppercase();
}

void ScintillaEditor::cancel()
{
	impl->cancel();
}

void ScintillaEditor::edit_toggle_overtype()
{
	impl->edit_toggle_overtype();
}

void ScintillaEditor::newline()
{
	impl->newline();
}

void ScintillaEditor::formfeed()
{
	impl->formfeed();
}

void ScintillaEditor::tab()
{
	impl->tab();
}

void ScintillaEditor::backtab()
{
	impl->backtab();
}

void ScintillaEditor::selection_duplicate()
{
	impl->selection_duplicate();
}

void ScintillaEditor::vertical_centre_caret()
{
	impl->vertical_centre_caret();
}

void ScintillaEditor::move_selected_lines_up()
{
	impl->move_selected_lines_up();
}

void ScintillaEditor::move_selected_lines_down()
{
	impl->move_selected_lines_down();
}

void ScintillaEditor::scroll_to_start()
{
	impl->scroll_to_start();
}

void ScintillaEditor::scroll_to_end()
{
	impl->scroll_to_end();
}

void ScintillaEditor::assign_cmd_key(int keyDefinition, int sciCommand)
{
	impl->assign_cmd_key(keyDefinition, sciCommand);
}

void ScintillaEditor::clear_cmd_key(int keyDefinition)
{
	impl->clear_cmd_key(keyDefinition);
}

void ScintillaEditor::clear_all_cmd_keys()
{
	impl->clear_all_cmd_keys();
}

void ScintillaEditor::use_popup(bool bEnablePopup)
{
	impl->use_popup(bEnablePopup);
}

void ScintillaEditor::start_record()
{
	impl->start_record();
}

void ScintillaEditor::stop_record()
{
	impl->stop_record();
}

void ScintillaEditor::play_record(const std::string &str)
{
	impl->play_record(str);
}

void ScintillaEditor::format_range(bool bDraw, Scintilla::Sci_RangeToFormat *pfr)
{
	impl->format_range(bDraw, pfr);
}

void ScintillaEditor::set_print_magnification(int magnification)
{
	impl->set_print_magnification(magnification);
}

int ScintillaEditor::get_print_magnification()
{
	return impl->get_print_magnification();
}

void ScintillaEditor::set_print_colou_mode(int mode)
{
	impl->set_print_colou_mode(mode);
}

int ScintillaEditor::get_print_color_mode()
{
	return impl->get_print_color_mode();
}

void ScintillaEditor::set_print_wrap_mode()
{
	impl->set_print_wrap_mode();
}

int ScintillaEditor::get_print_wrap_mode()
{
	return impl->get_print_wrap_mode();
}

Scintilla::Document *ScintillaEditor::get_doc_pointer()
{
	return impl->get_doc_pointer();
}

void ScintillaEditor::set_doc_pointer(Scintilla::Document *pDoc)
{
	impl->set_doc_pointer(pDoc);
}

Scintilla::Document *ScintillaEditor::create_document()
{
	return impl->create_document();
}

void ScintillaEditor::add_ref_document(Scintilla::Document *pDoc)
{
	impl->add_ref_document(pDoc);
}

void ScintillaEditor::release_document(Scintilla::Document *pDoc)
{
	impl->release_document(pDoc);
}

int ScintillaEditor::visible_from_doc_line(int docLine)
{
	return impl->visible_from_doc_line(docLine);
}

int ScintillaEditor::doc_line_from_visible(int displayLine)
{
	return impl->doc_line_from_visible(displayLine);
}

void ScintillaEditor::show_lines(int lineStart, int lineEnd)
{
	impl->show_lines(lineStart, lineEnd);
}

void ScintillaEditor::hide_lines(int lineStart, int lineEnd)
{
	impl->hide_lines(lineStart, lineEnd);
}

int ScintillaEditor::get_line_visible(int line)
{
	return impl->get_line_visible(line);
}

void ScintillaEditor::set_fold_level(int line, int level)
{
	impl->set_fold_level(line, level);
}

int ScintillaEditor::get_fold_level(int line)
{
	return impl->get_fold_level(line);
}

void ScintillaEditor::set_fold_flags(int flags)
{
	impl->set_fold_flags(flags);
}

int ScintillaEditor::get_last_child(int line, int level)
{
	return impl->get_last_child(line, level);
}

int ScintillaEditor::get_fold_parent(int line)
{
	return impl->get_fold_parent(line);
}

void ScintillaEditor::set_fold_expanded(int line, bool expanded)
{
	impl->set_fold_expanded(line, expanded);
}

bool ScintillaEditor::get_fold_expanded(int line)
{
	return impl->get_fold_expanded(line);
}

void ScintillaEditor::contracted_fold_next(int lineStart)
{
	impl->contracted_fold_next(lineStart);
}

void ScintillaEditor::toggle_fold(int line)
{
	impl->toggle_fold(line);
}

void ScintillaEditor::ensure_visible(int line)
{
	impl->ensure_visible(line);
}

void ScintillaEditor::ensure_visible_enforce_policy(int line)
{
	impl->ensure_visible_enforce_policy(line);
}

void ScintillaEditor::set_wrap_mode(int wrapMode)
{
	impl->set_wrap_mode(wrapMode);
}

int ScintillaEditor::get_wrap_mode()
{
	return impl->get_wrap_mode();
}

void ScintillaEditor::set_wrap_visual_flags(int wrapVisualFlags)
{
	impl->set_wrap_visual_flags(wrapVisualFlags);
}

int ScintillaEditor::get_wrap_visual_flags()
{
	return impl->get_wrap_visual_flags();
}

void ScintillaEditor::set_wrap_visual_flags_location(int wrapVisualFlagsLocation)
{
	impl->set_wrap_visual_flags_location(wrapVisualFlagsLocation);
}

int ScintillaEditor::get_wrap_visual_flags_location()
{
	return impl->get_wrap_visual_flags_location();
}

void ScintillaEditor::set_wrap_indent_mode(int indentMode)
{
	impl->set_wrap_indent_mode(indentMode);
}

int ScintillaEditor::get_wrap_indent_mode()
{
	return impl->get_wrap_indent_mode();
}

void ScintillaEditor::set_wrap_start_indent(int indent)
{
	impl->set_wrap_start_indent(indent);
}

int ScintillaEditor::get_wrap_start_indent()
{
	return impl->get_wrap_start_indent();
}

void ScintillaEditor::set_layout_cache(int cacheMode)
{
	impl->set_layout_cache(cacheMode);
}

int ScintillaEditor::get_layout_cache()
{
	return impl->get_layout_cache();
}

void ScintillaEditor::set_position_cache(int size)
{
	impl->set_position_cache(size);
}

int ScintillaEditor::get_position_cache()
{
	return impl->get_position_cache();
}

void ScintillaEditor::line_split(int pixelWidth)
{
	impl->line_split(pixelWidth);
}

void ScintillaEditor::lines_join()
{
	impl->lines_join();
}

int ScintillaEditor::wrap_count(int docLine)
{
	return impl->wrap_count(docLine);
}

void ScintillaEditor::zoom_in()
{
	impl->zoom_in();
}

void ScintillaEditor::zoom_out()
{
	impl->zoom_out();
}

void ScintillaEditor::set_zoom(int zoomInPoints)
{
	impl->set_zoom(zoomInPoints);
}

int ScintillaEditor::get_zoom()
{
	return impl->get_zoom();
}

void ScintillaEditor::set_edge_mode(int mode)
{
	impl->set_edge_mode(mode);
}

int ScintillaEditor::get_edge_mode()
{
	return impl->get_edge_mode();
}

void ScintillaEditor::set_edge_column(int column)
{
	impl->set_edge_column(column);
}

int ScintillaEditor::get_edge_column()
{
	return impl->get_edge_column();
}

void ScintillaEditor::set_edge_color(const Colorf &colour)
{
	impl->set_edge_color(impl->to_int_color(colour));
}

Colorf ScintillaEditor::get_edge_color()
{
	return impl->from_int_color(impl->get_edge_color());
}

void ScintillaEditor::set_lexer(int lexer)
{
	impl->set_lexer(lexer);
}

int ScintillaEditor::get_lexer()
{
	return impl->get_lexer();
}

void ScintillaEditor::set_lexer_language(const std::string &name)
{
	impl->set_lexer_language(name.c_str());
}

std::string ScintillaEditor::get_lexer_language()
{
	int length = impl->get_lexer_language(0) + 1;
	DataBuffer buffer(length);
	length = clamp(impl->get_lexer_language(buffer.get_data()), 1, length) - 1;
	return std::string(buffer.get_data(), length);
}

void ScintillaEditor::load_lexer_library(const std::string &path)
{
	impl->load_lexer_library(path.c_str());
}

int ScintillaEditor::colorize(int start, int end)
{
	return impl->colorize(start, end);
}

int ScintillaEditor::change_lexer_state(int start, int end)
{
	return impl->change_lexer_state(start, end);
}

std::vector<std::string> ScintillaEditor::property_names()
{
	int length = impl->property_names(0) + 1;
	DataBuffer buffer(length);
	length = clamp(impl->property_names(buffer.get_data()), 1, length) - 1;
	std::string names(buffer.get_data(), length);
	return StringHelp::split_text(names, "\n");
}

int ScintillaEditor::property_type(const std::string &name)
{
	return impl->property_type(name.c_str());
}

std::string ScintillaEditor::describe_property(const std::string &name)
{
	int length = impl->describe_property(name.c_str(), 0) + 1;
	DataBuffer buffer(length);
	length = clamp(impl->describe_property(name.c_str(), buffer.get_data()), 1, length) - 1;
	return std::string(buffer.get_data(), length);
}

void ScintillaEditor::set_property(const std::string &key, const std::string &value)
{
	impl->set_property(key.c_str(), value.c_str());
}

std::string ScintillaEditor::get_property(const std::string &key)
{
	int length = impl->get_property(key.c_str(), 0) + 1;
	DataBuffer buffer(length);
	length = clamp(impl->get_property(key.c_str(), buffer.get_data()), 1, length) - 1;
	return std::string(buffer.get_data(), length);
}

std::string ScintillaEditor::get_property_expanded(const std::string &key)
{
	int length = impl->get_property_expanded(key.c_str(), 0) + 1;
	DataBuffer buffer(length);
	length = clamp(impl->get_property_expanded(key.c_str(), buffer.get_data()), 1, length) - 1;
	return std::string(buffer.get_data(), length);
}

int ScintillaEditor::get_property_int(const std::string &key, int default_)
{
	return impl->get_property_int(key.c_str(), default_);
}

std::vector<std::string> ScintillaEditor::describe_keyword_sets()
{
	int length = impl->describe_keyword_sets(0) + 1;
	DataBuffer buffer(length);
	length = clamp(impl->describe_keyword_sets(buffer.get_data()), 1, length) - 1;
	std::string sets(buffer.get_data(), length);
	return StringHelp::split_text(sets, "\n");
}

void ScintillaEditor::set_keywords(int keyWordSet, const std::string &keyWordList)
{
	impl->set_keywords(keyWordSet, keyWordList.c_str());
}

int ScintillaEditor::get_style_bits_needed()
{
	return impl->get_style_bits_needed();
}

void ScintillaEditor::set_mod_event_mask(int eventMask)
{
	impl->set_mod_event_mask(eventMask);
}

int ScintillaEditor::get_mod_event_mask()
{
	return impl->get_mod_event_mask();
}

void ScintillaEditor::set_mouse_dwell_time(int milliseconds)
{
	impl->set_mouse_dwell_time(milliseconds);
}

int ScintillaEditor::get_mouse_dwell_time()
{
	return impl->get_mouse_dwell_time();
}

void ScintillaEditor::set_identifier(int identifier)
{
	impl->set_identifier(identifier);
}

int ScintillaEditor::get_identifier()
{
	return impl->get_identifier();
}

Callback_v0 &ScintillaEditor::func_style_needed()
{
	return impl->cb_style_needed;
}

Callback_v0 &ScintillaEditor::func_char_added()
{
	return impl->cb_char_added;
}

Callback_v0 &ScintillaEditor::func_save_point_reached()
{
	return impl->cb_save_point_reached;
}

Callback_v0 &ScintillaEditor::func_save_point_left()
{
	return impl->cb_save_point_left;
}

Callback_v0 &ScintillaEditor::func_modify_attempt_read_only()
{
	return impl->cb_modify_attempt_read_only;
}

Callback_v0 &ScintillaEditor::func_key()
{
	return impl->cb_key;
}

Callback_v0 &ScintillaEditor::func_double_click()
{
	return impl->cb_double_click;
}

Callback_v0 &ScintillaEditor::func_update_ui()
{
	return impl->cb_update_ui;
}

Callback_v0 &ScintillaEditor::func_modified()
{
	return impl->cb_modified;
}

Callback_v1<const std::string &> &ScintillaEditor::func_macro_record()
{
	return impl->cb_macro_record;
}

Callback_v5<bool, bool, bool, int, int> &ScintillaEditor::func_margin_click()
{
	return impl->cb_margin_click;
}

Callback_v2<int, int> &ScintillaEditor::func_need_shown()
{
	return impl->cb_need_shown;
}

Callback_v0 &ScintillaEditor::func_painted()
{
	return impl->cb_painted;
}

Callback_v0 &ScintillaEditor::func_userlist_selection()
{
	return impl->cb_userlist_selection;
}

Callback_v2<int, Point> &ScintillaEditor::func_dwell_start()
{
	return impl->cb_dwell_start;
}

Callback_v2<int, Point> &ScintillaEditor::func_dwell_end()
{
	return impl->cb_dwell_end;
}

Callback_v0 &ScintillaEditor::func_zoom()
{
	return impl->cb_zoom;
}

Callback_v0 &ScintillaEditor::func_hotspot_click()
{
	return impl->cb_hotspot_click;
}

Callback_v0 &ScintillaEditor::func_hotspot_double_click()
{
	return impl->cb_hotspot_double_click;
}

Callback_v0 &ScintillaEditor::func_hotspot_release_click()
{
	return impl->cb_hotspot_release_click;
}

Callback_v0 &ScintillaEditor::func_indicator_click()
{
	return impl->cb_indicator_click;
}

Callback_v0 &ScintillaEditor::func_indicator_release()
{
	return impl->cb_indicator_release;
}

Callback_v0 &ScintillaEditor::func_calltip_click()
{
	return impl->cb_calltip_click;
}

Callback_v0 &ScintillaEditor::func_autocomplete_selection()
{
	return impl->cb_autocomplete_selection;
}

Callback_v0 &ScintillaEditor::func_autocomplete_cancelled()
{
	return impl->cb_autocomplete_cancelled;
}

Callback_v0 &ScintillaEditor::func_autocomplete_char_deleted()
{
	return impl->cb_autocomplete_char_deleted;
}

Callback_v0 &ScintillaEditor::func_change()
{
	return impl->cb_change;
}

}
