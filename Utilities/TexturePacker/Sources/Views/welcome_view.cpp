#include "precomp.h"
#include "welcome_view.h"

WelcomeView::WelcomeView(CL_GUIComponent *parent, MainWindow *mainwindow)
: View(parent, mainwindow, "Welcome")
{
	func_resized().set(this, &WelcomeView::on_resized);

	on_resized();
}

WelcomeView::~WelcomeView()
{
}

void WelcomeView::on_resized()
{
}
