
#include "precomp.h"
#include "program.h"
#include "css_browser.h"
#include <ClanLib/application.h>
#include <ClanLib/swrender.h>

CL_ClanApplication clanapp(&Program::main);

int Program::main(const std::vector<CL_String> &args)
{
	CL_SetupCore setup_core;
	CL_SetupNetwork setup_network;
	CL_SetupDisplay setup_display;
	CL_SetupSWRender setup_swrender;
	CL_SetupGUI setup_gui;
	
	CL_GUIManager gui("Resources/GUIThemeAero");
	gui.add_resources("Resources/Theme/resources.xml");
	try
	{
		CSSBrowser browser(&gui);
		return gui.exec();
	}
	catch (CL_Exception &e)
	{
		cl_message_box(&gui, "Unhandled Exception", e.get_message_and_stack_trace(), cl_mb_buttons_ok, cl_mb_icon_error);
		return 0;
	}
}
