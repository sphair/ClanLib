
#include <Windows.h>
#include <CommCtrl.h>
#include <Richedit.h>

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

class MonthCalControl : public Control
{
protected:
	HWND create_control(HWND parent, HINSTANCE instance) const override { return CreateWindowEx(0, MONTHCAL_CLASS, L"", WS_CHILD, 0, 0, 0, 0, parent, 0, instance, 0); }
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
