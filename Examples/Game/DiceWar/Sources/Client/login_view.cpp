
#include "precomp.h"
#include "login_view.h"
#include "client.h"

LoginView::LoginView(Client *client)
: GUIComponent(client->get_gui(), get_toplevel_description(), "connectdialog"), client(client)
{
	label_info = new Label(this);
	label_info->set_geometry(RectPS(15, 10, 370, 27));
	label_info->set_text("Please enter connection information:");

	label_player_nick = new Label(this);
	label_player_nick->set_geometry(RectPS(15, 50, 370, 27));
	label_player_nick->set_text("Player nick:");

	edit_player_nick = new LineEdit(this);
	edit_player_nick->set_geometry(RectPS(15, 77, 370, 35));
	edit_player_nick->func_after_edit_changed().set(this, &LoginView::on_edit_player_nick_changed);
	#ifdef _DEBUG
	edit_player_nick->set_text("Player");
	#endif

	label_server = new Label(this);
	label_server->set_geometry(RectPS(15, 120, 370, 27));
	label_server->set_text("Server information:");

	edit_server = new LineEdit(this);
	edit_server->set_geometry(RectPS(15, 147, 300, 35));
	#ifdef _DEBUG
	edit_server->set_text("localhost");
	#else
	edit_server->set_text("esoteric.clanlib.org");
	#endif

	edit_port = new LineEdit(this);
	edit_port->set_geometry(RectPS(320, 147, 65, 35));
	edit_port->set_text("4558");

	button_ok = new PushButton(this);
	button_ok->set_geometry(RectPS(15, 205, 140, 40));
	button_ok->set_text("OK");
	button_ok->func_clicked().set(this, &LoginView::on_button_ok_clicked);
	#ifdef _DEBUG
	button_ok->set_enabled(true);
	#else
	button_ok->set_enabled(false);
	#endif

	button_cancel = new PushButton(this);
	button_cancel->set_geometry(RectPS(245, 205, 140, 40));
	button_cancel->set_text("Cancel");
	button_cancel->func_clicked().set(this, &LoginView::on_button_cancel_clicked);
}

LoginView::~LoginView()
{
}

GUITopLevelDescription LoginView::get_toplevel_description()
{
	GUITopLevelDescription desc;
	desc.set_title("Connect to Server");
	desc.set_position(RectPS(312, 100, 400, 260), false);
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

void LoginView::on_edit_player_nick_changed(InputEvent &e)
{
	if(edit_player_nick->get_text().length() > 0)
		button_ok->set_enabled(true);
	else
		button_ok->set_enabled(false);
}
