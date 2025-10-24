
#pragma once

class ClientGUI
{
public:
	ClientGUI();
	~ClientGUI() {};

	CL_GUIManager *get_gui() { return &gui; }

private:
	void on_window_close();

	CL_DisplayWindow display_window;
	CL_GUIManager gui;
	CL_Slot slot_quit;
};
