#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gui.h>
#include <ClanLib/gl.h>

#include "radial_menu.h"
#include "game_component.h"
#include "toolbar.h"

class App
{
public:
	int main(const std::vector<CL_String> &args);

private:
	void on_window_close(CL_GUIManager *gui);
};

class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		CL_SetupCore setup_core;
		CL_SetupDisplay setup_display;
		CL_SetupGUI setup_gui;
		CL_SetupGL setup_gl;

		App app;
		return app.main(args);
	}
};

// Instantiate CL_ClanApplication, informing it where the Program is located
CL_ClanApplication app(&Program::main);

// The start of the Application
int App::main(const std::vector<CL_String> &args)
{
	CL_ConsoleWindow console("Console", 80, 1000);
	CL_ConsoleLogger logger;

	try
	{
		CL_GUIManager gui;

		CL_DisplayWindow display_window("Custom GUI Components", 640, 480);
		CL_Slot slotWindowClose = display_window.sig_window_close().connect(this, &App::on_window_close, &gui);

		CL_ResourceManager resources("Resources/resources.xml"); 

		CL_GUIWindowManagerTexture wm(display_window);

		//  Note - If you are using the GL1 target, you will get a perfomance increase by enabling these 2 lines
		//   It reduces the number of internal CL_FrameBuffer swaps. The GL1 target (OpenGL 1.3), performs this slowly
		//   Setting the texture group here, lets the GUI Texture Window Manager know the optimum texture size of all root components
		//CL_TextureGroup texture_group(display_window.get_gc(), CL_Size(1024, 1024));
		//wm.set_texture_group(texture_group);

		CL_GUIThemeDefault theme;
		theme.set_resources(resources);

		gui.set_window_manager(wm); 
		gui.set_theme(theme);
		gui.set_css_document("Resources/theme.css");

		CL_Rect viewport = display_window.get_viewport();
		GameComponent game_component(viewport, &gui);

		CL_Rect toolbar_rect = CL_Rect((viewport.right - 448) / 2, viewport.bottom - 56, (viewport.right - 448) / 2 + 448, viewport.bottom);
		Toolbar toolbar(toolbar_rect, &game_component);	// GameComponent is the "desktop" that the toolbar sits on, as an owner

		CL_GraphicContext gc = display_window.get_gc();

		toolbar.add_item(CL_Sprite(gc, "Resources/Images/spell1.png"), CL_Sprite(gc, "Resources/Images/spell1_selected.png"), CL_Sprite(gc, "Resources/Images/spell1_clicked.png"));
		toolbar.add_item(CL_Sprite(gc, "Resources/Images/spell2.png"), CL_Sprite(gc, "Resources/Images/spell2_selected.png"), CL_Sprite(gc, "Resources/Images/spell2_clicked.png"));
		toolbar.add_item(CL_Sprite(gc, "Resources/Images/spell3.png"), CL_Sprite(gc, "Resources/Images/spell3_selected.png"), CL_Sprite(gc, "Resources/Images/spell3_clicked.png"));
		toolbar.add_item(CL_Sprite(gc, "Resources/Images/spell4.png"), CL_Sprite(gc, "Resources/Images/spell4_selected.png"), CL_Sprite(gc, "Resources/Images/spell4_clicked.png"));
		toolbar.add_item(CL_Sprite(gc, "Resources/Images/spell5.png"), CL_Sprite(gc, "Resources/Images/spell5_selected.png"), CL_Sprite(gc, "Resources/Images/spell5_clicked.png"));
		toolbar.add_item(CL_Sprite(gc, "Resources/Images/spell6.png"), CL_Sprite(gc, "Resources/Images/spell6_selected.png"), CL_Sprite(gc, "Resources/Images/spell6_clicked.png"));
		toolbar.add_item(CL_Sprite(gc, "Resources/Images/spell7.png"), CL_Sprite(gc, "Resources/Images/spell7_selected.png"), CL_Sprite(gc, "Resources/Images/spell7_clicked.png"));

		gui.exec();
	}
	catch(CL_Exception &exception)
	{
		CL_Console::write_line("Exception caught: " + exception.get_message_and_stack_trace());
		console.display_close_message();

		return -1;
	}
	return 0;
}

void App::on_window_close(CL_GUIManager *gui)
{
	gui->exit_with_code(0);
}
