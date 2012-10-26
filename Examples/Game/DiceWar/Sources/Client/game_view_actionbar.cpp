
#include "precomp.h"
#include "game_view_actionbar.h"
#include "game_view.h"

GameViewActionBar::GameViewActionBar(GameView *view)
: GUIComponent(view), view(view)
{
	set_type_name("actionbar");
	set_geometry(Rect(view->get_width() - 100, view->get_height() - 41, view->get_width(), view->get_height()));

	button_endturn = new PushButton(this);
	button_endturn->set_geometry(Rect(5, 5, get_width() - 5, 36));
	button_endturn->set_text("End Turn");
	button_endturn->func_clicked().set(this, &GameViewActionBar::on_button_endturn_clicked);
}

GameViewActionBar::~GameViewActionBar()
{
}

void GameViewActionBar::enable_end_turn()
{
	button_endturn->set_enabled(true);
}

void GameViewActionBar::disable_end_turn()
{
	button_endturn->set_enabled(false);
}

void GameViewActionBar::on_button_endturn_clicked()
{
	view->end_turn();
}
