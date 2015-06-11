
#include <Windows.h>
#include <CommCtrl.h>

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

class AnimationControl : public Control
{
protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return Animate_Create(parent, 0, WS_CHILD, instance); }
};

class ButtonControl : public Control
{
public:
	std::string text() const
	{
		wchar_t buffer[1024];
		GetWindowText(control_handle(), buffer, 1024);
		buffer[1023] = 0;
		return clan::StringHelp::ucs2_to_utf8(buffer);
	}

	void set_text(const std::string &text)
	{
		SetWindowText(control_handle(), clan::StringHelp::utf8_to_ucs2(text).c_str());
	}

	void set_default(bool enable = true)
	{
		// To do: change style flags
	}

protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return CreateWindowEx(0, L"BUTTON", L"", WS_CHILD, 0, 0, 0, 0, parent, 0, instance, 0); }
};

class ComboBoxControl : public Control
{
protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return CreateWindowEx(0, WC_COMBOBOX, L"", WS_CHILD, 0, 0, 0, 0, parent, 0, instance, 0); }
};

class ComboBoxExControl : public Control
{
protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override{ return CreateWindowEx(0, WC_COMBOBOXEX, L"", WS_CHILD, 0, 0, 0, 0, parent, 0, instance, 0); }
};

class DateTimeControl : public Control
{
protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return CreateWindowEx(0, DATETIMEPICK_CLASS, L"", WS_CHILD, 0, 0, 0, 0, parent, 0, instance, 0); }
};

class EditControl : public Control
{
protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return CreateWindowEx(0, L"EDIT", L"", WS_CHILD, 0, 0, 0, 0, parent, 0, instance, 0); }
};

class FlatScrollbarControl : public Control
{
public:
};

class HeaderControl : public Control
{
public:
};

class HotKeyControl : public Control
{
public:
};

class IpAddressControl : public Control
{
public:
};

class ListBoxControl : public Control
{
public:
};

class ListViewControl : public Control
{
public:
};

class MonthCalControl : public Control
{
protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return CreateWindowEx(0, MONTHCAL_CLASS, L"", WS_CHILD, 0, 0, 0, 0, parent, 0, instance, 0); }
};

class PagerControl : public Control
{
public:
};

class ProgressBarControl : public Control
{
public:
};

class PropertySheetControl : public Control
{
public:
};

class RebarControl : public Control
{
public:
};

class RichEditControl : public Control
{
public:
};

class ScrollBarControl : public Control
{
public:
};

class StaticControl : public Control
{
public:
};

class StatusBarControl : public Control
{
public:
};

class SysLinkControl : public Control
{
public:
};

class TabControl : public Control
{
public:
};

class ToolbarControl : public Control
{
public:
};

class TrackbarControl : public Control
{
public:
};

class TreeViewControl : public Control
{
public:
};

class UpDownControl : public Control
{
public:
};
