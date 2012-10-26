
#include "precomp.h"
#include "splash_screen.h"

SplashScreen::SplashScreen(CL_GUIManager *gui)
: UIScreen(gui)
{
	label = new CL_Label(this);
	label->set_geometry(CL_Rect(10, 10, CL_Size(300,40)));
	label->set_text("Loading..");

	func_render().set(this, &SplashScreen::on_render);
}

SplashScreen::~SplashScreen()
{
}

void SplashScreen::on_render(CL_GraphicContext &gc, const CL_Rect &rect)
{
	gc.clear(CL_Colorf::orangered);
}
