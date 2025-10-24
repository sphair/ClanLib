#include "precomp.h"
#include "client_gui.h"

ClientGUI::ClientGUI()
{
	CL_OpenGLWindowDescription desc;
	desc.set_title("ClanLib - Dice War");
	desc.set_size(CL_Size(1024, 768), true);
//	desc.set_allow_resize(true);
//	desc.set_multisampling(4);
	display_window = CL_DisplayWindow(desc);

	CL_Font_System::register_font("Resources\\bitstream_vera_sans\\VeraBd.ttf", "VeraBd");

	slot_quit = display_window.sig_window_close().connect(this, &ClientGUI::on_window_close);

	gui = CL_GUIManager(display_window, "Resources/GUITheme");
}

void ClientGUI::on_window_close()
{
	gui.exit_with_code(0);
}