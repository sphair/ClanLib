
#pragma once

class ClientGUI
{
public:
	ClientGUI();
	~ClientGUI() {};

	GUIManager *get_gui() { return &gui; }

private:
	void on_window_close();

	DisplayWindow display_window;
	GUIManager gui;
	Slot slot_quit;
};
