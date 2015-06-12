
#include <Windows.h>
#include <CommCtrl.h>
#include <Richedit.h>
#include <windowsx.h>

class Control : public clan::View
{
public:
	Control();
	~Control();

	void layout_subviews(clan::Canvas &canvas) override;

protected:
	HWND control_handle() const;

	virtual HWND create_control(HWND parent, HINSTANCE instance) const = 0;

	float calculate_preferred_width(clan::Canvas &canvas) override;
	float calculate_preferred_height(clan::Canvas &canvas, float width) override;
	float calculate_first_baseline_offset(clan::Canvas &canvas, float width) override;
	float calculate_last_baseline_offset(clan::Canvas &canvas, float width) override;

private:
	mutable HWND handle = 0;
	HFONT font_handle = 0;
};

//class ImageList;
//class TaskDialog;
//class ToolTip;
//class PropertySheet;

class AnimationControl : public Control
{
public:
	enum class Style : int
	{
		autoplay = ACS_AUTOPLAY,
		center = ACS_CENTER,
		transparent = ACS_TRANSPARENT
	};

	void close() { Animate_Close(control_handle()); }
	bool is_playing() const { return Animate_IsPlaying(control_handle()) != FALSE; }
	bool open(const std::string &name) { return Animate_Open(control_handle(), clan::StringHelp::utf8_to_ucs2(name).c_str()) != FALSE; }
	bool open(const std::string &name, HINSTANCE instance) { return Animate_OpenEx(control_handle(), instance, clan::StringHelp::utf8_to_ucs2(name).c_str()) != FALSE; }
	bool open(int resource) { return Animate_Open(control_handle(), MAKEINTRESOURCE(resource)) != FALSE; }
	bool open(int resource, HINSTANCE instance) { return Animate_OpenEx(control_handle(), instance, MAKEINTRESOURCE(resource)) != FALSE; }
	bool play(int from, int to, int repeat = -1) { return Animate_Play(control_handle(), from, to, repeat) != FALSE; }
	bool seek(int frame) { return Animate_Seek(control_handle(), frame) != FALSE; }
	bool stop() { return Animate_Stop(control_handle()) != FALSE; }

	clan::Signal<void()> sig_start; // ACN_START
	clan::Signal<void()> sig_stop; // ACN_STOP

protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return Animate_Create(parent, 0, WS_CHILD, instance); }
};

class ButtonControl : public Control
{
public:
	enum class CheckState : int
	{
		checked = BST_CHECKED,
		indeterminate = BST_INDETERMINATE,
		unchecked = BST_UNCHECKED
	};

	enum class ImageAlignment : int
	{
		left = BUTTON_IMAGELIST_ALIGN_LEFT,
		right = BUTTON_IMAGELIST_ALIGN_RIGHT,
		top = BUTTON_IMAGELIST_ALIGN_TOP,
		bottom = BUTTON_IMAGELIST_ALIGN_BOTTOM,
		center = BUTTON_IMAGELIST_ALIGN_CENTER
	};

	struct SplitStyle
	{
		bool align_left = false;
		bool image = false;
		bool no_split = false;
		bool stretch = false;

		static SplitStyle from_mask(UINT mask)
		{
			SplitStyle style;
			style.align_left = (mask & BCSS_ALIGNLEFT) == BCSS_ALIGNLEFT;
			style.image = (mask & BCSS_IMAGE) == BCSS_IMAGE;
			style.no_split = (mask & BCSS_NOSPLIT) == BCSS_NOSPLIT;
			style.stretch = (mask & BCSS_STRETCH) == BCSS_STRETCH;
			return style;
		}

		UINT to_mask() const
		{
			UINT mask = 0;
			if (align_left) mask |= BCSS_ALIGNLEFT;
			if (image) mask |= BCSS_IMAGE;
			if (no_split) mask |= BCSS_NOSPLIT;
			if (stretch) mask |= BCSS_STRETCH;
			return mask;
		}
	};

	struct TextMargin
	{
		TextMargin() { }
		TextMargin(int left, int top, int right, int bottom) : left(left), top(top), right(right), bottom(bottom) { }

		int left = 0;
		int top = 0;
		int right = 0;
		int bottom = 0;
	};

	enum class Style : int
	{
		threestate = BS_3STATE,
		auto_3state = BS_AUTO3STATE,
		auto_checkbox = BS_AUTOCHECKBOX,
		auto_radio_button = BS_AUTORADIOBUTTON,
		bitmap = BS_BITMAP,
		bottom = BS_BOTTOM,
		center = BS_CENTER,
		checkbox = BS_CHECKBOX,
		commandlink = BS_COMMANDLINK,
		default_commandlink = BS_DEFCOMMANDLINK,
		default_push_button = BS_DEFPUSHBUTTON,
		default_split_button = BS_DEFSPLITBUTTON,
		groupbox = BS_GROUPBOX,
		icon = BS_ICON,
		flat = BS_FLAT,
		left = BS_LEFT,
		left_text = BS_LEFTTEXT,
		multiline = BS_MULTILINE,
		notify = BS_NOTIFY,
		push_button = BS_PUSHBUTTON,
		push_like = BS_PUSHLIKE,
		radio_button = BS_RADIOBUTTON,
		right = BS_RIGHT,
		right_button = BS_RIGHTBUTTON,
		split_button = BS_SPLITBUTTON,
		text = BS_TEXT,
		top = BS_TOP,
		vcenter = BS_VCENTER
	};

	CheckState get_check() const { return (CheckState)Button_GetCheck(control_handle()); }
	clan::Size get_ideal_size() const { SIZE size = { 0 }; Button_GetIdealSize(control_handle(), &size); return clan::Size(size.cx, size.cy); }
	BUTTON_IMAGELIST *get_image_list() const { BUTTON_IMAGELIST *image_list = nullptr; Button_GetImageList(control_handle(), &image_list); return image_list; }

	std::string get_note() const
	{
		wchar_t buffer[1024];
		int size = 1023;
		buffer[0] = 0;
		Button_GetNote(control_handle(), buffer, &size);
		buffer[1023] = 0;
		return clan::StringHelp::ucs2_to_utf8(buffer);
	}

	std::string get_text() const
	{
		wchar_t buffer[1024];
		buffer[0] = 0;
		GetWindowText(control_handle(), buffer, 1024);
		buffer[1023] = 0;
		return clan::StringHelp::ucs2_to_utf8(buffer);
	}

	HIMAGELIST get_split_glyph() { BUTTON_SPLITINFO info = { 0 }; info.mask = BCSIF_GLYPH; Button_GetSplitInfo(control_handle(), &info); return info.himlGlyph; }
	HIMAGELIST get_split_image() { BUTTON_SPLITINFO info = { 0 }; info.mask = BCSIF_IMAGE; Button_GetSplitInfo(control_handle(), &info); return info.himlGlyph; }
	SIZE get_split_size() { BUTTON_SPLITINFO info = { 0 }; info.mask = BCSIF_SIZE; Button_GetSplitInfo(control_handle(), &info); return info.size; }
	SplitStyle get_split_style() { BUTTON_SPLITINFO info = { 0 }; info.mask = BCSIF_STYLE; Button_GetSplitInfo(control_handle(), &info); return SplitStyle::from_mask(info.uSplitStyle); }

	bool is_checked() const { return (Button_GetState(control_handle()) & BST_CHECKED) == BST_CHECKED; }
	bool is_dropdown_pushed() const { return (Button_GetState(control_handle()) & BST_DROPDOWNPUSHED) == BST_DROPDOWNPUSHED; }
	bool is_focus() const { return (Button_GetState(control_handle()) & BST_FOCUS) == BST_FOCUS; }
	bool is_hot() const { return (Button_GetState(control_handle()) & BST_HOT) == BST_HOT; }
	bool is_indeterminate() const { return (Button_GetState(control_handle()) & BST_INDETERMINATE) == BST_INDETERMINATE; }
	bool is_pushed() const { return (Button_GetState(control_handle()) & BST_PUSHED) == BST_PUSHED; }
	bool is_unchecked() const { return (Button_GetState(control_handle()) & (BST_CHECKED|BST_INDETERMINATE)) == BST_UNCHECKED; }

	TextMargin get_text_margin() const { RECT rect = { 0 }; Button_GetTextMargin(control_handle(), &rect); return TextMargin(rect.left, rect.top, rect.right, rect.bottom); }

	void enable(bool enable) { Button_Enable(control_handle(), enable ? TRUE : FALSE); }
	void set_check(CheckState state) { Button_SetCheck(control_handle(), (int)state); }
	void set_dropdown(bool enable = true) { Button_SetDropDownState(control_handle(), enable ? TRUE : FALSE); }
	void set_elevation_required(bool enable = true) { Button_SetElevationRequiredState(control_handle(), enable ? TRUE : FALSE); }
	void set_image_list(BUTTON_IMAGELIST *image_list) { Button_SetImageList(control_handle(), image_list); }
	void set_note(const std::string &text) { Button_SetNote(control_handle(), clan::StringHelp::utf8_to_ucs2(text).c_str()); }
	void set_split_glyph(HIMAGELIST glyph) { BUTTON_SPLITINFO info = { 0 }; info.mask = BCSIF_GLYPH; info.himlGlyph = glyph; Button_SetSplitInfo(control_handle(), &info); }
	void set_split_image(HIMAGELIST image) { BUTTON_SPLITINFO info = { 0 }; info.mask = BCSIF_IMAGE; info.himlGlyph = image; Button_SetSplitInfo(control_handle(), &info); }
	void set_split_size(SIZE size) { BUTTON_SPLITINFO info = { 0 }; info.mask = BCSIF_SIZE; info.size = size; Button_SetSplitInfo(control_handle(), &info); }
	void set_split_style(SplitStyle style) { BUTTON_SPLITINFO info = { 0 }; info.mask = BCSIF_GLYPH; info.uSplitStyle = style.to_mask(); Button_SetSplitInfo(control_handle(), &info); }
	void set_highlighted(bool enable = true) { Button_SetState(control_handle(), enable ? TRUE : FALSE); }
	void set_style(Style style, bool redraw) { Button_SetStyle(control_handle(), (int)style, redraw ? TRUE : FALSE); }
	void set_text(const std::string &text) { SetWindowText(control_handle(), clan::StringHelp::utf8_to_ucs2(text).c_str()); }
	void set_margin(TextMargin margin) { RECT rect; rect.left = margin.left; rect.top = margin.top; rect.right = margin.right; rect.bottom = margin.bottom; Button_SetTextMargin(control_handle(), &rect); }

	clan::Signal<void()> sig_dropdown; // BCN_DROPDOWN
	clan::Signal<void()> sig_hot_item_change; // BCN_HOTITEMCHANGE
	clan::Signal<void()> sig_clicked; // BN_CLICKED
	clan::Signal<void()> sig_dblclk; // BN_DBLCLK
	clan::Signal<void()> sig_disable; // BN_DISABLE
	clan::Signal<void()> sig_double_clicked; // BN_DOUBLECLICKED
	clan::Signal<void()> sig_hilite; // BN_HILITE
	clan::Signal<void()> sig_kill_focus; // BN_KILLFOCUS
	clan::Signal<void()> sig_paint; // BN_PAINT
	clan::Signal<void()> sig_pushed; // BN_PUSHED
	clan::Signal<void()> sig_set_focus; // BN_SETFOCUS
	clan::Signal<void()> sig_unhilite; // BN_UNHILITE
	clan::Signal<void()> sig_unpushed; // BN_UNPUSHED

protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return CreateWindowEx(0, L"BUTTON", L"", WS_CHILD, 0, 0, 0, 0, parent, 0, instance, 0); }
};

class ComboBoxControl : public Control
{
public:
	enum class Style : int
	{
		simple = CBS_SIMPLE,
		dropdown = CBS_DROPDOWN,
		dropdown_list = CBS_DROPDOWNLIST, // simple | dropdown
		auto_hscroll = CBS_AUTOHSCROLL,
		disable_noscroll = CBS_DISABLENOSCROLL,
		lowercase = CBS_LOWERCASE,
		no_integral_height = CBS_NOINTEGRALHEIGHT,
		oem_convert = CBS_OEMCONVERT,
		sort = CBS_SORT,
		uppercase = CBS_UPPERCASE
	};

	int get_count() const { return ComboBox_GetCount(control_handle()); }
	int get_cur_sel() const { return ComboBox_GetCurSel(control_handle()); }
	clan::Rect get_dropped_control_rect() const { RECT rect = { 0 }; ComboBox_GetDroppedControlRect(control_handle(), &rect); return clan::Rect(rect.left, rect.top, rect.right, rect.bottom); }
	bool get_dropped_state() const { return ComboBox_GetDroppedState(control_handle()) != FALSE; }
	bool get_extended_ui() const { return ComboBox_GetExtendedUI(control_handle()) != FALSE; }
	void *get_item_data_ptr(int index) const { return (void*)ComboBox_GetItemData(control_handle(), index); }
	int get_item_data_int(int index) const { return ComboBox_GetItemData(control_handle(), index); }
	int get_item_height() const { return ComboBox_GetItemHeight(control_handle()); }
	int get_min_visible() const { return ComboBox_GetMinVisible(control_handle()); }

	std::string get_cue_banner_text() const
	{
		wchar_t buffer[1024];
		buffer[0] = 0;
		int length = 1023;
		ComboBox_GetCueBannerText(control_handle(), buffer, &length);
		buffer[1023] = 0;
		return clan::StringHelp::ucs2_to_utf8(buffer);
	}

	std::string get_lb_text() const
	{
		wchar_t buffer[1024];
		buffer[0] = 0;
		int length = 1023;
		ComboBox_GetLBText(control_handle(), buffer, &length);
		buffer[1023] = 0;
		return clan::StringHelp::ucs2_to_utf8(buffer);
	}

	std::string get_text() const
	{
		wchar_t buffer[1024];
		buffer[0] = 0;
		int length = 1023;
		ComboBox_GetCueBannerText(control_handle(), buffer, &length);
		buffer[1023] = 0;
		return clan::StringHelp::ucs2_to_utf8(buffer);
	}

	void add_item_data(void *data) { ComboBox_AddItemData(control_handle(), data); }
	void add_item_data(int data) { ComboBox_AddItemData(control_handle(), data); }
	void add_string(const std::string &text) { ComboBox_AddString(control_handle(), clan::StringHelp::utf8_to_ucs2(text).c_str()); }
	void delete_string(int index) { ComboBox_DeleteString(control_handle(), index); }
	void dir(unsigned int attrs, const std::string &file_spec) { ComboBox_Dir(control_handle(), attrs, clan::StringHelp::utf8_to_ucs2(file_spec).c_str()); }
	void enable(bool value = true) { ComboBox_Enable(control_handle(), value ? TRUE : FALSE); }
	int find_item_data(int index_start, void *data) { return ComboBox_FindItemData(control_handle(), index_start, data); }
	int find_item_data(int index_start, int data) { return ComboBox_FindItemData(control_handle(), index_start, data); }
	int find_string(int index_start, const std::string &text) { return ComboBox_FindString(control_handle(), index_start, clan::StringHelp::utf8_to_ucs2(text).c_str()); }
	int find_string_exact(int index_start, const std::string &text) { return ComboBox_FindStringExact(control_handle(), index_start, clan::StringHelp::utf8_to_ucs2(text).c_str()); }
	void insert_item_data(int index, void *data) { ComboBox_InsertItemData(control_handle(), index, data); }
	void insert_item_data(int index, int data) { ComboBox_InsertItemData(control_handle(), index, data); }
	void insert_string(int index, const std::string &text) { ComboBox_InsertString(control_handle(), index, clan::StringHelp::utf8_to_ucs2(text).c_str()); }
	void limit_text(int max_length) { ComboBox_LimitText(control_handle(), max_length); }
	void reset_content() { ComboBox_ResetContent(control_handle()); }
	void select_item_data(int index_start, void *data) { ComboBox_SelectItemData(control_handle(), index_start, data); }
	void select_item_data(int index_start, int data) { ComboBox_SelectItemData(control_handle(), index_start, data); }
	void select_string(int index_start, const std::string &text) { ComboBox_SelectString(control_handle(), index_start, clan::StringHelp::utf8_to_ucs2(text).c_str()); }
	void set_cue_banner_text(const std::string &text) { ComboBox_SetCueBannerText(control_handle(), clan::StringHelp::utf8_to_ucs2(text).c_str()); }
	void set_cur_sel(int index) { ComboBox_SetCurSel(control_handle(), index); }
	void set_extended_ui(bool enable = true) { ComboBox_SetExtendedUI(control_handle(), enable ? TRUE : FALSE); }
	void set_item_data(int index, void *data) { ComboBox_SetItemData(control_handle(), index, data); }
	void set_item_data(int index, int data) { ComboBox_SetItemData(control_handle(), index, data); }
	void set_item_height(int index, int item_height) { ComboBox_SetItemHeight(control_handle(), index, item_height); }
	void set_min_visible(int min_visible_count) { ComboBox_SetMinVisible(control_handle(), min_visible_count); }
	void set_text(const std::string &text) { ComboBox_SetText(control_handle(), clan::StringHelp::utf8_to_ucs2(text).c_str()); }
	void show_dropdown(bool show = true) { ComboBox_ShowDropdown(control_handle(), show ? TRUE : FALSE); }

	clan::Signal<void()> sig_closeup; // CBN_CLOSEUP
	clan::Signal<void()> sig_dblclk; // CBN_DBLCLK
	clan::Signal<void()> sig_dropdown; // CBN_DROPDOWN
	clan::Signal<void()> sig_editchange; // CBN_EDITCHANGE
	clan::Signal<void()> sig_editupdate; // CBN_EDITUPDATE
	clan::Signal<void()> sig_errspace; // CBN_ERRSPACE
	clan::Signal<void()> sig_killfocus; // CBN_KILLFOCUS
	clan::Signal<void()> sig_selchange; // CBN_SELCHANGE
	clan::Signal<void()> sig_selendcancel; // CBN_SELENDCANCEL
	clan::Signal<void()> sig_selendok; // CBN_SELENDOK
	clan::Signal<void()> sig_setfocus; // CBN_SETFOCUS
	clan::Signal<void()> sig_compareitem; // WM_COMPAREITEM

protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return CreateWindowEx(0, WC_COMBOBOX, L"", WS_CHILD, 0, 0, 0, 0, parent, 0, instance, 0); }
};

class ComboBoxExControl : public Control
{
	// This control is actually implemented as a ComboBox control using ownerdrawn to draw images too

protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override{ return CreateWindowEx(0, WC_COMBOBOXEX, L"", WS_CHILD, 0, 0, 0, 0, parent, 0, instance, 0); }
};

class MonthCalControl : public Control
{
public:
	enum class Style : int
	{
		day_state = MCS_DAYSTATE,
		multi_select = MCS_MULTISELECT,
		week_numbers = MCS_WEEKNUMBERS,
		no_today_circle = MCS_NOTODAYCIRCLE,
		no_today = MCS_NOTODAY,
		no_trailing_dates = MCS_NOTRAILINGDATES,
		short_day_of_week = MCS_SHORTDAYSOFWEEK,
		no_selchange_on_nav = MCS_NOSELCHANGEONNAV
	};

	enum class ViewType : int
	{
		month = MCMV_MONTH,
		year = MCMV_YEAR,
		decade = MCMV_DECADE,
		century = MCMV_CENTURY
	};

	clan::Rect size_rect_to_min(clan::Rect box) { RECT rect; rect.left = box.left; rect.top = box.top; rect.right = box.right; rect.bottom = box.bottom; MonthCal_SizeRectToMin(control_handle(), &rect); return clan::Rect(rect.left, rect.top, rect.right, rect.bottom); }

	int get_calendar_border() const { return MonthCal_GetCalendarBorder(control_handle()); }
	int get_calendar_count() const { return MonthCal_GetCalendarCount(control_handle()); }
	MCGRIDINFO get_calendar_grid_info() const { MCGRIDINFO info = { 0 }; MonthCal_GetCalendarGridInfo(control_handle(), &info); return info; }
	CALID get_calid() const { return MonthCal_GetCALID(control_handle()); }
	COLORREF get_background_color() { return MonthCal_GetColor(control_handle(), MCSC_BACKGROUND); }
	COLORREF get_month_background_color() { return MonthCal_GetColor(control_handle(), MCSC_MONTHBK); }
	COLORREF get_text_color() { return MonthCal_GetColor(control_handle(), MCSC_TEXT); }
	COLORREF get_title_background_color() { return MonthCal_GetColor(control_handle(), MCSC_TITLEBK); }
	COLORREF get_title_text_color() { return MonthCal_GetColor(control_handle(), MCSC_TITLETEXT); }
	COLORREF get_trailing_text_color() { return MonthCal_GetColor(control_handle(), MCSC_TRAILINGTEXT); }
	ViewType get_current_view() const { return (ViewType)MonthCal_GetCurrentView(control_handle()); }
	SYSTEMTIME get_cur_sel() const { SYSTEMTIME sel = { 0 }; MonthCal_GetCurSel(control_handle(), &sel); return sel; }
	int get_first_day_of_week() const { return MonthCal_GetFirstDayOfWeek(control_handle()); }
	int get_max_sel_count() const { return MonthCal_GetMaxSelCount(control_handle()); }
	int get_max_today_width() const { return MonthCal_GetMaxTodayWidth(control_handle()); }
	clan::Rect get_min_req_rect() const { RECT rect = { 0 }; MonthCal_GetMinReqRect(control_handle(), &rect); return clan::Rect(rect.left, rect.top, rect.right, rect.bottom); }
	int get_month_delta() const { return MonthCal_GetMonthDelta(control_handle()); }
	int get_month_range_daystate(SYSTEMTIME &out_range_min, SYSTEMTIME &out_range_max) const { SYSTEMTIME times[2] = { 0 }; int span = MonthCal_GetMonthRange(control_handle(), GMR_DAYSTATE, &times); out_range_min = times[0]; out_range_max = times[1]; return span; }
	int get_month_range_visible(SYSTEMTIME &out_range_min, SYSTEMTIME &out_range_max) const { SYSTEMTIME times[2] = { 0 }; int span = MonthCal_GetMonthRange(control_handle(), GMR_VISIBLE, &times); out_range_min = times[0]; out_range_max = times[1]; return span; }
	void get_range(SYSTEMTIME &out_range_min, SYSTEMTIME &out_range_max) const { SYSTEMTIME times[2] = { 0 }; MonthCal_GetRange(control_handle(), times); out_range_min = times[0]; out_range_max = times[1]; }
	void get_sel_range(SYSTEMTIME &out_range_min, SYSTEMTIME &out_range_max) const { SYSTEMTIME times[2] = { 0 }; MonthCal_GetSelRange(control_handle(), times); out_range_min = times[0]; out_range_max = times[1]; }
	SYSTEMTIME get_today() const { SYSTEMTIME today = { 0 }; MonthCal_GetToday(control_handle(), &today); return today; }

	DWORD hit_test(MCHITTESTINFO *hittest) { return MonthCal_HitTest(control_handle(), hittest); }

	void set_calendar_border(int pixels) { MonthCal_SetCalendarBorder(control_handle(), TRUE, pixels); }
	void reset_calendar_border() { MonthCal_SetCalendarBorder(control_handle(), FALSE, 0); }
	void set_calid(CALID calid) { MonthCal_SetCALID(control_handle(), calid); }
	void set_background_color(clan::Color color) { MonthCal_SetColor(control_handle(), MCSC_BACKGROUND, RGB(color.r, color.g, color.b)); }
	void set_month_background_color(clan::Color color) { MonthCal_SetColor(control_handle(), MCSC_MONTHBK, RGB(color.r, color.g, color.b)); }
	void set_text_color(clan::Color color) { MonthCal_SetColor(control_handle(), MCSC_TEXT, RGB(color.r, color.g, color.b)); }
	void set_title_background_color(clan::Color color) { MonthCal_SetColor(control_handle(), MCSC_TITLEBK, RGB(color.r, color.g, color.b)); }
	void set_title_text_color(clan::Color color) { MonthCal_SetColor(control_handle(), MCSC_TITLETEXT, RGB(color.r, color.g, color.b)); }
	void set_trailing_text_color(clan::Color color) { MonthCal_SetColor(control_handle(), MCSC_TRAILINGTEXT, RGB(color.r, color.g, color.b)); }
	void set_current_view(ViewType type) { MonthCal_SetCurrentView(control_handle(), (int)type); }
	void set_cur_sel(SYSTEMTIME systime) { MonthCal_SetCurSel(control_handle(), &systime); }
	void set_day_state(const std::vector<MONTHDAYSTATE> &states) { MonthCal_SetDayState(control_handle(), states.size(), states.data()); }
	void set_first_day_of_week(int day) { MonthCal_SetFirstDayOfWeek(control_handle(), day); }
	void set_max_sel_count(int count) { MonthCal_SetMaxSelCount(control_handle(), count); }
	void set_month_delta(int delta) { MonthCal_SetMonthDelta(control_handle(), delta); }
	void set_range(SYSTEMTIME min_value, SYSTEMTIME max_value) { SYSTEMTIME times[2] = { min_value, max_value }; MonthCal_SetRange(control_handle(), GDTR_MIN | GDTR_MAX, times); }
	void set_range_min(SYSTEMTIME min_value) { SYSTEMTIME times[2]; times[0] = min_value; MonthCal_SetRange(control_handle(), GDTR_MIN, times); }
	void set_range_max(SYSTEMTIME max_value) { SYSTEMTIME times[2]; times[1] = max_value; MonthCal_SetRange(control_handle(), GDTR_MAX, times); }
	void clear_range() { SYSTEMTIME times[2]; MonthCal_SetRange(control_handle(), 0, times); }
	void set_sel_range(SYSTEMTIME min_value, SYSTEMTIME max_value) { SYSTEMTIME times[2] = { min_value, max_value }; MonthCal_SetSelRange(control_handle(), times); }
	void set_today(SYSTEMTIME today) { MonthCal_SetToday(control_handle(), &today); }
	void set_today_default() { MonthCal_SetToday(control_handle(), NULL); }

	clan::Signal<void()> sig_getdaystate; // MCN_GETDAYSTATE
	clan::Signal<void()> sig_selchange; // MCN_SELCHANGE
	clan::Signal<void()> sig_select; // MCN_SELECT
	clan::Signal<void()> sig_viewchange; // MCN_VIEWCHANGE
	clan::Signal<void()> sig_released_capture; // NM_RELEASEDCAPTURE

protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return CreateWindowEx(0, MONTHCAL_CLASS, L"", WS_CHILD, 0, 0, 0, 0, parent, 0, instance, 0); }
};

class DateTimeControl : public Control
{
public:
	enum class Style : int
	{
		app_can_parse = DTS_APPCANPARSE,
		long_date_format = DTS_LONGDATEFORMAT,
		right_align = DTS_RIGHTALIGN,
		show_none = DTS_SHOWNONE,
		short_date_format = DTS_SHORTDATEFORMAT,
		short_date_century_format = DTS_SHORTDATECENTURYFORMAT,
		time_format = DTS_TIMEFORMAT,
		updown = DTS_UPDOWN
	};

	clan::Size get_ideal_size() const { SIZE size = { 0 }; DateTime_GetIdealSize(control_handle(), &size); return clan::Size(size.cx, size.cy); }

	DATETIMEPICKERINFO get_date_time_picker_info() const { DATETIMEPICKERINFO info = { 0 }; info.cbSize = sizeof(DATETIMEPICKERINFO); DateTime_GetDateTimePickerInfo(control_handle(), &info); return info; }

	// Only valid between DTN_DROPDOWN and DTN_CLOSEUP notifications
	HWND get_month_cal() const { DateTime_GetMonthCal(control_handle()); }

	COLORREF get_month_cal_background_color() { return DateTime_GetMonthCalColor(control_handle(), MCSC_BACKGROUND); }
	COLORREF get_month_cal_month_background_color() { return DateTime_GetMonthCalColor(control_handle(), MCSC_MONTHBK); }
	COLORREF get_month_cal_text_color() { return DateTime_GetMonthCalColor(control_handle(), MCSC_TEXT); }
	COLORREF get_month_cal_title_background_color() { return DateTime_GetMonthCalColor(control_handle(), MCSC_TITLEBK); }
	COLORREF get_month_cal_title_text_color() { return DateTime_GetMonthCalColor(control_handle(), MCSC_TITLETEXT); }
	COLORREF get_month_cal_trailing_text_color() { return DateTime_GetMonthCalColor(control_handle(), MCSC_TRAILINGTEXT); }
	HFONT get_month_cal_font() const { return (HFONT)DateTime_GetMonthCalFont(control_handle()); }
	MonthCalControl::Style get_month_cal_style() const { return (MonthCalControl::Style)DateTime_GetMonthCalStyle(control_handle()); }

	SYSTEMTIME get_range_min() const { SYSTEMTIME times[2];  DateTime_GetRange(control_handle(), times); return times[0]; }
	SYSTEMTIME get_range_max() const { SYSTEMTIME times[2];  DateTime_GetRange(control_handle(), times); return times[1]; }
	SYSTEMTIME get_systemtime() const { SYSTEMTIME value = { 0 }; DateTime_GetSystemtime(control_handle(), &value); return value; }

	void close_month_cal() { DateTime_CloseMonthCal(control_handle()); }
	void set_format(const std::string &format) { DateTime_SetFormat(control_handle(), clan::StringHelp::utf8_to_ucs2(format).c_str()); }
	void set_month_cal_background_color(clan::Color color) { DateTime_SetMonthCalColor(control_handle(), MCSC_BACKGROUND, RGB(color.r, color.g, color.b)); }
	void set_month_cal_month_background_color(clan::Color color) { DateTime_SetMonthCalColor(control_handle(), MCSC_MONTHBK, RGB(color.r, color.g, color.b)); }
	void set_month_cal_text_color(clan::Color color) { DateTime_SetMonthCalColor(control_handle(), MCSC_TEXT, RGB(color.r, color.g, color.b)); }
	void set_month_cal_title_background_color(clan::Color color) { DateTime_SetMonthCalColor(control_handle(), MCSC_TITLEBK, RGB(color.r, color.g, color.b)); }
	void set_month_cal_title_text_color(clan::Color color) { DateTime_SetMonthCalColor(control_handle(), MCSC_TITLETEXT, RGB(color.r, color.g, color.b)); }
	void set_month_cal_trailing_text_color(clan::Color color) { DateTime_SetMonthCalColor(control_handle(), MCSC_TRAILINGTEXT, RGB(color.r, color.g, color.b)); }
	void set_month_cal_font(HFONT font, bool redraw) { DateTime_SetMonthCalFont(control_handle(), font, redraw ? TRUE : FALSE); }
	void set_month_cal_style(MonthCalControl::Style style) { DateTime_SetMonthCalStyle(control_handle(), (int)style); }
	void set_range(SYSTEMTIME min_value, SYSTEMTIME max_value) { SYSTEMTIME times[2] = { min_value, max_value }; DateTime_SetRange(control_handle(), GDTR_MIN | GDTR_MAX, times); }
	void set_range_min(SYSTEMTIME min_value) { SYSTEMTIME times[2]; times[0] = min_value; DateTime_SetRange(control_handle(), GDTR_MIN, times); }
	void set_range_max(SYSTEMTIME max_value) { SYSTEMTIME times[2]; times[1] = max_value; DateTime_SetRange(control_handle(), GDTR_MAX, times); }
	void clear_range() { SYSTEMTIME times[2]; DateTime_SetRange(control_handle(), 0, times); }
	void set_systemtime(SYSTEMTIME value) { DateTime_SetSystemtime(control_handle(), GDT_VALID, &value); }
	void set_systemtime_nodate() { DateTime_SetSystemtime(control_handle(), GDT_NONE, 0); }

	clan::Signal<void()> sig_closeup; // DTN_CLOSEUP
	clan::Signal<void()> sig_datetimechange; // DTN_DATETIMECHANGE
	clan::Signal<void()> sig_dropdown; // DTN_DROPDOWN
	clan::Signal<void()> sig_format; // DTN_FORMAT
	clan::Signal<void()> sig_formatquery; // DTN_FORMATQUERY
	clan::Signal<void()> sig_userstring; // DTN_USERSTRING
	clan::Signal<void()> sig_wmkeydown; // DTN_WMKEYDOWN
	clan::Signal<void()> sig_killfocus; // NM_KILLFOCUS
	clan::Signal<void()> sig_setfocus; // NM_SETFOCUS

protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return CreateWindowEx(0, DATETIMEPICK_CLASS, L"", WS_CHILD, 0, 0, 0, 0, parent, 0, instance, 0); }
};

class EditControl : public Control
{
public:
	enum class Style : int
	{
		auto_hscroll = ES_AUTOHSCROLL,
		auto_vscroll = ES_AUTOVSCROLL,
		center = ES_CENTER,
		left = ES_LEFT,
		lowercase = ES_LOWERCASE,
		multiline = ES_MULTILINE,
		no_hidesel = ES_NOHIDESEL,
		number = ES_NUMBER,
		oem_convert = ES_OEMCONVERT,
		password = ES_PASSWORD,
		readonly = ES_READONLY,
		right = ES_RIGHT,
		uppercase = ES_UPPERCASE,
		wantreturn = ES_WANTRETURN
	};

	enum class BalloonTipIcon : int
	{
		error = TTI_ERROR,
		info = TTI_INFO,
		none = TTI_NONE,
		warning = TTI_WARNING,
		info_large = TTI_INFO_LARGE,
		warning_large = TTI_WARNING_LARGE,
		error_large = TTI_ERROR_LARGE
	};

	bool can_undo() const { return Edit_CanUndo(control_handle()) != FALSE; }
	int get_first_visible_line() const { return Edit_GetFirstVisibleLine(control_handle()); }
	HLOCAL get_handle() const { return Edit_GetHandle(control_handle()); }
	int get_line_count() const { return Edit_GetLineCount(control_handle()); }
	bool get_modify() const { return Edit_GetModify(control_handle()) != FALSE; }
	int get_password_char() const { return Edit_GetPasswordChar(control_handle()); }
	clan::Rect get_formatting_rect() const { RECT rect = { 0 }; Edit_GetRect(control_handle(), &rect); return clan::Rect(rect.left, rect.top, rect.right, rect.bottom); }
	void get_selection(int &out_start, int &out_end) const { DWORD sel = Edit_GetSel(control_handle()); out_start = LOWORD(sel); out_end = HIWORD(sel); }
	int get_text_length() const { return Edit_GetTextLength(control_handle()); }
	EDITWORDBREAKPROC get_wordbreak_proc() const { return Edit_GetWordBreakProc(control_handle()); }

	std::string get_text() const
	{
		std::vector<wchar_t> buffer;
		buffer.resize(get_text_length() + 1);
		int length = Edit_GetText(control_handle(), buffer.data(), buffer.size());
		buffer.resize(length);
		buffer.push_back(0);
		return clan::StringHelp::ucs2_to_utf8(buffer.data());
	}

	std::string get_cue_banner_text() const
	{
		std::vector<wchar_t> buffer;
		buffer.resize(1024);
		int length = Edit_GetCueBannerText(control_handle(), buffer.data(), buffer.size());
		buffer.resize(length);
		buffer.push_back(0);
		return clan::StringHelp::ucs2_to_utf8(buffer.data());
	}

	std::string get_line_text(int line) const
	{
		std::vector<wchar_t> buffer;
		buffer.resize(get_text_length() + 1);
		int length = Edit_GetLine(control_handle(), line, buffer.data(), buffer.size());
		buffer.resize(length);
		buffer.push_back(0);
		return clan::StringHelp::ucs2_to_utf8(buffer.data());
	}

	int line_from_char(int char_index) const { return Edit_LineFromChar(control_handle(), char_index); }
	int line_index(int line) const { return Edit_LineIndex(control_handle(), line); }
	int line_length(int line) const { return Edit_LineLength(control_handle(), line); }

	void empty_undo_buffer() { Edit_EmptyUndoBuffer(control_handle()); }
	void enable(bool value = true) { Edit_Enable(control_handle(), value ? TRUE : FALSE); }
	void format_lines(bool add_eol) { Edit_FmtLines(control_handle(), add_eol ? TRUE : FALSE); }
	void hide_balloon_tip() { Edit_HideBalloonTip(control_handle()); }
	void limit_text(int count) { Edit_LimitText(control_handle(), count); }
	void no_set_focus() { Edit_NoSetFocus(control_handle()); }
	void replace_selection(const std::string &text) { Edit_ReplaceSel(control_handle(), clan::StringHelp::utf8_to_ucs2(text).c_str()); }
	void scroll(int delta_row, int delta_column) { Edit_Scroll(control_handle(), delta_row, delta_column); }
	void scroll_caret() { Edit_ScrollCaret(control_handle()); }
	void set_cue_banner_text(const std::string &text) { Edit_SetCueBannerText(control_handle(), clan::StringHelp::utf8_to_ucs2(text).c_str()); }
	void set_cue_banner_text(const std::string &text, bool draw_focused) { Edit_SetCueBannerTextFocused(control_handle(), clan::StringHelp::utf8_to_ucs2(text).c_str(), draw_focused ? TRUE : FALSE); }
	void set_handle(HLOCAL handle) { Edit_SetHandle(control_handle(), handle); }
	void set_modify(bool flag) { Edit_SetModify(control_handle(), flag ? TRUE : FALSE); }
	void set_password_char(int c) { Edit_SetPasswordChar(control_handle(), c); }
	void set_readonly(bool enable) { Edit_SetReadOnly(control_handle(), enable ? TRUE : FALSE); }
	void set_formatting_rect(const clan::Rect &rect) { RECT r; r.left = rect.left; r.top = rect.top; r.right = rect.right; r.bottom = rect.bottom; Edit_SetRect(control_handle(), &r); }
	void set_formatting_rect_no_paint(const clan::Rect &rect) { RECT r; r.left = rect.left; r.top = rect.top; r.right = rect.right; r.bottom = rect.bottom; Edit_SetRectNoPaint(control_handle(), &r); }
	void set_selection(int start, int end) { Edit_SetSel(control_handle(), start, end); }
	void set_tab_stops(const std::vector<int> &stops) { Edit_SetTabStops(control_handle(), stops.size(), stops.data()); }
	void set_text(const std::string &text) { Edit_SetText(control_handle(), clan::StringHelp::utf8_to_ucs2(text).c_str()); }
	void set_wordbreak_proc(EDITWORDBREAKPROC proc) { Edit_SetWordBreakProc(control_handle(), proc); }
	void show_balloon_tip(const std::string &title, const std::string &text, BalloonTipIcon icon) { auto title_utf16 = clan::StringHelp::utf8_to_ucs2(title); auto text_utf16 = clan::StringHelp::utf8_to_ucs2(text); EDITBALLOONTIP tip = { 0 }; tip.cbStruct = sizeof(EDITBALLOONTIP); tip.pszTitle = title_utf16.c_str(), tip.pszText = text_utf16.c_str(); tip.ttiIcon = (int)icon; Edit_ShowBalloonTip(control_handle(), &tip); }
	void take_focus() { Edit_TakeFocus(control_handle()); }
	void undo() { Edit_Undo(control_handle()); }

	clan::Signal<void()> sig_align_ltr_ec; // EN_ALIGN_LTR_EC
	clan::Signal<void()> sig_align_rtl_ec; // EN_ALIGN_RTL_EC
	clan::Signal<void()> sig_change; // EN_CHANGE
	clan::Signal<void()> sig_errspace; // EN_ERRSPACE
	clan::Signal<void()> sig_hscroll; // EN_HSCROLL
	clan::Signal<void()> sig_killfocus; // EN_KILLFOCUS
	clan::Signal<void()> sig_maxtext; // EN_MAXTEXT
	clan::Signal<void()> sig_setfocus; // EN_SETFOCUS
	clan::Signal<void()> sig_update; // EN_UPDATE
	clan::Signal<void()> sig_vscroll; // EN_VSCROLL
	clan::Signal<void()> sig_coloredit; // WM_CTLCOLOREDIT

protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return CreateWindowEx(0, L"EDIT", L"", WS_CHILD, 0, 0, 0, 0, parent, 0, instance, 0); }
};

class HeaderControl : public Control
{
protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return CreateWindowEx(0, WC_HEADER, L"", WS_CHILD, 0, 0, 0, 0, parent, 0, instance, 0); }
};

class HotKeyControl : public Control
{
protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return CreateWindowEx(0, HOTKEY_CLASS, L"", WS_CHILD, 0, 0, 0, 0, parent, 0, instance, 0); }
};

class IpAddressControl : public Control
{
protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return CreateWindowEx(0, WC_IPADDRESS, L"", WS_CHILD, 0, 0, 0, 0, parent, 0, instance, 0); }
};

class ListBoxControl : public Control
{
protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return CreateWindowEx(0, WC_LISTBOX, L"", WS_CHILD, 0, 0, 0, 0, parent, 0, instance, 0); }
};

class ListViewControl : public Control
{
protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return CreateWindowEx(0, WC_LISTVIEW, L"", WS_CHILD, 0, 0, 0, 0, parent, 0, instance, 0); }
};

class PagerControl : public Control
{
protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return CreateWindowEx(0, WC_PAGESCROLLER, L"", WS_CHILD, 0, 0, 0, 0, parent, 0, instance, 0); }
};

class ProgressBarControl : public Control
{
protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return CreateWindowEx(0, PROGRESS_CLASS, L"", WS_CHILD, 0, 0, 0, 0, parent, 0, instance, 0); }
};

class RebarControl : public Control
{
protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return CreateWindowEx(WS_EX_TOOLWINDOW, REBARCLASSNAME, L"", WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 0, 0, parent, 0, instance, 0); }
};

class RichEditControl : public Control
{
protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override
	{
		LoadLibrary(L"Msftedit.dll");
		return CreateWindowEx(0, MSFTEDIT_CLASS, L"", WS_CHILD, 0, 0, 0, 0, parent, 0, instance, 0);
	}
};

class ScrollBarControl : public Control
{
protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return CreateWindowEx(0, L"SCROLLBAR", L"", WS_CHILD, 0, 0, 0, 0, parent, 0, instance, 0); }
};

class StaticControl : public Control
{
protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return CreateWindowEx(0, L"STATIC", L"", WS_CHILD, 0, 0, 0, 0, parent, 0, instance, 0); }
};

class StatusBarControl : public Control
{
public:
	void set_simple(bool enable = true) { SendMessage(control_handle(), SB_SIMPLE, enable ? TRUE : FALSE, 0); }
	void set_parts(const std::vector<int> parts) { SendMessage(control_handle(), SB_SETPARTS, (WPARAM)parts.size(), (LPARAM)parts.data()); }
	void set_text(const std::string &text) { SendMessage(control_handle(), SB_SETTEXT, SB_SIMPLEID, (LPARAM)clan::StringHelp::utf8_to_ucs2(text).c_str()); }
	void set_text(int id, const std::string &text) { SendMessage(control_handle(), SB_SETTEXT, LOWORD(id), (LPARAM)clan::StringHelp::utf8_to_ucs2(text).c_str()); }

protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return CreateWindowEx(0, STATUSCLASSNAME, L"", WS_CHILD | SBARS_SIZEGRIP, 0, 0, 0, 0, parent, 0, instance, 0); }
};

class SysLinkControl : public Control
{
protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return CreateWindowEx(0, WC_LINK, L"", WS_CHILD, 0, 0, 0, 0, parent, 0, instance, 0); }
};

class TabControl : public Control
{
protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return CreateWindowEx(0, WC_TABCONTROL, L"", WS_CHILD, 0, 0, 0, 0, parent, 0, instance, 0); }
};

class ToolbarControl : public Control
{
protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return CreateWindowEx(0, TOOLBARCLASSNAME, L"", WS_CHILD, 0, 0, 0, 0, parent, 0, instance, 0); }
};

class TrackbarControl : public Control
{
protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return CreateWindowEx(0, TRACKBAR_CLASS, L"", WS_CHILD, 0, 0, 0, 0, parent, 0, instance, 0); }
};

class TreeViewControl : public Control
{
protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return CreateWindowEx(0, WC_TREEVIEW, L"", WS_CHILD, 0, 0, 0, 0, parent, 0, instance, 0); }
};

class UpDownControl : public Control
{
protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return CreateWindowEx(0, UPDOWN_CLASS, L"", WS_CHILD, 0, 0, 0, 0, parent, 0, instance, 0); }
};
