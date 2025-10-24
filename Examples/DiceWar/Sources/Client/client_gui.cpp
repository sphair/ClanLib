#include "precomp.h"
#include "client_gui.h"
#include "client.h"
#include <ClanLib/GL/opengl_wrap.h>

ClientGUI::ClientGUI(Client *client)
: client(client), gui(0), theme(0), window_manager(0)
{
	CL_OpenGLWindowDescription desc;
	desc.set_title("ClanLib - Dice War");
	desc.set_size(CL_Size(1024, 768), true);
//	desc.set_allow_resize(true);
//	desc.set_multisampling(4);
	display_window = CL_DisplayWindow(desc);

	CL_Font_Native::register_font("Resources\\accid___.ttf", "Accidental Presidency");

	slot_quit = display_window.sig_window_close().connect(this, &ClientGUI::on_window_close);

	resources = CL_ResourceManager("Resources/GUITheme/resources.xml");
	theme = new CL_GUIThemeDefault;
	theme->set_resources(resources);
	window_manager = new CL_GUIWindowManagerTexture(display_window);
	gui = new CL_GUIManager;
	gui->set_window_manager(window_manager);
	gui->set_theme(theme);
	gui->set_css_document("Resources/GUITheme/theme.css");
}

ClientGUI::~ClientGUI()
{
	delete gui;
	delete window_manager;
	delete theme;
	display_window = CL_DisplayWindow();
}

void ClientGUI::on_window_close()
{
	gui->exit_with_code(0);
}