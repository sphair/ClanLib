
#include "precomp.h"
#include "preferences_page.h"
#include "preferences_viewer.h"

PreferencesPage::PreferencesPage(PreferencesViewer *viewer)
: CL_GUIComponent(viewer)
{
	set_visible(false);
}
