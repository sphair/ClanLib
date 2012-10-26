
#include "precomp.h"
#include "error_screen.h"

ErrorScreen::ErrorScreen(CL_GUIManager *gui)
: UIScreen(gui)
{
	label = new CL_Label(this);
	label->set_geometry(CL_Rect(10, 10, CL_Size(300,40)));
	label->set_text("Error!");

	button = new CL_PushButton(this);
	button->set_geometry(CL_Rect(10, 50, CL_Size(100, 30)));
	button->set_text("Quit");
	button->func_clicked().set(this, &ErrorScreen::on_button_click);

	func_render().set(this, &ErrorScreen::on_render);
}

ErrorScreen::~ErrorScreen()
{
}

void ErrorScreen::on_render(CL_GraphicContext &gc, const CL_Rect &rect)
{
	gc.clear(CL_Colorf::dodgerblue);
}

void ErrorScreen::on_button_click()
{

}
