
#include "precomp.h"
#include "program.h"
#include "css_browser.h"
#include <ClanLib/application.h>
#include <ClanLib/d3d.h>

using namespace clan;

Application clanapp(&Program::main);

int Program::main(const std::vector<std::string> &args)
{
	SetupCore setup_core;
	SetupNetwork setup_network;
	SetupDisplay setup_display;
	SetupD3D setup_d3d;
	SetupGUI setup_gui;
	
	GUIManager gui("../../../Resources/GUIThemeAero");
	//gui.add_resources("Resources/Theme/resources.xml");
	//try
	{
		CSSBrowser browser(&gui);
		return gui.exec();
	}
	/*catch (Exception &e)
	{
		message_box(&gui, "Unhandled Exception", e.get_message_and_stack_trace(), mb_buttons_ok, mb_icon_error);
		return 0;
	}*/
}
