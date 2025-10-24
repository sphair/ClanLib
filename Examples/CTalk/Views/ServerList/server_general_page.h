
#pragma once

#include "Framework/preferences_page.h"

class ServerGeneralPage : public PreferencesPage
{
public:
	ServerGeneralPage(PreferencesViewer *viewer);

private:
	CL_GUILayoutCorners layout;
};
