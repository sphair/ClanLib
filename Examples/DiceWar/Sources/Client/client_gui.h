
#pragma once

class Client;

class ClientGUI
{
public:
	ClientGUI(Client *client);
	~ClientGUI();

	CL_GUIManager *get_gui() { return gui; }

private:
	void on_window_close();

	Client *client;
	CL_DisplayWindow display_window;
	CL_GUIManager *gui;
	CL_GUITheme *theme;
	CL_GUIWindowManagerTexture *window_manager;
	CL_ResourceManager resources;
	CL_Slot slot_quit;
};
