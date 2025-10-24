
#pragma once

class PreferencesViewer;

class PreferencesPage : public CL_GUIComponent
{
public:
	PreferencesPage(PreferencesViewer *viewer);
	virtual ~PreferencesPage() { }

	PreferencesViewer *viewer;
	CL_String label;
	CL_Image icon;
	CL_String title;
};
