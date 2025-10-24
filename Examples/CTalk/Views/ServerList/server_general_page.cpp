
#include "precomp.h"
#include "server_general_page.h"

ServerGeneralPage::ServerGeneralPage(PreferencesViewer *viewer)
: PreferencesPage(viewer)
{
	set_layout(layout);
	create_components("server_general_page.gui");
}
