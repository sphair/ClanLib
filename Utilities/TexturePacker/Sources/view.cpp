#include "precomp.h"
#include "view.h"
#include "main_window.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

View::View(CL_GUIComponent *parent, MainWindow *mainwindow, const CL_String &title)
: CL_GUIComponent(parent), mainwindow(mainwindow), title(title), closable(true)
{
}

View::~View()
{
}
