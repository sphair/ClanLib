
#include "precomp.h"
#include "login_view.h"

LoginView::LoginView(CL_GUIManager *gui_manager)
: CL_GUIComponent(gui_manager, get_toplevel_description())
{
	set_class_name("login");

	label_info = new CL_Label(this);
	label_info->set_geometry(CL_RectPS(15, 10, 370, 27));
	label_info->set_class_name("header");
	label_info->set_text("Enter connection information");

	label_player_nick = new CL_Label(this);
	label_player_nick->set_geometry(CL_RectPS(15, 50, 370, 27));
	label_player_nick->set_text("Player nick:");

	edit_player_nick = new CL_LineEdit(this);
	edit_player_nick->set_geometry(CL_RectPS(15, 77, 370, 23));
	edit_player_nick->func_after_edit_changed().set(this, &LoginView::on_edit_player_nick_changed);
	#ifdef _DEBUG
	edit_player_nick->set_text("Player");
	#endif

	label_server = new CL_Label(this);
	label_server->set_geometry(CL_RectPS(15, 120, 370, 27));
	label_server->set_text("Server information:");

	edit_server = new CL_LineEdit(this);
	edit_server->set_geometry(CL_RectPS(15, 147, 300, 23));
	edit_server->set_text("localhost");

	edit_port = new CL_LineEdit(this);
	edit_port->set_geometry(CL_RectPS(320, 147, 65, 23));
	edit_port->set_text("4558");

	button_ok = new CL_PushButton(this);
	button_ok->set_geometry(CL_RectPS(15, 205, 75, 23));
	button_ok->set_text("OK");
	button_ok->func_clicked().set(this, &LoginView::on_button_ok_clicked);
	update_button_states();

	button_cancel = new CL_PushButton(this);
	button_cancel->set_geometry(CL_RectPS(245, 205, 75, 23));
	button_cancel->set_text("Cancel");
	button_cancel->func_clicked().set(this, &LoginView::on_button_cancel_clicked);
}

LoginView::~LoginView()
{
}

CL_GUITopLevelDescription LoginView::get_toplevel_description()
{
	CL_GUITopLevelDescription desc;
	desc.set_title("Connect to Server");
	desc.set_position(CL_RectPS(312, 100, 400, 260), false);
	return desc;
}

void LoginView::on_button_ok_clicked()
{
	set_visible(false);
	exit_with_code(1);
}

void LoginView::on_button_cancel_clicked()
{
	set_visible(false);
	exit_with_code(0);
}

void LoginView::on_edit_player_nick_changed(CL_InputEvent &e)
{
	update_button_states();
}

void LoginView::update_button_states()
{
	if(edit_player_nick->get_text().length() > 0)
		button_ok->set_enabled(true);
	else
		button_ok->set_enabled(false);
}
