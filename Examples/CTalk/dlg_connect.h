
#pragma once

class MainFrame;

class DlgConnect : public CL_Window
{
//! Construction:
public:
	DlgConnect(MainFrame *mainframe);

	~DlgConnect();

//! Attributes:
public:
	CL_String get_server();
	CL_String get_nick();
	CL_String get_username();
	CL_String get_fullname();
	CL_String get_password();

//! Operations:
public:

//! Implementation:
private:
	void on_button_ok_clicked();
	void on_button_cancel_clicked();

	static CL_Rect get_initial_geometry(MainFrame *mainframe);
	static CL_GUITopLevelDescription get_window_description();

	MainFrame *mainframe;
	CL_Label *label_title;
	CL_Label *label_server;
	CL_Label *label_fullname;
	CL_Label *label_username;
	CL_Label *label_password;

	CL_LineEdit *inputbox_server;
	CL_LineEdit *inputbox_fullname;
	CL_LineEdit *inputbox_username;
	CL_LineEdit *inputbox_password;

	CL_PushButton *button_ok;
	CL_PushButton *button_cancel;

	CL_SlotContainer slots;
};
