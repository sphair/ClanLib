
#include "precomp.h"
#include "game_view_end.h"
#include "game_model.h"
#include "game_view.h"
#include "client.h"

GameViewEnd::GameViewEnd(GameView *view)
: GUIComponent(view, "game-over")
{
	set_visible(false);
	set_geometry(Rect(view->get_width() / 2 - 200, view->get_height() / 2 - 150, view->get_width() / 2 + 200, view->get_height() / 2 + 150));

	label_title = new Label(this);
	label_title->set_tag_name("title");
	label_title->set_geometry(Rect(10, 10, 100, 30));

	label_message = new Label(this);
	label_message->set_tag_name("title");
	label_message->set_geometry(Rect(10, 40, 100, 70));
	
	button_close = new PushButton(this);
	button_close->set_tag_name("close");
	button_close->set_text("Close");
	button_close->set_geometry(Rect(10, 80, 100, 110));
	button_close->func_clicked().set(this, &GameViewEnd::on_button_close_clicked);
}

GameViewEnd::~GameViewEnd()
{
}

void GameViewEnd::game_over(const std::string &title, const std::string &message)
{
	label_title->set_text(title);
	label_message->set_text(message);
	set_visible(true);
}

void GameViewEnd::close()
{
	set_visible(false);
}

void GameViewEnd::on_button_close_clicked()
{
	close();
}
