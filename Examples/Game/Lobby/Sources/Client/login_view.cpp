
#include "precomp.h"
#include "login_view.h"

LoginView::LoginView(clan::GUIManager *gui_manager)
: clan::GUIComponent(gui_manager, get_toplevel_description(), "login")
{
	label_info = new clan::Label(this);
	label_info->set_geometry(clan::RectPS(15, 10, 370, 27));
	label_info->set_class("header", true);
	label_info->set_text("Enter connection information");

	label_player_nick = new clan::Label(this);
	label_player_nick->set_geometry(clan::RectPS(15, 50, 370, 27));
	label_player_nick->set_text("Player nick:");

	edit_player_nick = new clan::LineEdit(this);
	edit_player_nick->set_geometry(clan::RectPS(15, 77, 370, 23));
	edit_player_nick->func_after_edit_changed().set(this, &LoginView::on_edit_player_nick_changed);
	#ifdef _DEBUG
	edit_player_nick->set_text("Player");
	#endif

	label_server = new clan::Label(this);
	label_server->set_geometry(clan::RectPS(15, 120, 370, 27));
	label_server->set_text("Server information:");

	edit_server = new clan::LineEdit(this);
	edit_server->set_geometry(clan::RectPS(15, 147, 300, 23));
	edit_server->set_text("localhost");

	edit_port = new clan::LineEdit(this);
	edit_port->set_geometry(clan::RectPS(320, 147, 65, 23));
	edit_port->set_text("4558");

	button_ok = new clan::PushButton(this);
	button_ok->set_geometry(clan::RectPS(15, 205, 75, 23));
	button_ok->set_text("OK");
	button_ok->func_clicked().set(this, &LoginView::on_button_ok_clicked);
	update_button_states();

	button_cancel = new clan::PushButton(this);
	button_cancel->set_geometry(clan::RectPS(245, 205, 75, 23));
	button_cancel->set_text("Cancel");
	button_cancel->func_clicked().set(this, &LoginView::on_button_cancel_clicked);
}

LoginView::~LoginView()
{
}

clan::GUITopLevelDescription LoginView::get_toplevel_description()
{
	clan::GUITopLevelDescription desc;
	desc.set_title("Connect to Server");
	desc.set_position(clan::RectPS(312, 100, 400, 260), false);
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

void LoginView::on_edit_player_nick_changed(clan::InputEvent &e)
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
