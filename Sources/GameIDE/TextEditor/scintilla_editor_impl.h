
#pragma once

#include "Platform.h"
#include "ILexer.h"
#include "Scintilla.h"

#include "Scintilla/src/SplitVector.h"
#include "Scintilla/src/Partitioning.h"
#include "Scintilla/src/RunStyles.h"
#include "Scintilla/src/ContractionState.h"
#include "Scintilla/src/CellBuffer.h"
#include "Scintilla/src/CallTip.h"
#include "Scintilla/src/KeyMap.h"
#include "Scintilla/src/Indicator.h"
#include "Scintilla/src/XPM.h"
#include "Scintilla/src/LineMarker.h"
#include "Scintilla/src/Style.h"
#include "Scintilla/src/AutoComplete.h"
#include "Scintilla/src/ViewStyle.h"
#include "Scintilla/src/CharClassify.h"
#include "Scintilla/src/Decoration.h"
#include "Scintilla/src/Document.h"
#include "Scintilla/src/Selection.h"
#include "Scintilla/src/PositionCache.h"
#include "Scintilla/src/Editor.h"
#include "Scintilla/src/ScintillaBase.h"
#include "Scintilla/src/UniConversion.h"

class ScintillaEditor;

class ScintillaWindowHandle
{
public:
	virtual ~ScintillaWindowHandle() { }
	virtual void Window_Destroy() { }
	virtual bool Window_HasFocus() { return false; }
	virtual Scintilla::PRectangle Window_GetPosition() { return Scintilla::PRectangle(); }
	virtual void Window_SetPosition(Scintilla::PRectangle rc) { }
	virtual void Window_SetPositionRelative(Scintilla::PRectangle rc, ScintillaWindowHandle *relativeTo) { }
	virtual Scintilla::PRectangle Window_GetClientPosition() { return Scintilla::PRectangle(); }
	virtual void Window_Show(bool show) { }
	virtual void Window_InvalidateAll() { }
	virtual void Window_InvalidateRectangle(Scintilla::PRectangle rc) { }
	virtual void Window_SetFont(Scintilla::Font &font) { }
	virtual void Window_SetCursor(Scintilla::Window::Cursor curs) { }
	virtual void Window_SetTitle(const char *s) { }
	virtual Scintilla::PRectangle Window_GetMonitorRect(Scintilla::Point pt) { return Scintilla::PRectangle(); }

	virtual clan::Canvas sci_get_canvas() = 0;
	virtual void sci_set_cliprect(clan::Canvas &canvas, clan::Rect box) = 0;
};

class ScintillaEditor_Impl : public ScintillaWindowHandle, Scintilla::ScintillaBase
{
public:
	ScintillaEditor_Impl(ScintillaEditor *component);
	~ScintillaEditor_Impl();

	static int to_int_color(const clan::Colorf &c);
	static clan::Colorf from_int_color(int color);

	int get_text(int length, char *text) { return send_message(SCI_GETTEXT, length, (sptr_t)text); }
	void set_text(const char *text) { send_message(SCI_SETTEXT, 0, (sptr_t)text); }
	void set_savepoint() { send_message(SCI_SETSAVEPOINT, 0, 0); }
	int get_line(int line, char *text) { return send_message(SCI_GETLINE, line, (sptr_t)text); }
	void replace_sel(const char *text) { send_message(SCI_REPLACESEL, 0, (sptr_t)text); }
	void set_readonly(bool readOnly) { send_message(SCI_SETREADONLY, readOnly ? 1 : 0, 0); }
	bool get_readonly() { return send_message(SCI_GETREADONLY, 0, 0) != 0; }
	int get_text_range(Scintilla::Sci_TextRange *tr) { return send_message(SCI_GETTEXTRANGE, 0, (sptr_t)tr); }
	int get_styled_text(Scintilla::Sci_TextRange *tr) { return send_message(SCI_GETSTYLEDTEXT, 0, (sptr_t)tr); }
	void allocate(int bytes) { send_message(SCI_ALLOCATE, bytes, 0); }
	void add_text(int length, const char *s) { send_message(SCI_ADDTEXT, length, (sptr_t)s); }
	int add_styled_text(int length, Scintilla::CellBuffer *s) { return send_message(SCI_ADDSTYLEDTEXT, length, (sptr_t)s); }
	int append_text(int length, const char *s) { return send_message(SCI_APPENDTEXT, length, (sptr_t)s); }
	int insert_text(int pos, const char *text) { return send_message(SCI_INSERTTEXT, pos, (sptr_t)text); }
	void clear_all() { send_message(SCI_CLEARALL, 0, 0); }
	void clear_document_style() { send_message(SCI_CLEARDOCUMENTSTYLE, 0, 0); }
	int get_char_at(int pos) { return send_message(SCI_GETCHARAT, pos, 0); }
	int get_style_at(int pos) { return send_message(SCI_GETSTYLEAT, pos, 0); }
	void set_style_bits(int bits) { send_message(SCI_SETSTYLEBITS, bits, 0); }
	int get_style_bits() { return send_message(SCI_GETSTYLEBITS, 0, 0); }
	int find_text(int searchFlags, Scintilla::Sci_TextToFind *ttf) { return send_message(SCI_FINDTEXT, searchFlags, (sptr_t)ttf); }
	int search_anchor() { return send_message(SCI_SEARCHANCHOR, 0, 0); }
	int search_next(int searchFlags, const char *text) { return send_message(SCI_SEARCHNEXT, searchFlags, (sptr_t)text); }
	int search_prev(int searchFlags, const char *text) { return send_message(SCI_SEARCHPREV, searchFlags, (sptr_t)text); }
	void set_target_start(int pos) { send_message(SCI_SETTARGETSTART, pos, 0); }
	int get_target_start() { return send_message(SCI_GETTARGETSTART, 0, 0); }
	void set_target_end(int pos) { send_message(SCI_SETTARGETEND, pos, 0); }
	int get_target_end() { return send_message(SCI_GETTARGETEND, 0, 0); }
	void target_from_selection() { send_message(SCI_TARGETFROMSELECTION, 0, 0); }
	int set_search_flags(int searchFlags) { return send_message(SCI_SETSEARCHFLAGS, searchFlags, 0); }
	int get_search_flags() { return send_message(SCI_GETSEARCHFLAGS, 0, 0); }
	int search_in_target(int length, const char *text) { return send_message(SCI_SEARCHINTARGET, length, (sptr_t)text); }
	int replace_target(int length, const char *text) { return send_message(SCI_REPLACETARGET, length, (sptr_t)text); }
	int replace_target_re(int length, const char *text) { return send_message(SCI_REPLACETARGETRE, length, (sptr_t)text); }
	int get_tag(int tagNumber, char *tagValue) { return send_message(SCI_GETTAG, tagNumber, (sptr_t)tagValue); }
	int set_overtype(bool overType) { return send_message(SCI_SETOVERTYPE, overType ? 1 : 0, 0); }
	bool get_overtype() { return send_message(SCI_GETOVERTYPE, 0, 0) != 0; }
	void cut() { send_message(SCI_CUT, 0, 0); }
	void copy() { send_message(SCI_COPY, 0, 0); }
	void paste() { send_message(SCI_PASTE, 0, 0); }
	void clear() { send_message(SCI_CLEAR, 0, 0); }
	bool can_paste() { return send_message(SCI_CANPASTE, 0, 0) != 0; }
	void copy_allow_line() { send_message(SCI_COPYALLOWLINE, 0, 0); }
	void copy_range(int start, int end) { send_message(SCI_COPYRANGE, start, end); }
	void copy_text(int length, const char *text) { send_message(SCI_COPYTEXT, length, (sptr_t)text); }
	void set_paste_convert_endings(bool convert) { send_message(SCI_SETPASTECONVERTENDINGS, convert ? 1 : 0, 0); }
	int get_paste_convert_endings() { return send_message(SCI_GETPASTECONVERTENDINGS, 0, 0); }
	void set_status(int status) { send_message(SCI_SETSTATUS, status, 0); }
	int get_status() { return send_message(SCI_GETSTATUS, 0, 0); }
	void undo() { send_message(SCI_UNDO, 0, 0); }
	bool can_undo() { return send_message(SCI_CANUNDO, 0, 0) != 0; }
	void redo() { send_message(SCI_REDO, 0, 0); }
	bool can_redo() { return send_message(SCI_CANREDO, 0, 0) != 0; }
	void empty_undo_buffer() { send_message(SCI_EMPTYUNDOBUFFER, 0, 0); }
	void set_undo_collection(bool collectUndo) { send_message(SCI_SETUNDOCOLLECTION, collectUndo ? 1 : 0, 0); }
	bool get_undo_collection() { return send_message(SCI_GETUNDOCOLLECTION, 0, 0) != 0; }
	void begin_undo_action() { send_message(SCI_BEGINUNDOACTION, 0, 0); }
	void end_undo_action() { send_message(SCI_ENDUNDOACTION, 0, 0); }
	void add_undo_action(int token, int flags) { send_message(SCI_ADDUNDOACTION, token, flags); }
	int get_text_length() { return send_message(SCI_GETTEXTLENGTH, 0, 0); }
	int get_length() { return send_message(SCI_GETLENGTH, 0, 0); }
	int get_line_count() { return send_message(SCI_GETLINECOUNT, 0, 0); }
	void set_first_visible_line(int lineDisplay) { send_message(SCI_SETFIRSTVISIBLELINE, lineDisplay, 0); }
	int get_first_visible_line() { return send_message(SCI_GETFIRSTVISIBLELINE, 0, 0); }
	int line_on_screen() { return send_message(SCI_LINESONSCREEN, 0, 0); }
	bool get_modify() { return send_message(SCI_GETMODIFY, 0, 0) != 0; }
	void set_sel(int anchorPos, int currentPos) { send_message(SCI_SETSEL, anchorPos, currentPos); }
	void goto_pos(int pos) { send_message(SCI_GOTOPOS, pos, 0); }
	void goto_line(int line) { send_message(SCI_GOTOLINE, line, 0); }
	void set_current_pos(int pos) { send_message(SCI_SETCURRENTPOS, pos, 0); }
	int get_current_pos() { return send_message(SCI_GETCURRENTPOS, 0, 0); }
	void set_anchor(int pos) { send_message(SCI_SETANCHOR, pos, 0); }
	int get_anchor() { return send_message(SCI_GETANCHOR, 0, 0); }
	void set_selection_start(int pos) { send_message(SCI_SETSELECTIONSTART, pos, 0); }
	void set_selection_end(int pos) { send_message(SCI_SETSELECTIONEND, pos, 0); }
	int get_selection_start() { return send_message(SCI_GETSELECTIONSTART, 0, 0); }
	int get_selection_end() { return send_message(SCI_GETSELECTIONEND, 0, 0); }
	void set_empty_selection(int pos) { send_message(SCI_SETEMPTYSELECTION, pos, 0); }
	void select_all() { send_message(SCI_SELECTALL, 0, 0); }
	int line_from_position(int pos) { return send_message(SCI_LINEFROMPOSITION, pos, 0); }
	int position_from_line(int line) { return send_message(SCI_POSITIONFROMLINE, line, 0); }
	int get_line_end_position(int line) { return send_message(SCI_GETLINEENDPOSITION, line, 0); }
	int line_length(int line) { return send_message(SCI_LINELENGTH, line, 0); }
	int get_selection_text(char *text) { return send_message(SCI_GETSELTEXT, 0, (sptr_t)text); }
	int get_current_line(int textLen, char *text) { return send_message(SCI_GETCURLINE, textLen, (sptr_t)text); }
	bool selection_is_rectangle() { return send_message(SCI_SELECTIONISRECTANGLE, 0, 0) != 0; }
	void set_selection_mode(int mode) { send_message(SCI_SETSELECTIONMODE, mode, 0); }
	int get_selection_mode() { return send_message(SCI_GETSELECTIONMODE, 0, 0); }
	int get_line_selection_start_position(int line) { return send_message(SCI_GETLINESELSTARTPOSITION, line, 0); }
	int get_line_selection_end_position(int line) { return send_message(SCI_GETLINESELENDPOSITION, line, 0); }
	void move_caret_inside_view() { send_message(SCI_MOVECARETINSIDEVIEW, 0, 0); }
	int word_end_position(int position, bool onlyWordCharacters) { return send_message(SCI_WORDENDPOSITION, position, onlyWordCharacters ? 1 : 0); }
	int word_start_position(int position, bool onlyWordCharacters) { return send_message(SCI_WORDSTARTPOSITION, position, onlyWordCharacters ? 1 : 0); }
	int position_before(int position) { return send_message(SCI_POSITIONBEFORE, position, 0); }
	int position_after(int position) { return send_message(SCI_POSITIONAFTER, position, 0); }
	int text_width(int styleNumber, const char *text) { return send_message(SCI_TEXTWIDTH, styleNumber, (sptr_t)text); }
	int text_height(int line) { return send_message(SCI_TEXTHEIGHT, line, 0); }
	int get_column(int pos) { return send_message(SCI_GETCOLUMN, pos, 0); }
	int find_column(int line, int column) { return send_message(SCI_FINDCOLUMN, line, column); }
	int position_from_point(int x, int y) { return send_message(SCI_POSITIONFROMPOINT, x, y); }
	int position_from_point_close(int x, int y) { return send_message(SCI_POSITIONFROMPOINTCLOSE, x, y); }
	int char_position_from_point(int x, int y) { return send_message(SCI_CHARPOSITIONFROMPOINT, x, y); }
	int char_position_from_point_close(int x, int y) { return send_message(SCI_CHARPOSITIONFROMPOINTCLOSE, x, y); }
	int point_x_from_position(int pos) { return send_message(SCI_POINTXFROMPOSITION, 0, pos); }
	int point_y_from_position(int pos) { return send_message(SCI_POINTYFROMPOSITION, 0, pos); }
	void hide_selection(bool hide) { send_message(SCI_HIDESELECTION, hide ? 1 : 0, 0); }
	void choose_caret_x() { send_message(SCI_CHOOSECARETX, 0, 0); }
	void set_multiple_selection(bool multipleSelection) { send_message(SCI_SETMULTIPLESELECTION, multipleSelection ? 1 : 0, 0); }
	bool get_multiple_selection() { return send_message(SCI_GETMULTIPLESELECTION, 0, 0) != 0; }
	void set_additional_selection_typing(bool additionalSelectionTyping) { send_message(SCI_SETADDITIONALSELECTIONTYPING, additionalSelectionTyping ? 1 : 0, 0); }
	bool get_additional_selection_typing() { return send_message(SCI_GETADDITIONALSELECTIONTYPING, 0, 0) != 0; }
	void set_multi_paste(int multiPaste) { send_message(SCI_SETMULTIPASTE, multiPaste, 0); }
	int get_multi_paste() { return send_message(SCI_GETMULTIPASTE, 0, 0); }
	void set_virtual_space_options(int virtualSpace) { send_message(SCI_SETVIRTUALSPACEOPTIONS, virtualSpace, 0); }
	int get_virtual_space_options() { return send_message(SCI_GETVIRTUALSPACEOPTIONS, 0, 0); }
	void set_rectangular_selection_modifier(int modifier) { send_message(SCI_SETRECTANGULARSELECTIONMODIFIER, modifier, 0); }
	int get_rectangular_selection_modifier() { return send_message(SCI_GETRECTANGULARSELECTIONMODIFIER, 0, 0); }
	int get_selections() { return send_message(SCI_GETSELECTIONS, 0, 0); }
	void clear_selections() { send_message(SCI_CLEARSELECTIONS, 0, 0); }
	void set_selection(int caret, int anchor) { send_message(SCI_SETSELECTION, caret, anchor); }
	void add_selection(int caret, int anchor) { send_message(SCI_ADDSELECTION, caret, anchor); }
	void set_main_selection(int selection) { send_message(SCI_SETMAINSELECTION, selection, 0); }
	int get_main_selection() { return send_message(SCI_GETMAINSELECTION, 0, 0); }
	void set_selection_caret(int selection, int pos) { send_message(SCI_SETSELECTIONNCARET, selection, pos); }
	int get_selection_caret(int selection) { return send_message(SCI_GETSELECTIONNCARET, selection, 0); }
	void set_selection_caret_virtual_space(int selection, int space) { send_message(SCI_SETSELECTIONNCARETVIRTUALSPACE, selection, space); }
	int get_selection_caret_virtual_space(int selection) { return send_message(SCI_GETSELECTIONNCARETVIRTUALSPACE, selection, 0); }
	void set_selection_anchor(int selection, int posAnchor) { send_message(SCI_SETSELECTIONNANCHOR, selection, posAnchor); }
	int get_selection_anchor(int selection) { return send_message(SCI_GETSELECTIONNANCHOR, selection, 0); }
	void set_selection_anchor_virtual_space(int selection, int space) { send_message(SCI_SETSELECTIONNANCHORVIRTUALSPACE, selection, space); }
	int get_selection_anchor_virtual_space(int selection) { return send_message(SCI_GETSELECTIONNANCHORVIRTUALSPACE, selection, 0); }
	void set_selection_start(int selection, int pos) { send_message(SCI_SETSELECTIONNSTART, selection, pos); }
	int get_selection_start(int selection) { return send_message(SCI_GETSELECTIONNSTART, selection, 0); }
	void set_selection_end(int selection, int pos) { send_message(SCI_SETSELECTIONNEND, selection, pos); }
	int get_selection_end(int selection) { return send_message(SCI_GETSELECTIONNEND, selection, 0); }
	void set_rectangular_selection_caret(int pos) { send_message(SCI_SETRECTANGULARSELECTIONCARET, pos, 0); }
	int get_rectangular_selection_caret() { return send_message(SCI_GETRECTANGULARSELECTIONCARET, 0, 0); }
	void set_rectangular_selection_caret_virtual_space(int space) { send_message(SCI_SETRECTANGULARSELECTIONCARETVIRTUALSPACE, space, 0); }
	int get_rectangular_selection_caret_virtual_space() { return send_message(SCI_GETRECTANGULARSELECTIONCARETVIRTUALSPACE, 0, 0); }
	void set_rectangular_selection_anchor(int posAnchor) { send_message(SCI_SETRECTANGULARSELECTIONANCHOR, posAnchor, 0); }
	int get_rectangular_selection_anchor() { return send_message(SCI_GETRECTANGULARSELECTIONANCHOR, 0, 0); }
	void set_rectangular_selection_anchor_virtual_space(int space) { send_message(SCI_SETRECTANGULARSELECTIONANCHORVIRTUALSPACE, space, 0); }
	int get_rectangular_selection_anchor_virtual_space() { return send_message(SCI_GETRECTANGULARSELECTIONANCHORVIRTUALSPACE, 0, 0); }
	void set_additional_sel_alpha(int alpha) { send_message(SCI_SETADDITIONALSELALPHA, alpha, 0); }
	int get_additional_sel_alpha() { return send_message(SCI_GETADDITIONALSELALPHA, 0, 0); }
	void set_additional_sel_foreground(int colour) { send_message(SCI_SETADDITIONALSELFORE, colour, 0); }
	void set_additional_sel_background(int colour) { send_message(SCI_SETADDITIONALSELBACK, colour, 0); }
	void set_additional_caret_foreground(int colour) { send_message(SCI_SETADDITIONALCARETFORE, colour, 0); }
	int get_additional_caret_foreground() { return send_message(SCI_GETADDITIONALCARETFORE, 0, 0); }
	void set_additional_carets_blink(bool additionalCaretsBlink) { send_message(SCI_SETADDITIONALCARETSBLINK, additionalCaretsBlink, 0); }
	bool get_additional_carets_blink() { return send_message(SCI_GETADDITIONALCARETSBLINK, 0, 0) != 0; }
	void set_additional_carets_visible(bool additionalCaretsVisible) { send_message(SCI_SETADDITIONALCARETSVISIBLE, additionalCaretsVisible ? 1 : 0, 0); }
	bool get_additional_carets_visible() { return send_message(SCI_GETADDITIONALCARETSVISIBLE, 0, 0) != 0; }
	void swap_main_anchor_caret() { send_message(SCI_SWAPMAINANCHORCARET, 0, 0); }
	void rotate_selection() { send_message(SCI_ROTATESELECTION, 0, 0); }
	void line_scroll(int column, int line) { send_message(SCI_LINESCROLL, column, line); }
	void scroll_caret() { send_message(SCI_SCROLLCARET, 0, 0); }
	void set_x_caret_policy(int caretPolicy, int caretSlop) { send_message(SCI_SETXCARETPOLICY, caretPolicy, caretSlop); }
	void set_y_caret_policy(int caretPolicy, int caretSlop) { send_message(SCI_SETYCARETPOLICY, caretPolicy, caretSlop); }
	void set_visible_policy(int caretPolicy, int caretSlop) { send_message(SCI_SETVISIBLEPOLICY, caretPolicy, caretSlop); }
	void set_horz_scrollbar(bool visible) { send_message(SCI_SETHSCROLLBAR, visible ? 1 : 0, 0); }
	bool get_horz_scrollbar() { return send_message(SCI_GETHSCROLLBAR, 0, 0) != 0; }
	void set_vert_scrollbar(bool visible) { send_message(SCI_SETVSCROLLBAR, visible ? 1 : 0, 0); }
	bool get_vert_scrollbar() { return send_message(SCI_GETVSCROLLBAR, 0, 0) != 0; }
	void set_x_offset(int xOffset) { send_message(SCI_SETXOFFSET, xOffset, 0); }
	int get_x_offset() { return send_message(SCI_GETXOFFSET, 0, 0); }
	void set_scroll_width(int pixelWidth) { send_message(SCI_SETSCROLLWIDTH, pixelWidth, 0); }
	int get_scroll_width() { return send_message(SCI_GETSCROLLWIDTH, 0, 0); }
	void set_scroll_width_tracking(bool tracking) { send_message(SCI_SETSCROLLWIDTHTRACKING, tracking ? 1 : 0, 0); }
	bool get_scroll_width_tracking() { return send_message(SCI_GETSCROLLWIDTHTRACKING, 0, 0) != 0; }
	void set_end_at_last_line(bool endAtLastLine) { send_message(SCI_SETENDATLASTLINE, endAtLastLine ? 1 : 0, 0); }
	bool get_end_at_last_line() { return send_message(SCI_GETENDATLASTLINE, 0, 0) != 0; }
	void set_view_ws(int wsMode) { send_message(SCI_SETVIEWWS, wsMode, 0); }
	int get_view_ws() { return send_message(SCI_GETVIEWWS, 0, 0); }
	void set_whitespace_foreground(bool useWhitespaceForeColour, int colour) { send_message(SCI_SETWHITESPACEFORE, useWhitespaceForeColour ? 1 : 0, colour); }
	void set_whitespace_background(bool useWhitespaceBackColour, int colour) { send_message(SCI_SETWHITESPACEBACK, useWhitespaceBackColour ? 1 : 0, colour); }
	void set_whitespace_size(int size) { send_message(SCI_SETWHITESPACESIZE, size, 0); }
	int get_whitespace_size() { return send_message(SCI_GETWHITESPACESIZE, 0, 0); }
	void set_extra_ascent(int extraAscent) { send_message(SCI_SETEXTRAASCENT, extraAscent, 0); }
	int get_extra_ascent() { return send_message(SCI_GETEXTRAASCENT, 0, 0); }
	void set_extra_descent(int extraDescent) { send_message(SCI_SETEXTRADESCENT, extraDescent, 0); }
	int get_extra_descent() { return send_message(SCI_GETEXTRADESCENT, 0, 0); }
	void set_cursor(int curType) { send_message(SCI_SETCURSOR, curType, 0); }
	int get_cursor() { return send_message(SCI_GETCURSOR, 0, 0); }
	void set_mouse_down_captures(bool captures) { send_message(SCI_SETMOUSEDOWNCAPTURES, captures ? 1 : 0, 0); }
	bool get_mouse_down_captures() { return send_message(SCI_GETMOUSEDOWNCAPTURES, 0, 0) != 0; }
	void set_eol_mode(int eolMode) { send_message(SCI_SETEOLMODE, eolMode, 0); }
	int get_eol_mode() { return send_message(SCI_GETEOLMODE, 0, 0); }
	void convert_eols(int eolMode) { send_message(SCI_CONVERTEOLS, eolMode, 0); }
	void set_view_eol(bool visible) { send_message(SCI_SETVIEWEOL, visible ? 1 : 0, 0); }
	bool get_view_eol() { return send_message(SCI_GETVIEWEOL, 0, 0) != 0; }
	int get_end_styled() { return send_message(SCI_GETENDSTYLED, 0, 0); }
	void start_styling(int pos, int mask) { send_message(SCI_STARTSTYLING, pos, mask); }
	void set_styling(int length, int style) { send_message(SCI_SETSTYLING, length, style); }
	void set_styling_ex(int length, const char *styles) { send_message(SCI_SETSTYLINGEX, length, (sptr_t)styles); }
	void set_line_state(int line, int value) { send_message(SCI_SETLINESTATE, line, value); }
	int get_line_state(int line) { return send_message(SCI_GETLINESTATE, line, 0); }
	int get_max_line_state() { return send_message(SCI_GETMAXLINESTATE, 0, 0); }
	void style_reset_default() { send_message(SCI_STYLERESETDEFAULT, 0, 0); }
	void style_clear_all() { send_message(SCI_STYLECLEARALL, 0, 0); }
	void style_set_font(int styleNumber, const char *fontName) { send_message(SCI_STYLESETFONT, styleNumber, (sptr_t)fontName); }
	int style_get_font(int styleNumber, char *fontName) { return send_message(SCI_STYLEGETFONT, styleNumber, (sptr_t)fontName); }
	void style_set_size(int styleNumber, int sizeInPoints) { send_message(SCI_STYLESETSIZE, styleNumber, sizeInPoints); }
	int style_get_size(int styleNumber) { return send_message(SCI_STYLEGETSIZE, styleNumber, 0); }
	void style_set_bold(int styleNumber, bool bold) { send_message(SCI_STYLESETBOLD, styleNumber, bold ? 1 : 0); }
	bool style_get_bold(int styleNumber) { return send_message(SCI_STYLEGETBOLD, styleNumber, 0) != 0; }
	void style_set_italic(int styleNumber, bool italic) { send_message(SCI_STYLESETITALIC, styleNumber, italic ? 1 : 0); }
	bool style_get_italic(int styleNumber) { return send_message(SCI_STYLEGETITALIC, styleNumber, 0) != 0; }
	void style_set_underline(int styleNumber, bool underline) { send_message(SCI_STYLESETUNDERLINE, styleNumber, underline ? 1 : 0); }
	bool style_get_underline(int styleNumber) { return send_message(SCI_STYLEGETUNDERLINE, styleNumber, 0) != 0; }
	void style_set_foreground(int styleNumber, int colour) { send_message(SCI_STYLESETFORE, styleNumber, colour); }
	int style_get_foreground(int styleNumber) { return send_message(SCI_STYLEGETFORE, styleNumber, 0); }
	void style_set_background(int styleNumber, int colour) { send_message(SCI_STYLESETBACK, styleNumber, colour); }
	int style_get_background(int styleNumber) { return send_message(SCI_STYLEGETBACK, styleNumber, 0); }
	void style_set_eol_filled(int styleNumber, bool eolFilled) { send_message(SCI_STYLESETEOLFILLED, styleNumber, eolFilled ? 1 : 0); }
	bool style_get_eol_filled(int styleNumber) { return send_message(SCI_STYLEGETEOLFILLED, styleNumber, 0) != 0; }
	void style_set_character_set(int styleNumber, int charSet) { send_message(SCI_STYLESETCHARACTERSET, styleNumber, charSet); }
	int style_get_character_set(int styleNumber) { return send_message(SCI_STYLEGETCHARACTERSET, styleNumber, 0); }
	void style_set_case(int styleNumber, int caseMode) { send_message(SCI_STYLESETCASE, styleNumber, caseMode); }
	int style_get_case(int styleNumber) { return send_message(SCI_STYLEGETCASE, styleNumber, 0); }
	void style_set_visible(int styleNumber, bool visible) { send_message(SCI_STYLESETVISIBLE, styleNumber, visible ? 1 : 0); }
	bool style_get_visible(int styleNumber) { return send_message(SCI_STYLEGETVISIBLE, styleNumber, 0) != 0; }
	void style_set_changeable(int styleNumber, bool changeable) { send_message(SCI_STYLESETCHANGEABLE, styleNumber, changeable ? 1 : 0); }
	bool style_get_changeable(int styleNumber) { return send_message(SCI_STYLEGETCHANGEABLE, styleNumber, 0) != 0; }
	void style_set_hotspot(int styleNumber, bool hotspot) { send_message(SCI_STYLESETHOTSPOT, styleNumber, hotspot ? 1 : 0); }
	bool style_get_hotspot(int styleNumber) { return send_message(SCI_STYLEGETHOTSPOT, styleNumber, 0) != 0; }
	void set_sel_foreground(bool useSelectionForeColour, int colour) { send_message(SCI_SETSELFORE, useSelectionForeColour ? 1 : 0, colour); }
	void set_sel_background(bool useSelectionBackColour, int colour) { send_message(SCI_SETSELBACK, useSelectionBackColour ? 1 : 0, colour); }
	void set_sel_alpha(int alpha) { send_message(SCI_SETSELALPHA, alpha, 0); }
	int get_sel_alpha() { return send_message(SCI_GETSELALPHA, 0, 0); }
	void set_sel_eol_filled(bool filled) { send_message(SCI_SETSELEOLFILLED, filled ? 1 : 0, 0); }
	bool get_sel_eol_filled() { return send_message(SCI_GETSELEOLFILLED, 0, 0) != 0; }
	void set_caret_foreground(int colour) { send_message(SCI_SETCARETFORE, colour, 0); }
	int get_caret_foreground() { return send_message(SCI_GETCARETFORE, 0, 0); }
	void set_caret_line_visible(bool show) { send_message(SCI_SETCARETLINEVISIBLE, show ? 1 : 0, 0); }
	bool get_caret_line_visible() { return send_message(SCI_GETCARETLINEVISIBLE, 0, 0) != 0; }
	void set_caret_line_background(int colour) { send_message(SCI_SETCARETLINEBACK, colour, 0); }
	int get_caret_line_background() { return send_message(SCI_GETCARETLINEBACK, 0, 0); }
	void set_caret_line_background_alpha(int alpha) { send_message(SCI_SETCARETLINEBACKALPHA, alpha, 0); }
	int get_caret_line_background_alpha() { return send_message(SCI_GETCARETLINEBACKALPHA, 0, 0); }
	void set_caret_period(int milliseconds) { send_message(SCI_SETCARETPERIOD, milliseconds, 0); }
	int get_caret_period() { return send_message(SCI_GETCARETPERIOD, 0, 0); }
	void set_caret_style(int style) { send_message(SCI_SETCARETSTYLE, style, 0); }
	int get_caret_style() { return send_message(SCI_GETCARETSTYLE, 0, 0); }
	void set_caret_width(int pixels) { send_message(SCI_SETCARETWIDTH, pixels, 0); }
	int get_caret_width() { return send_message(SCI_GETCARETWIDTH, 0, 0); }
	void set_hotspot_active_foreground(bool useSetting, int colour) { send_message(SCI_SETHOTSPOTACTIVEFORE, useSetting ? 1 : 0, colour); }
	int get_hotspot_active_foreground() { return send_message(SCI_GETHOTSPOTACTIVEFORE, 0, 0); }
	void set_hotspot_active_background(bool useSetting, int colour) { send_message(SCI_SETHOTSPOTACTIVEBACK, useSetting ? 1 : 0, colour); }
	int get_hotspot_active_background() { return send_message(SCI_GETHOTSPOTACTIVEBACK, 0, 0); }
	void set_hotspot_active_underline(bool underline) { send_message(SCI_SETHOTSPOTACTIVEUNDERLINE, underline ? 1 : 0, 0); }
	bool get_hotspot_active_underline() { return send_message(SCI_GETHOTSPOTACTIVEUNDERLINE, 0, 0) != 0; }
	void set_hotspot_single_line(bool singleLine) { send_message(SCI_SETHOTSPOTSINGLELINE, singleLine ? 1 : 0, 0); }
	bool get_hotspot_single_line() { return send_message(SCI_GETHOTSPOTSINGLELINE, 0, 0) != 0; }
	void set_control_char_symbol(int symbol) { send_message(SCI_SETCONTROLCHARSYMBOL, symbol, 0); }
	int get_control_char_symbol() { return send_message(SCI_GETCONTROLCHARSYMBOL, 0, 0); }
	void set_caret_sticky(int useCaretStickyBehaviour) { send_message(SCI_SETCARETSTICKY, useCaretStickyBehaviour, 0); }
	int get_caret_sticky() { return send_message(SCI_GETCARETSTICKY, 0, 0); }
	void toggle_caret_sticky() { send_message(SCI_TOGGLECARETSTICKY, 0, 0); }
	void set_margin_type(int margin, int type) { send_message(SCI_SETMARGINTYPEN, margin, type); }
	int get_margin_type(int margin) { return send_message(SCI_GETMARGINTYPEN, margin, 0); }
	void set_margin_width(int margin, int pixelWidth) { send_message(SCI_SETMARGINWIDTHN, margin, pixelWidth); }
	int get_margin_width(int margin) { return send_message(SCI_GETMARGINWIDTHN, margin, 0); }
	void set_margin_mask(int margin, int mask) { send_message(SCI_SETMARGINMASKN, margin, mask); }
	int get_margin_mask(int margin) { return send_message(SCI_GETMARGINMASKN, margin, 0); }
	void set_margin_sensitive(int margin, bool sensitive) { send_message(SCI_SETMARGINSENSITIVEN, margin, sensitive ? 1 : 0); }
	bool get_margin_sensitive(int margin) { return send_message(SCI_GETMARGINSENSITIVEN, margin, 0) != 0; }
	void set_margin_cursor(int margin, int cursor) { send_message(SCI_SETMARGINCURSORN, margin, cursor); }
	int get_margin_cursor(int margin) { return send_message(SCI_GETMARGINCURSORN, margin, 0); }
	void set_margin_left(int pixels) { send_message(SCI_SETMARGINLEFT, 0, pixels); }
	int get_margin_left() { return send_message(SCI_GETMARGINLEFT, 0, 0); }
	void set_margin_right(int pixels) { send_message(SCI_SETMARGINRIGHT, 0, pixels); }
	int get_margin_right() { return send_message(SCI_GETMARGINRIGHT, 0, 0); }
	void set_fold_margin_color(bool useSetting, int colour) { send_message(SCI_SETFOLDMARGINCOLOUR, useSetting ? 1 : 0, colour); }
	void set_fold_margin_hi_color(bool useSetting, int colour) { send_message(SCI_SETFOLDMARGINHICOLOUR, useSetting ? 1 : 0, colour); }
	void margin_set_text(int line, char *text) { send_message(SCI_MARGINSETTEXT, line, (sptr_t)text); }
	int margin_get_text(int line, char *text) { return send_message(SCI_MARGINGETTEXT, line, (sptr_t)text); }
	void margin_set_style(int line, int style) { send_message(SCI_MARGINSETSTYLE, line, style); }
	int margin_get_style(int line) { return send_message(SCI_MARGINGETSTYLE, line, 0); }
	void margin_set_styles(int line, char *styles) { send_message(SCI_MARGINSETSTYLES, line, (sptr_t)styles); }
	int margin_get_styles(int line, char *styles) { return send_message(SCI_MARGINGETSTYLES, line, (sptr_t)styles); }
	void margin_text_clear_all() { send_message(SCI_MARGINTEXTCLEARALL, 0, 0); }
	void margin_set_style_offset(int style) { send_message(SCI_MARGINSETSTYLEOFFSET, style, 0); }
	int margin_get_style_offset() { return send_message(SCI_MARGINGETSTYLEOFFSET, 0, 0); }
	void set_margin_options(int marginOptions) { send_message(SCI_SETMARGINOPTIONS, marginOptions, 0); }
	int get_margin_options() { return send_message(SCI_GETMARGINOPTIONS, 0, 0); }
	void annotation_set_text(int line, char *text) { send_message(SCI_ANNOTATIONSETTEXT, line, (sptr_t)text); }
	int annotation_get_text(int line, char *text) { return send_message(SCI_ANNOTATIONGETTEXT, line, (sptr_t)text); }
	void annotation_set_style(int line, int style) { send_message(SCI_ANNOTATIONSETSTYLE, line, style); }
	int annotation_get_style(int line) { return send_message(SCI_ANNOTATIONGETSTYLE, line, 0); }
	void annotation_set_styles(int line, char *styles) { send_message(SCI_ANNOTATIONSETSTYLES, line, (sptr_t)styles); }
	int annotation_get_styles(int line, char *styles) { return send_message(SCI_ANNOTATIONGETSTYLES, line, (sptr_t)styles); }
	int annotation_get_lines(int line) { return send_message(SCI_ANNOTATIONGETLINES, line, 0); }
	void annotation_clear_all() { send_message(SCI_ANNOTATIONCLEARALL, 0, 0); }
	void annotation_set_visible(int visible) { send_message(SCI_ANNOTATIONSETVISIBLE, visible, 0); }
	int annotation_get_visible() { return send_message(SCI_ANNOTATIONGETVISIBLE, 0, 0); }
	void annotation_set_style_offset(int style) { send_message(SCI_ANNOTATIONSETSTYLEOFFSET, style, 0); }
	int annotation_get_style_offset() { return send_message(SCI_ANNOTATIONGETSTYLEOFFSET, 0, 0); }
	void set_use_palette(bool allowPaletteUse) { send_message(SCI_SETUSEPALETTE, allowPaletteUse ? 1 : 0, 0); }
	bool get_use_palette() { return send_message(SCI_GETUSEPALETTE, 0, 0) != 0; }
	void set_buffered_draw(bool isBuffered) { send_message(SCI_SETBUFFEREDDRAW, isBuffered ? 1 : 0, 0); }
	bool get_buffered_draw() { return send_message(SCI_GETBUFFEREDDRAW, 0, 0) != 0; }
	void set_two_phase_draw(bool twoPhase) { send_message(SCI_SETTWOPHASEDRAW, twoPhase ? 1 : 0, 0); }
	bool get_two_phase_draw() { return send_message(SCI_GETTWOPHASEDRAW, 0, 0) != 0; }
	void set_font_quality(int fontQuality) { send_message(SCI_SETFONTQUALITY, fontQuality, 0); }
	int get_font_quality() { return send_message(SCI_GETFONTQUALITY, 0, 0); }
	void set_code_page(int codePage) { send_message(SCI_SETCODEPAGE, codePage, 0); }
	int get_code_page() { return send_message(SCI_GETCODEPAGE, 0, 0); }
	void set_keys_unicode(bool keysUnicode) { send_message(SCI_SETKEYSUNICODE, keysUnicode ? 1 : 0, 0); }
	bool get_keys_unicode() { return send_message(SCI_GETKEYSUNICODE, 0, 0) != 0; }
	void set_word_chars(const char *chars) { send_message(SCI_SETWORDCHARS, 0, (sptr_t)chars); }
	void set_whitespace_chars(const char *chars) { send_message(SCI_SETWHITESPACECHARS, 0, (sptr_t)chars); }
	void set_chars_default() { send_message(SCI_SETCHARSDEFAULT, 0, 0); }
	void grab_focus() { send_message(SCI_GRABFOCUS, 0, 0); }
	void set_focus(bool focus) { send_message(SCI_SETFOCUS, focus ? 1 : 0, 0); }
	bool get_focus() { return send_message(SCI_GETFOCUS, 0, 0) != 0; }
	void brace_highlight(int pos1, int pos2) { send_message(SCI_BRACEHIGHLIGHT, pos1, pos2); }
	void brace_badlight(int pos1) { send_message(SCI_BRACEBADLIGHT, pos1, 0); }
	void brace_highlight_indicator(bool useBraceHighlightIndicator, int indicatorNumber) { send_message(SCI_BRACEHIGHLIGHTINDICATOR, useBraceHighlightIndicator ? 1 : 0, indicatorNumber); }
	void brace_badlight_indicator(bool useBraceBadLightIndicator, int indicatorNumber) { send_message(SCI_BRACEBADLIGHTINDICATOR, useBraceBadLightIndicator ? 1 : 0, indicatorNumber); }
	void brace_match(int position, int maxReStyle) { send_message(SCI_BRACEMATCH, position, maxReStyle); }
	void set_tab_width(int widthInChars) { send_message(SCI_SETTABWIDTH, widthInChars, 0); }
	int get_tab_width() { return send_message(SCI_GETTABWIDTH, 0, 0); }
	void set_use_tabs(bool useTabs) { send_message(SCI_SETUSETABS, useTabs ? 1 : 0, 0); }
	bool get_use_tabs() { return send_message(SCI_GETUSETABS, 0, 0) != 0; }
	void set_indent(int widthInChars) { send_message(SCI_SETINDENT, widthInChars, 0); }
	int get_indent() { return send_message(SCI_GETINDENT, 0, 0); }
	void set_tab_indents(bool tabIndents) { send_message(SCI_SETTABINDENTS, tabIndents ? 1 : 0, 0); }
	bool get_tab_indents() { return send_message(SCI_GETTABINDENTS, 0, 0) != 0; }
	void set_backspace_unindents(bool bsUnIndents) { send_message(SCI_SETBACKSPACEUNINDENTS, bsUnIndents ? 1 : 0, 0); }
	bool get_backspace_unindents() { return send_message(SCI_GETBACKSPACEUNINDENTS, 0, 0) != 0; }
	void set_line_indentation(int line, int indentation) { send_message(SCI_SETLINEINDENTATION, line, indentation); }
	int get_line_indentation(int line) { return send_message(SCI_GETLINEINDENTATION, line, 0); }
	int get_line_indent_position(int line) { return send_message(SCI_GETLINEINDENTPOSITION, line, 0); }
	void set_indentation_guides(int indentView) { send_message(SCI_SETINDENTATIONGUIDES, indentView, 0); }
	int get_indentation_guides() { return send_message(SCI_GETINDENTATIONGUIDES, 0, 0); }
	void set_highlight_guide(int column) { send_message(SCI_SETHIGHLIGHTGUIDE, column, 0); }
	int get_highlight_guide() { return send_message(SCI_GETHIGHLIGHTGUIDE, 0, 0); }
	void marker_define(int markerNumber, int markerSymbols) { send_message(SCI_MARKERDEFINE, markerNumber, markerSymbols); }
	void marker_define_pixmap(int markerNumber, const char *xpm) { send_message(SCI_MARKERDEFINEPIXMAP, markerNumber, (sptr_t)xpm); }
	void rgba_image_set_width(int width) { send_message(SCI_RGBAIMAGESETWIDTH, width, 0); }
	void rgba_image_set_height(int height) { send_message(SCI_RGBAIMAGESETHEIGHT, height, 0); }
	void marker_define_rgba_image(int markerNumber, const char *pixels) { send_message(SCI_MARKERDEFINERGBAIMAGE, markerNumber, (sptr_t)pixels); }
	void marker_symbol_defined(int markerNumber) { send_message(SCI_MARKERSYMBOLDEFINED, markerNumber, 0); }
	void marker_set_foreground(int markerNumber, int colour) { send_message(SCI_MARKERSETFORE, markerNumber, colour); }
	void marker_set_background(int markerNumber, int colour) { send_message(SCI_MARKERSETBACK, markerNumber, colour); }
	void marker_set_background_selected(int markerNumber, int colour) { send_message(SCI_MARKERSETBACKSELECTED, markerNumber, colour); }
	void marker_enable_highlight(int enabled) { send_message(SCI_MARKERENABLEHIGHLIGHT, enabled, 0); }
	void marker_set_alpha(int markerNumber, int alpha) { send_message(SCI_MARKERSETALPHA, markerNumber, alpha); }
	void marker_add(int line, int markerNumber) { send_message(SCI_MARKERADD, line, markerNumber); }
	void marker_add_set(int line, int markerMask) { send_message(SCI_MARKERADDSET, line, markerMask); }
	void marker_delete(int line, int markerNumber) { send_message(SCI_MARKERDELETE, line, markerNumber); }
	void marker_delete_all(int markerNumber) { send_message(SCI_MARKERDELETEALL, markerNumber, 0); }
	int marker_get(int line) { return send_message(SCI_MARKERGET, line, 0); }
	int marker_next(int lineStart, int markerMask) { return send_message(SCI_MARKERNEXT, lineStart, markerMask); }
	int marker_previous(int lineStart, int markerMask) { return send_message(SCI_MARKERPREVIOUS, lineStart, markerMask); }
	int marker_line_from_handle(int handle) { return send_message(SCI_MARKERLINEFROMHANDLE, handle, 0); }
	int marker_delete_handle(int handle) { return send_message(SCI_MARKERDELETEHANDLE, handle, 0); }
	void indic_set_style(int indicatorNumber, int indicatorStyle) { send_message(SCI_INDICSETSTYLE, indicatorNumber, indicatorStyle); }
	int indic_get_style(int indicatorNumber) { return send_message(SCI_INDICGETSTYLE, indicatorNumber, 0); }
	void indic_set_foreground(int indicatorNumber, int colour) { send_message(SCI_INDICSETFORE, indicatorNumber, colour); }
	int indic_get_foreground(int indicatorNumber) { return send_message(SCI_INDICGETFORE, indicatorNumber, 0); }
	void indic_set_alpha(int indicatorNumber, int alpha) { send_message(SCI_INDICSETALPHA, indicatorNumber, alpha); }
	int indic_get_alpha(int indicatorNumber) { return send_message(SCI_INDICGETALPHA, indicatorNumber, 0); }
	void indic_set_outline_alpha(int indicatorNumber, int alpha) { send_message(SCI_INDICSETOUTLINEALPHA, indicatorNumber, alpha); }
	int indic_get_outline_alpha(int indicatorNumber) { return send_message(SCI_INDICGETOUTLINEALPHA, indicatorNumber, 0); }
	void indic_set_underline(int indicatorNumber, bool under) { send_message(SCI_INDICSETUNDER, indicatorNumber, under ? 1 : 0); }
	bool indic_get_underline(int indicatorNumber) { return send_message(SCI_INDICGETUNDER, indicatorNumber, 0) != 0; }
	void autocomplete_show(int lenEntered, const char *list) { send_message(SCI_AUTOCSHOW, lenEntered, (sptr_t)list); }
	void autocomplete_cancel() { send_message(SCI_AUTOCCANCEL, 0, 0); }
	void autocomplete_active() { send_message(SCI_AUTOCACTIVE, 0, 0); }
	int autocomplete_pos_start() { return send_message(SCI_AUTOCPOSSTART, 0, 0); }
	void autocomplete_complete() { send_message(SCI_AUTOCCOMPLETE, 0, 0); }
	void autocomplete_stops(const char *chars) { send_message(SCI_AUTOCSTOPS, 0, (sptr_t)chars); }
	void autocomplete_set_separator(char separator) { send_message(SCI_AUTOCSETSEPARATOR, separator, 0); }
	int autocomplete_get_seperator() { return send_message(SCI_AUTOCGETSEPARATOR, 0, 0); }
	void autocomplete_select(const char *select) { send_message(SCI_AUTOCSELECT, 0, (sptr_t)select); }
	int autocomplete_get_current() { return send_message(SCI_AUTOCGETCURRENT, 0, 0); }
	int autocomplete_get_current_text(char *text) { return send_message(SCI_AUTOCGETCURRENTTEXT, 0, (sptr_t)text); }
	void autocomplete_set_cancel_at_start(bool cancel) { send_message(SCI_AUTOCSETCANCELATSTART, cancel ? 1 : 0, 0); }
	bool autocomplete_get_cancel_at_start() { return send_message(SCI_AUTOCGETCANCELATSTART, 0, 0) != 0; }
	void autocomplete_set_fill_ups(const char *chars) { send_message(SCI_AUTOCSETFILLUPS, 0, (sptr_t)chars); }
	void autocomplete_set_choose_single(bool chooseSingle) { send_message(SCI_AUTOCSETCHOOSESINGLE, chooseSingle ? 1 : 0, 0); }
	bool autocomplete_get_choose_single() { return send_message(SCI_AUTOCGETCHOOSESINGLE, 0, 0) != 0; }
	void autocomplete_set_ignore_case(bool ignoreCase) { send_message(SCI_AUTOCSETIGNORECASE, ignoreCase ? 1 : 0, 0); }
	bool autocomplete_get_ignore_case() { return send_message(SCI_AUTOCGETIGNORECASE, 0, 0) != 0; }
	void autocomplete_set_auto_hide(bool autoHide) { send_message(SCI_AUTOCSETAUTOHIDE, autoHide ? 1 : 0, 0); }
	bool autocomplete_get_auto_hide() { return send_message(SCI_AUTOCGETAUTOHIDE, 0, 0) != 0; }
	void autocomplete_set_drop_rest_of_word(bool dropRestOfWord) { send_message(SCI_AUTOCSETDROPRESTOFWORD, dropRestOfWord ? 1 : 0, 0); }
	bool autocomplete_get_drop_rest_of_word() { return send_message(SCI_AUTOCGETDROPRESTOFWORD, 0, 0) != 0; }
	int register_image(int type, const char *xpmData) { return send_message(SCI_REGISTERIMAGE, type, (sptr_t)xpmData); }
	int register_rgba_image(int type, const char *pixels) { return send_message(SCI_REGISTERRGBAIMAGE, type, (sptr_t)pixels); }
	int clear_registered_images() { return send_message(SCI_CLEARREGISTEREDIMAGES, 0, 0); }
	void autocomplete_set_type_separator(char separatorCharacter) { send_message(SCI_AUTOCSETTYPESEPARATOR, separatorCharacter, 0); }
	int autocomplete_get_type_separator() { return send_message(SCI_AUTOCGETTYPESEPARATOR, 0, 0); }
	void autocomplete_set_max_height(int rowCount) { send_message(SCI_AUTOCSETMAXHEIGHT, rowCount, 0); }
	int autocomplete_get_max_height() { return send_message(SCI_AUTOCGETMAXHEIGHT, 0, 0); }
	void autocomplete_set_max_width(int characterCount) { send_message(SCI_AUTOCSETMAXWIDTH, characterCount, 0); }
	int autocomplete_get_max_width() { return send_message(SCI_AUTOCGETMAXWIDTH, 0, 0); }
	void calltip_show(int posStart, const char *definition) { send_message(SCI_CALLTIPSHOW, posStart, (sptr_t)definition); }
	void calltip_cancel() { send_message(SCI_CALLTIPCANCEL, 0, 0); }
	void calltip_active() { send_message(SCI_CALLTIPACTIVE, 0, 0); }
	int calltip_posstart() { return send_message(SCI_CALLTIPPOSSTART, 0, 0); }
	void calltip_set_highlight(int highlightStart, int highlightEnd) { send_message(SCI_CALLTIPSETHLT, highlightStart, highlightEnd); }
	void calltip_set_backrgound(int colour) { send_message(SCI_CALLTIPSETBACK, colour, 0); }
	void calltip_set_foreground(int colour) { send_message(SCI_CALLTIPSETFORE, colour, 0); }
	void calltip_set_foreground_highlight(int colour) { send_message(SCI_CALLTIPSETFOREHLT, colour, 0); }
	void calltip_use_style(int tabsize) { send_message(SCI_CALLTIPUSESTYLE, tabsize, 0); }

	void line_down() { send_message(SCI_LINEDOWN, 0, 0); }
	void line_down_extend() { send_message(SCI_LINEDOWNEXTEND, 0, 0); }
	void line_down_rect_extend() { send_message(SCI_LINEDOWNRECTEXTEND, 0, 0); }
	void line_scroll_down() { send_message(SCI_LINESCROLLDOWN, 0, 0); }
	void line_up() { send_message(SCI_LINEUP, 0, 0); }
	void line_up_extend() { send_message(SCI_LINEUPEXTEND, 0, 0); }
	void line_up_rect_extend() { send_message(SCI_LINEUPRECTEXTEND, 0, 0); }
	void line_scroll_up() { send_message(SCI_LINESCROLLUP, 0, 0); }
	void paragraph_down() { send_message(SCI_PARADOWN, 0, 0); }
	void paragraph_down_extend() { send_message(SCI_PARADOWNEXTEND, 0, 0); }
	void paragraph_up() { send_message(SCI_PARAUP, 0, 0); }
	void paragraph_up_extend() { send_message(SCI_PARAUPEXTEND, 0, 0); }
	void char_left() { send_message(SCI_CHARLEFT, 0, 0); }
	void char_left_extend() { send_message(SCI_CHARLEFTEXTEND, 0, 0); }
	void char_left_rect_extend() { send_message(SCI_CHARLEFTRECTEXTEND, 0, 0); }
	void char_right() { send_message(SCI_CHARRIGHT, 0, 0); }
	void char_right_extend() { send_message(SCI_CHARRIGHTEXTEND, 0, 0); }
	void char_right_rect_extend() { send_message(SCI_CHARRIGHTRECTEXTEND, 0, 0); }
	void word_left() { send_message(SCI_WORDLEFT, 0, 0); }
	void word_left_extend() { send_message(SCI_WORDLEFTEXTEND, 0, 0); }
	void word_right() { send_message(SCI_WORDRIGHT, 0, 0); }
	void word_right_extend() { send_message(SCI_WORDRIGHTEXTEND, 0, 0); }
	void word_left_end() { send_message(SCI_WORDLEFTEND, 0, 0); }
	void word_left_end_extend() { send_message(SCI_WORDLEFTENDEXTEND, 0, 0); }
	void word_right_end() { send_message(SCI_WORDRIGHTEND, 0, 0); }
	void word_right_end_extend() { send_message(SCI_WORDRIGHTENDEXTEND, 0, 0); }
	void word_part_left() { send_message(SCI_WORDPARTLEFT, 0, 0); }
	void word_part_left_extend() { send_message(SCI_WORDPARTLEFTEXTEND, 0, 0); }
	void word_part_right() { send_message(SCI_WORDPARTRIGHT, 0, 0); }
	void word_part_right_extend() { send_message(SCI_WORDPARTRIGHTEXTEND, 0, 0); }
	void home() { send_message(SCI_HOME, 0, 0); }
	void home_extend() { send_message(SCI_HOMEEXTEND, 0, 0); }
	void home_rect_extend() { send_message(SCI_HOMERECTEXTEND, 0, 0); }
	void home_display() { send_message(SCI_HOMEDISPLAY, 0, 0); }
	void home_display_extend() { send_message(SCI_HOMEDISPLAYEXTEND, 0, 0); }
	void home_wrap() { send_message(SCI_HOMEWRAP, 0, 0); }
	void home_wrap_extend() { send_message(SCI_HOMEWRAPEXTEND, 0, 0); }
	void vc_home() { send_message(SCI_VCHOME, 0, 0); }
	void vc_home_extend() { send_message(SCI_VCHOMEEXTEND, 0, 0); }
	void vc_home_rect_extend() { send_message(SCI_VCHOMERECTEXTEND, 0, 0); }
	void vc_home_wrap() { send_message(SCI_VCHOMEWRAP, 0, 0); }
	void vc_home_wrap_extend() { send_message(SCI_VCHOMEWRAPEXTEND, 0, 0); }
	void line_end() { send_message(SCI_LINEEND, 0, 0); }
	void line_end_extend() { send_message(SCI_LINEENDEXTEND, 0, 0); }
	void line_end_rect_extend() { send_message(SCI_LINEENDRECTEXTEND, 0, 0); }
	void line_end_display() { send_message(SCI_LINEENDDISPLAY, 0, 0); }
	void line_end_display_extend() { send_message(SCI_LINEENDDISPLAYEXTEND, 0, 0); }
	void line_end_wrap() { send_message(SCI_LINEENDWRAP, 0, 0); }
	void line_end_wrap_extend() { send_message(SCI_LINEENDWRAPEXTEND, 0, 0); }
	void document_start() { send_message(SCI_DOCUMENTSTART, 0, 0); }
	void document_start_extend() { send_message(SCI_DOCUMENTSTARTEXTEND, 0, 0); }
	void document_end() { send_message(SCI_DOCUMENTEND, 0, 0); }
	void document_end_extend() { send_message(SCI_DOCUMENTENDEXTEND, 0, 0); }
	void page_up() { send_message(SCI_PAGEUP, 0, 0); }
	void page_up_extend() { send_message(SCI_PAGEUPEXTEND, 0, 0); }
	void page_up_rect_extend() { send_message(SCI_PAGEUPRECTEXTEND, 0, 0); }
	void page_down() { send_message(SCI_PAGEDOWN, 0, 0); }
	void page_down_extend() { send_message(SCI_PAGEDOWNEXTEND, 0, 0); }
	void page_down_rect_extend() { send_message(SCI_PAGEDOWNRECTEXTEND, 0, 0); }
	void stuttered_page_up() { send_message(SCI_STUTTEREDPAGEUP, 0, 0); }
	void stuttered_page_up_extend() { send_message(SCI_STUTTEREDPAGEUPEXTEND, 0, 0); }
	void stuttered_page_down() { send_message(SCI_STUTTEREDPAGEDOWN, 0, 0); }
	void stuttered_page_down_extend() { send_message(SCI_STUTTEREDPAGEDOWNEXTEND, 0, 0); }
	void delete_back() { send_message(SCI_DELETEBACK, 0, 0); }
	void delete_back_not_line() { send_message(SCI_DELETEBACKNOTLINE, 0, 0); }
	void del_word_left() { send_message(SCI_DELWORDLEFT, 0, 0); }
	void del_word_right() { send_message(SCI_DELWORDRIGHT, 0, 0); }
	void del_word_right_end() { send_message(SCI_DELWORDRIGHTEND, 0, 0); }
	void del_line_left() { send_message(SCI_DELLINELEFT, 0, 0); }
	void del_line_right() { send_message(SCI_DELLINERIGHT, 0, 0); }
	void line_delete() { send_message(SCI_LINEDELETE, 0, 0); }
	void line_cut() { send_message(SCI_LINECUT, 0, 0); }
	void line_copy() { send_message(SCI_LINECOPY, 0, 0); }
	void line_transpose() { send_message(SCI_LINETRANSPOSE, 0, 0); }
	void line_duplicate() { send_message(SCI_LINEDUPLICATE, 0, 0); }
	void lowercase() { send_message(SCI_LOWERCASE, 0, 0); }
	void uppercase() { send_message(SCI_UPPERCASE, 0, 0); }
	void cancel() { send_message(SCI_CANCEL, 0, 0); }
	void edit_toggle_overtype() { send_message(SCI_EDITTOGGLEOVERTYPE, 0, 0); }
	void newline() { send_message(SCI_NEWLINE, 0, 0); }
	void formfeed() { send_message(SCI_FORMFEED, 0, 0); }
	void tab() { send_message(SCI_TAB, 0, 0); }
	void backtab() { send_message(SCI_BACKTAB, 0, 0); }
	void selection_duplicate() { send_message(SCI_SELECTIONDUPLICATE, 0, 0); }
	void vertical_centre_caret() { send_message(SCI_VERTICALCENTRECARET, 0, 0); }
	void move_selected_lines_up() { send_message(SCI_MOVESELECTEDLINESUP, 0, 0); }
	void move_selected_lines_down() { send_message(SCI_MOVESELECTEDLINESDOWN, 0, 0); }
	void scroll_to_start() { send_message(SCI_SCROLLTOSTART, 0, 0); }
	void scroll_to_end() { send_message(SCI_SCROLLTOEND, 0, 0); }

	void assign_cmd_key(int keyDefinition, int sciCommand) { send_message(SCI_ASSIGNCMDKEY, keyDefinition, sciCommand); }
	void clear_cmd_key(int keyDefinition) { send_message(SCI_CLEARCMDKEY, keyDefinition, 0); }
	void clear_all_cmd_keys() { send_message(SCI_CLEARALLCMDKEYS, 0, 0); }
	void use_popup(bool bEnablePopup) { send_message(SCI_USEPOPUP, bEnablePopup ? 1 : 0, 0); }
	void start_record() { send_message(SCI_STARTRECORD, 0, 0); }
	void stop_record() { send_message(SCI_STOPRECORD, 0, 0); }
	void format_range(bool bDraw, Scintilla::Sci_RangeToFormat *pfr) { send_message(SCI_FORMATRANGE, bDraw ? 1 : 0, (sptr_t)pfr); }
	void set_print_magnification(int magnification) { send_message(SCI_SETPRINTMAGNIFICATION, magnification, 0); }
	int get_print_magnification() { return send_message(SCI_GETPRINTMAGNIFICATION, 0, 0); }
	void set_print_colou_mode(int mode) { send_message(SCI_SETPRINTCOLOURMODE, mode, 0); }
	int get_print_color_mode() { return send_message(SCI_GETPRINTCOLOURMODE, 0, 0); }
	void set_print_wrap_mode() { send_message(SCI_SETPRINTWRAPMODE, 0, 0); }
	int get_print_wrap_mode() { return send_message(SCI_GETPRINTWRAPMODE, 0, 0); }
	Scintilla::Document *get_doc_pointer() { return (Scintilla::Document*)send_message(SCI_GETDOCPOINTER, 0, 0); }
	void set_doc_pointer(Scintilla::Document *pDoc) { send_message(SCI_SETDOCPOINTER, 0, (sptr_t)pDoc); }
	Scintilla::Document *create_document() { return (Scintilla::Document*)send_message(SCI_CREATEDOCUMENT, 0, 0); }
	void add_ref_document(Scintilla::Document *pDoc) { send_message(SCI_ADDREFDOCUMENT, 0, (sptr_t)pDoc); }
	void release_document(Scintilla::Document *pDoc) { send_message(SCI_RELEASEDOCUMENT, 0, (sptr_t)pDoc); }
	int visible_from_doc_line(int docLine) { return send_message(SCI_VISIBLEFROMDOCLINE, docLine, 0); }
	int doc_line_from_visible(int displayLine) { return send_message(SCI_DOCLINEFROMVISIBLE, displayLine, 0); }
	void show_lines(int lineStart, int lineEnd) { send_message(SCI_SHOWLINES, lineStart, lineEnd); }
	void hide_lines(int lineStart, int lineEnd) { send_message(SCI_HIDELINES, lineStart, lineEnd); }
	int get_line_visible(int line) { return send_message(SCI_GETLINEVISIBLE, line, 0); }
	void set_fold_level(int line, int level) { send_message(SCI_SETFOLDLEVEL, line, level); }
	int get_fold_level(int line) { return send_message(SCI_GETFOLDLEVEL, line, 0); }
	void set_fold_flags(int flags) { send_message(SCI_SETFOLDFLAGS, flags, 0); }
	int get_last_child(int line, int level) { return send_message(SCI_GETLASTCHILD, line, level); }
	int get_fold_parent(int line) { return send_message(SCI_GETFOLDPARENT, line, 0); }
	void set_fold_expanded(int line, bool expanded) { send_message(SCI_SETFOLDEXPANDED, line, expanded ? 1 : 0); }
	bool get_fold_expanded(int line) { return send_message(SCI_GETFOLDEXPANDED, line, 0) != 0; }
	void contracted_fold_next(int lineStart) { send_message(SCI_CONTRACTEDFOLDNEXT, lineStart, 0); }
	void toggle_fold(int line) { send_message(SCI_TOGGLEFOLD, line, 0); }
	void ensure_visible(int line) { send_message(SCI_ENSUREVISIBLE, line, 0); }
	void ensure_visible_enforce_policy(int line) { send_message(SCI_ENSUREVISIBLEENFORCEPOLICY, line, 0); }
	void set_wrap_mode(int wrapMode) { send_message(SCI_SETWRAPMODE, wrapMode, 0); }
	int get_wrap_mode() { return send_message(SCI_GETWRAPMODE, 0, 0); }
	void set_wrap_visual_flags(int wrapVisualFlags) { send_message(SCI_SETWRAPVISUALFLAGS, wrapVisualFlags, 0); }
	int get_wrap_visual_flags() { return send_message(SCI_GETWRAPVISUALFLAGS, 0, 0); }
	void set_wrap_visual_flags_location(int wrapVisualFlagsLocation) { send_message(SCI_SETWRAPVISUALFLAGSLOCATION, wrapVisualFlagsLocation, 0); }
	int get_wrap_visual_flags_location() { return send_message(SCI_GETWRAPVISUALFLAGSLOCATION, 0, 0); }
	void set_wrap_indent_mode(int indentMode) { send_message(SCI_SETWRAPINDENTMODE, indentMode, 0); }
	int get_wrap_indent_mode() { return send_message(SCI_GETWRAPINDENTMODE, 0, 0); }
	void set_wrap_start_indent(int indent) { send_message(SCI_SETWRAPSTARTINDENT, indent, 0); }
	int get_wrap_start_indent() { return send_message(SCI_GETWRAPSTARTINDENT, 0, 0); }
	void set_layout_cache(int cacheMode) { send_message(SCI_SETLAYOUTCACHE, cacheMode, 0); }
	int get_layout_cache() { return send_message(SCI_GETLAYOUTCACHE, 0, 0); }
	void set_position_cache(int size) { send_message(SCI_SETPOSITIONCACHE, size, 0); }
	int get_position_cache() { return send_message(SCI_GETPOSITIONCACHE, 0, 0); }
	void line_split(int pixelWidth) { send_message(SCI_LINESSPLIT, pixelWidth, 0); }
	void lines_join() { send_message(SCI_LINESJOIN, 0, 0); }
	int wrap_count(int docLine) { return send_message(SCI_WRAPCOUNT, docLine, 0); }
	void zoom_in() { send_message(SCI_ZOOMIN, 0, 0); }
	void zoom_out() { send_message(SCI_ZOOMOUT, 0, 0); }
	void set_zoom(int zoomInPoints) { send_message(SCI_SETZOOM, zoomInPoints, 0); }
	int get_zoom() { return send_message(SCI_GETZOOM, 0, 0); }
	void set_edge_mode(int mode) { send_message(SCI_SETEDGEMODE, mode, 0); }
	int get_edge_mode() { return send_message(SCI_GETEDGEMODE, 0, 0); }
	void set_edge_column(int column) { send_message(SCI_SETEDGECOLUMN, column, 0); }
	int get_edge_column() { return send_message(SCI_GETEDGECOLUMN, 0, 0); }
	void set_edge_color(int colour) { send_message(SCI_SETEDGECOLOUR, colour, 0); }
	int get_edge_color() { return send_message(SCI_GETEDGECOLOUR, 0, 0); }
	void set_lexer(int lexer) { send_message(SCI_SETLEXER, lexer, 0); }
	int get_lexer() { return send_message(SCI_GETLEXER, 0, 0); }
	void set_lexer_language(const char *name) { send_message(SCI_SETLEXERLANGUAGE, 0, (sptr_t)name); }
	int get_lexer_language(char *name) { return send_message(SCI_GETLEXERLANGUAGE, 0, (sptr_t)name); }
	void load_lexer_library(const char *path) { send_message(SCI_LOADLEXERLIBRARY, 0, (sptr_t)path); }
	int colorize(int start, int end) { return send_message(SCI_COLOURISE, start, end); }
	int change_lexer_state(int start, int end) { return send_message(SCI_CHANGELEXERSTATE, start, end); }
	int property_names(char *names) { return send_message(SCI_PROPERTYNAMES, 0, (sptr_t)names); }
	int property_type(const char *name) { return send_message(SCI_PROPERTYTYPE, (sptr_t)name, 0); }
	int describe_property(const char *name, char *description) { return send_message(SCI_DESCRIBEPROPERTY, (sptr_t)name, (sptr_t)description); }
	void set_property(const char *key, const char *value) { send_message(SCI_SETPROPERTY, (uptr_t)key, (sptr_t)value); }
	int get_property(const char *key, char *value) { return send_message(SCI_GETPROPERTY, (uptr_t)key, (sptr_t)value); }
	int get_property_expanded(const char *key, char *value) { return send_message(SCI_GETPROPERTYEXPANDED, (uptr_t)key, (sptr_t)value); }
	int get_property_int(const char *key, int default_) { return send_message(SCI_GETPROPERTYINT, (uptr_t)key, default_); }
	int describe_keyword_sets(char *descriptions) { return send_message(SCI_DESCRIBEKEYWORDSETS, 0, (sptr_t)descriptions); }
	void set_keywords(int keyWordSet, const char *keyWordList) { send_message(SCI_SETKEYWORDS, keyWordSet, (sptr_t)keyWordList); }
	int get_style_bits_needed() { return send_message(SCI_GETSTYLEBITSNEEDED, 0, 0); }
	void set_mod_event_mask(int eventMask) { send_message(SCI_SETMODEVENTMASK, eventMask, 0); }
	int get_mod_event_mask() { return send_message(SCI_GETMODEVENTMASK, 0, 0); }
	void set_mouse_dwell_time(int milliseconds) { send_message(SCI_SETMOUSEDWELLTIME, milliseconds, 0); }
	int get_mouse_dwell_time() { return send_message(SCI_GETMOUSEDWELLTIME, 0, 0); }
	void set_identifier(int identifier) { send_message(SCI_SETIDENTIFIER, identifier, 0); }
	int get_identifier() { return send_message(SCI_GETIDENTIFIER, 0, 0); }

	clan::Callback_v0 cb_style_needed;
	clan::Callback_v0 cb_char_added;
	clan::Callback_v0 cb_save_point_reached;
	clan::Callback_v0 cb_save_point_left;
	clan::Callback_v0 cb_modify_attempt_read_only;
	clan::Callback_v0 cb_key;
	clan::Callback_v0 cb_double_click;
	clan::Callback_v0 cb_update_ui;
	clan::Callback_v0 cb_modified;
	clan::Callback_v1<const std::string &> cb_macro_record;
	clan::Callback_v5<bool, bool, bool, int, int> cb_margin_click;
	clan::Callback_v2<int, int> cb_need_shown;
	clan::Callback_v0 cb_painted;
	clan::Callback_v0 cb_userlist_selection;
	clan::Callback_v2<int, clan::Point> cb_dwell_start;
	clan::Callback_v2<int, clan::Point> cb_dwell_end;
	clan::Callback_v0 cb_zoom;
	clan::Callback_v0 cb_hotspot_click;
	clan::Callback_v0 cb_hotspot_double_click;
	clan::Callback_v0 cb_hotspot_release_click;
	clan::Callback_v0 cb_indicator_click;
	clan::Callback_v0 cb_indicator_release;
	clan::Callback_v0 cb_calltip_click;
	clan::Callback_v0 cb_autocomplete_selection;
	clan::Callback_v0 cb_autocomplete_cancelled;
	clan::Callback_v0 cb_autocomplete_char_deleted;
	clan::Callback_v0 cb_change;

	void play_record(const std::string &str);

private:
	void on_render(clan::Canvas &canvas, const clan::Rect &update_box);
	void on_resized();
	void on_scrollbar_vert_scroll();
	void on_scrollbar_horz_scroll();
	bool on_input_pressed(const clan::InputEvent &e);
	bool on_input_released(const clan::InputEvent &e);
	bool on_input_doubleclick(const clan::InputEvent &e);
	bool on_input_pointer_moved(const clan::InputEvent &e);
	bool on_focus_gained();
	bool on_focus_lost();
	sptr_t send_message(unsigned int message, uptr_t wparam, sptr_t lparam) { return WndProc(message, wparam, lparam); }

	std::string record_string(unsigned int message, uptr_t wparam, sptr_t lparam);

	void Initialise();
	void Finalise();
	void CreateCallTipWindow(Scintilla::PRectangle rc);
	void AddToPopUp(const char *label, int cmd=0, bool enabled=true);
	sptr_t WndProc(unsigned int iMessage, uptr_t wParam, sptr_t lParam);
	sptr_t DefWndProc(unsigned int iMessage, uptr_t wParam, sptr_t lParam);
	void ScrollText(int linesToMove);
	void SetVerticalScrollPos();
	void SetHorizontalScrollPos();
	bool ModifyScrollBars(int nMax, int nPage);
	void Copy();
	void Paste();
	void ClaimSelection();
	void NotifyChange();
	void NotifyParent(Scintilla::SCNotification scn);
	void CopyToClipboard(const Scintilla::SelectionText &selectedText);
	void SetTicking(bool on);
	void SetMouseCapture(bool on);
	bool HaveMouseCapture();

	clan::Canvas sci_get_canvas();
	void sci_set_cliprect(clan::Canvas &canvas, clan::Rect box);
	void Window_Destroy();
	bool Window_HasFocus();
	Scintilla::PRectangle Window_GetPosition();
	void Window_SetPosition(Scintilla::PRectangle rc);
	void Window_SetPositionRelative(Scintilla::PRectangle rc, ScintillaWindowHandle *relativeTo);
	Scintilla::PRectangle Window_GetClientPosition();
	void Window_Show(bool show);
	void Window_InvalidateAll();
	void Window_InvalidateRectangle(Scintilla::PRectangle rc);
	void Window_SetFont(Scintilla::Font &font);
	void Window_SetCursor(Scintilla::Window::Cursor curs);
	void Window_SetTitle(const char *s);
	Scintilla::PRectangle Window_GetMonitorRect(Scintilla::Point pt);

	/////////////////////////////////////////////////////////////////////////
	// ScintillaWin:

	// void UpdateSystemCaret();
	// void CopyAllowLine();
	// bool CanPaste();
	// void NotifyFocus(bool focus);
	// void SetCtrlID(int identifier);
	// int GetCtrlID();
	// void NotifyDoubleClick(Point pt, bool shift, bool ctrl, bool alt);
	// std::string CaseMapString(const std::string &s, int caseMapping);
	// CaseFolder *CaseFolderForEncoding();
	// bool DragThreshold(Point ptStart, Point ptNow);
	// void StartDrag();
	// bool SetIdle(bool);
	// bool PaintContains(PRectangle rc);
	// bool ValidCodePage(int codePage);

	/////////////////////////////////////////////////////////////////////////
	// Other:

	// void RefreshColourPalette(Palette &pal, bool want);
	// void AddCharUTF(char *s, unsigned int len, bool treatAsDBCS=false);
	// void CancelModes();
	// int KeyCommand(unsigned int iMessage);
	// void ButtonDown(Point pt, unsigned int curTime, bool shift, bool ctrl, bool alt);
	// PRectangle GetClientRectangle();
	// void ReconfigureScrollBars();
	// void NotifyStyleToNeeded(int endStyleNeeded);
	// int KeyDefault(int key, int modifiers);
	// void DisplayCursor(Window::Cursor c);
	// void QueueStyling(int upTo);

	ScintillaEditor *component;
	bool is_mouse_captured;
	clan::ScrollBar *scrollbar_vert;
	clan::ScrollBar *scrollbar_horz;
	clan::Rect client_box;
};

class ScintillaListBoxHandle : public ScintillaWindowHandle, public clan::GUIComponent
{
public:
	ScintillaListBoxHandle(clan::GUIComponent *parent) : clan::GUIComponent(parent, "sci-listbox") { }
	clan::Canvas sci_get_canvas() { return get_canvas(); }
	void sci_set_cliprect(clan::Canvas &gc, clan::Rect box) { return set_cliprect(gc, box); }
};

class ScintillaListBox : public Scintilla::ListBox
{
public:
	void SetFont(Scintilla::Font &font);
	void Create(Scintilla::Window &parent, int ctrlID, Scintilla::Point location, int lineHeight_, bool unicodeMode_);
	void SetAverageCharWidth(int width);
	void SetVisibleRows(int rows);
	int GetVisibleRows() const;
	Scintilla::PRectangle GetDesiredRect();
	int CaretFromEdge();
	void Clear();
	void Append(char *s, int type = -1);
	int Length();
	void Select(int n);
	int GetSelection();
	int Find(const char *prefix);
	void GetValue(int n, char *value, int len);
	void RegisterImage(int type, const char *xpm_data);
	void RegisterRGBAImage(int type, int width, int height, const unsigned char *pixelsImage);
	void ClearRegisteredImages();
	void SetDoubleClickAction(Scintilla::CallBackAction, void *);
	void SetList(const char* list, char separator, char typesep);
};

class ScintillaSurface : public Scintilla::Surface
{
public:
	ScintillaSurface();
	ScintillaSurface(ScintillaWindowHandle *component, clan::Canvas &canvas);
	void Init(Scintilla::WindowID wid);
	void Init(Scintilla::SurfaceID sid, Scintilla::WindowID wid);
	void InitPixMap(int width, int height, Scintilla::Surface *surface_, Scintilla::WindowID wid);

	void Release();
	bool Initialised();
	void PenColour(Scintilla::ColourAllocated fore);
	int LogPixelsY();
	int DeviceHeightFont(int points);
	void MoveTo(int x_, int y_);
	void LineTo(int x_, int y_);
	void Polygon(Scintilla::Point *pts, int npts, Scintilla::ColourAllocated fore, Scintilla::ColourAllocated back);
	void RectangleDraw(Scintilla::PRectangle rc, Scintilla::ColourAllocated fore, Scintilla::ColourAllocated back);
	void FillRectangle(Scintilla::PRectangle rc, Scintilla::ColourAllocated back);
	void FillRectangle(Scintilla::PRectangle rc, Scintilla::Surface &surfacePattern);
	void RoundedRectangle(Scintilla::PRectangle rc, Scintilla::ColourAllocated fore, Scintilla::ColourAllocated back);
	void AlphaRectangle(Scintilla::PRectangle rc, int cornerSize, Scintilla::ColourAllocated fill, int alphaFill, Scintilla::ColourAllocated outline, int alphaOutline, int flags);
	void DrawRGBAImage(Scintilla::PRectangle rc, int width, int height, const unsigned char *pixelsImage);
	void Ellipse(Scintilla::PRectangle rc, Scintilla::ColourAllocated fore, Scintilla::ColourAllocated back);
	void Copy(Scintilla::PRectangle rc, Scintilla::Point from, Scintilla::Surface &surfaceSource);

	void DrawTextNoClip(Scintilla::PRectangle rc, Scintilla::Font &font_, int ybase, const char *s, int len, Scintilla::ColourAllocated fore, Scintilla::ColourAllocated back);
	void DrawTextClipped(Scintilla::PRectangle rc, Scintilla::Font &font_, int ybase, const char *s, int len, Scintilla::ColourAllocated fore, Scintilla::ColourAllocated back);
	void DrawTextTransparent(Scintilla::PRectangle rc, Scintilla::Font &font_, int ybase, const char *s, int len, Scintilla::ColourAllocated fore);
	void MeasureWidths(Scintilla::Font &font_, const char *s, int len, int *positions);
	int WidthText(Scintilla::Font &font_, const char *s, int len);
	int WidthChar(Scintilla::Font &font_, char ch);
	int Ascent(Scintilla::Font &font_);
	int Descent(Scintilla::Font &font_);
	int InternalLeading(Scintilla::Font &font_);
	int ExternalLeading(Scintilla::Font &font_);
	int Height(Scintilla::Font &font_);
	int AverageCharWidth(Scintilla::Font &font_);

	int SetPalette(Scintilla::Palette *pal, bool inBackGround);
	void SetClip(Scintilla::PRectangle rc);
	void FlushCachedState();

	void SetUnicodeMode(bool unicodeMode_);
	void SetDBCSMode(int codePage);

private:
	ScintillaWindowHandle *component;
	clan::Canvas canvas;
	clan::Color pen_color;
	clan::Point pen_pos;
};

class ScintillaFontHandle
{
public:
	ScintillaFontHandle(const char *faceName, int characterSet, int size, bool bold, bool italic, int extraFontFlag)
	: face_name(faceName), character_set(characterSet), size(size), bold(bold), italic(italic), extra_font_flag(extraFontFlag)
	{
	}

	clan::Font &get_font(clan::Canvas &canvas)
	{
		if (font.is_null())
		{
			clan::FontDescription desc;
			desc.set_typeface_name(face_name);
			desc.set_height(-size);
			desc.set_weight(bold ? 700 : 400);
			desc.set_italic(italic);
			font = clan::Font(canvas, desc);
		}
		return font;
	}

	static ScintillaFontHandle *from_handle(Scintilla::Font &font) { return static_cast<ScintillaFontHandle*>(font.GetID()); }

private:
	std::string face_name;
	int character_set;
	int size;
	bool bold;
	bool italic;
	int extra_font_flag;
	clan::Font font;
};
