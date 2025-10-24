
#include "precomp.h"
#include "dlg_connect.h"
#include "mainframe.h"

/////////////////////////////////////////////////////////////////////////////
// DlgConnect construction:

DlgConnect::DlgConnect(MainFrame *mainframe)
: CL_Window(get_initial_geometry(mainframe), mainframe, get_window_description()), mainframe(mainframe)
{
/*
	components = new CL_GUIComponentManager("Dialogs/connect", get_style_manager()->get_resources(), get_client_area());
	components->get_component("label_title", &label_title);
	components->get_component("label_server", &label_server);
	components->get_component("label_fullname", &label_fullname);
	components->get_component("label_username", &label_username);
	components->get_component("label_password", &label_password);
	components->get_component("inputbox_server", &inputbox_server);
	components->get_component("inputbox_fullname", &inputbox_fullname);
	components->get_component("inputbox_username", &inputbox_username);
	components->get_component("inputbox_password", &inputbox_password);
	components->get_component("button_ok", &button_ok);
	components->get_component("button_cancel", &button_cancel);
*/
	inputbox_server->set_focus(); // bug in gui, should not be needed.

	inputbox_server->set_text("irc.freenode.net");
	inputbox_fullname->set_text("Anonymous CTalk User");
	inputbox_username->set_text("CTalk");	
	inputbox_password->set_text("");

	button_ok->func_clicked().set(this, &DlgConnect::on_button_ok_clicked);
	button_cancel->func_clicked().set(this, &DlgConnect::on_button_cancel_clicked);
}

DlgConnect::~DlgConnect()
{
}

CL_Rect DlgConnect::get_initial_geometry(MainFrame *mainframe)
{
	return CL_Rect(CL_Point(mainframe->get_width()/2-200, mainframe->get_height()/2-150), CL_Size(400, 300));
}

CL_GUITopLevelDescription DlgConnect::get_window_description()
{
	CL_GUITopLevelDescription desc;
	desc.set_title("Connect to IRC Server");
	return desc;
}

/////////////////////////////////////////////////////////////////////////////
// DlgConnect attributes:

CL_String DlgConnect::get_server()
{
	return inputbox_server->get_text();
}

CL_String DlgConnect::get_nick()
{
	return inputbox_username->get_text();
}

CL_String DlgConnect::get_username()
{
	return inputbox_username->get_text();
}

CL_String DlgConnect::get_fullname()
{
	return inputbox_fullname->get_text();
}

CL_String DlgConnect::get_password()
{
	return inputbox_password->get_text();
}

/////////////////////////////////////////////////////////////////////////////
// DlgConnect operations:


/////////////////////////////////////////////////////////////////////////////
// DlgConnect implementation:

void DlgConnect::on_button_ok_clicked()
{
	exit_with_code(1);
}

void DlgConnect::on_button_cancel_clicked()
{
	exit_with_code(0);
}
