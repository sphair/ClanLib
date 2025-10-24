
#include "precomp.h"
#include "dlg_connect.h"
#include "mainframe.h"

/////////////////////////////////////////////////////////////////////////////
// DlgConnect construction:

DlgConnect::DlgConnect(MainFrame *mainframe) :
	CL_Window(mainframe), mainframe(mainframe), components(0)
{
	set_position(CL_Rect(CL_Point(mainframe->get_width()/2-200, mainframe->get_height()/2-150), CL_Size(400, 300)));
	set_title("Connect to IRC Server");

	components = new CL_ComponentManager("Dialogs/connect", get_style_manager()->get_resources(), get_client_area());
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

	inputbox_server->set_focus(); // bug in gui, should not be needed.

	inputbox_server->set_text("irc.freenode.net");
	inputbox_fullname->set_text("Anonymous CTalk User");
	inputbox_username->set_text("CTalk");	
	inputbox_password->set_text("");

	slots.connect(button_ok->sig_clicked(), this, &DlgConnect::on_button_ok_clicked);
	slots.connect(button_cancel->sig_clicked(), this, &DlgConnect::on_button_cancel_clicked);
}

DlgConnect::~DlgConnect()
{
	delete components;
}

/////////////////////////////////////////////////////////////////////////////
// DlgConnect attributes:

const std::string &DlgConnect::get_server()
{
	return inputbox_server->get_text();
}

const std::string &DlgConnect::get_nick()
{
	return inputbox_username->get_text();
}

const std::string &DlgConnect::get_username()
{
	return inputbox_username->get_text();
}

const std::string &DlgConnect::get_fullname()
{
	return inputbox_fullname->get_text();
}

const std::string &DlgConnect::get_password()
{
	return inputbox_password->get_text();
}

/////////////////////////////////////////////////////////////////////////////
// DlgConnect operations:


/////////////////////////////////////////////////////////////////////////////
// DlgConnect implementation:

void DlgConnect::on_button_ok_clicked()
{
	quit();
}

void DlgConnect::on_button_cancel_clicked()
{
	quit(); // todo: let quit() take an int as exit code.
}
