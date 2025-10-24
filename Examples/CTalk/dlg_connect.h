
#ifndef file_dlg_connect
#define file_dlg_connect

class MainFrame;

class DlgConnect : public CL_Window
{
//! Construction:
public:
	DlgConnect(MainFrame *mainframe);

	~DlgConnect();

//! Attributes:
public:
	const std::string &get_server();

	const std::string &get_nick();

	const std::string &get_username();

	const std::string &get_fullname();

	const std::string &get_password();

//! Operations:
public:

//! Implementation:
private:
	void on_button_ok_clicked();

	void on_button_cancel_clicked();

	MainFrame *mainframe;

	CL_ComponentManager *components;

	CL_Label *label_title;

	CL_Label *label_server;

	CL_Label *label_fullname;

	CL_Label *label_username;

	CL_Label *label_password;

	CL_InputBox *inputbox_server;

	CL_InputBox *inputbox_fullname;

	CL_InputBox *inputbox_username;

	CL_InputBox *inputbox_password;

	CL_Button *button_ok;

	CL_Button *button_cancel;

	CL_SlotContainer slots;
};

#endif
