#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

static const int BitmapFont_Height = 32;

static CL_Font_Texture_Position BitmapFont_Positions[] =
{
	{' ',0,0,0,0,0, 0 - BitmapFont_Height, 16,0},
	{'A',1,1,29,32,0, 1 - 1 - BitmapFont_Height, 29, 0},
	{'B',35,1,26,32,0,  1 - 1 - BitmapFont_Height, 26, 0},
	{'C',66,1,28,32,0, 1 - 1 - BitmapFont_Height, 28, 0},
	{'D',100,1,27,32,0, 1 - 1 - BitmapFont_Height, 27, 0},
	{'E',133,1,24,32,0, 1 - 1 - BitmapFont_Height, 24, 0},
	{'F',163,1,22,32,0, 1 - 1 - BitmapFont_Height, 22, 0},
	{'G',190,1,29,32,0, 1 - 1 - BitmapFont_Height, 29, 0},
	{'H',226,1,25,32,0, 1 - 1 - BitmapFont_Height, 25, 0},
	{'I',257,1,9,32,0, 1 - 1 - BitmapFont_Height, 9, 0},
	{'J',270,1,21,32,0, 1 - 1 - BitmapFont_Height, 21, 0},
	{'K',298,1,28,32,0, 1 - 1 - BitmapFont_Height, 28, 0},
	{'L',330,1,23,32,0, 1 - 1 - BitmapFont_Height, 23, 0},
	{'M',358,1,30,32,0, 1 - 1 - BitmapFont_Height, 30, 0},
	{'N',396,1,25,32,0, 1 - 1 - BitmapFont_Height, 25, 0},
	{'O',427,1,29,32,0, 1 - 1 - BitmapFont_Height, 29, 0},
	{'P',462,1,25,32,0, 1 - 1 - BitmapFont_Height, 25, 0},
	{'Q',492,1,30,34,0, 1 - 1 - BitmapFont_Height, 30, 0},
	{'R',528,1,27,32,0, 1 - 1 - BitmapFont_Height, 27, 0},
	{'S',558,1,27,32,0, 1 - 1 - BitmapFont_Height, 27, 0},
	{'T',589,1,24,32,0, 1 - 1 - BitmapFont_Height, 24, 0},
	{'U',619,1,25,32,0, 1 - 1 - BitmapFont_Height, 25, 0},
	{'V',648,1,30,32,0, 1 - 1 - BitmapFont_Height, 30, 0},
	{'W',679,1,41,32,0, 1 - 1 - BitmapFont_Height, 41, 0},
	{'X',722,1,29,32,0, 1 - 1 - BitmapFont_Height, 29, 0},
	{'Y',752,1,29,32,0, 1 - 1 - BitmapFont_Height, 29, 0},
	{'Z',782,1,25,32,0, 1 - 1 - BitmapFont_Height, 25, 0},
//	{'a',3,45,22,24,0, 45 - 37 - BitmapFont_Height, 22, 0},
	{'b',31,37,22,32,0, 37 - 37 - BitmapFont_Height, 22, 0},
	{'c',56,45,23,24,0, 45 - 37 - BitmapFont_Height, 23, 0},
	{'d',82,37,23,32,0, 37 - 37 - BitmapFont_Height, 23, 0},
	{'e',110,45,22,24,0, 45 - 37 - BitmapFont_Height, 22, 0},
	{'f',134,37,17,32,0, 37 - 37 - BitmapFont_Height, 17, 0},
	{'g',152,45,23,32,0, 45 - 37 - BitmapFont_Height, 23, 0},
	{'h',181,37,21,32,0, 37 - 37 - BitmapFont_Height, 21, 0},
	{'i',208,37,8,32,0, 37 - 37 - BitmapFont_Height, 8, 0},
	{'j',218,37,13,40,0, 37 - 37 - BitmapFont_Height, 13, 0},
	{'k',238,37,21,32,0, 37 - 37 - BitmapFont_Height, 21, 0},
	{'l',263,37,9,32,0, 37 - 37 - BitmapFont_Height, 9, 0},
	{'m',277,45,35,24,0, 45 - 37 - BitmapFont_Height, 35, 0},
	{'n',317,45,21,24,0, 45 - 37 - BitmapFont_Height, 21, 0},
	{'o',343,45,24,24,0, 45 - 37 - BitmapFont_Height, 24, 0},
	{'p',372,45,23,32,0, 45 - 37 - BitmapFont_Height, 23, 0},
	{'q',398,45,22,32,0, 45 - 37 - BitmapFont_Height, 22, 0},
	{'r',428,45,16,24,0, 45 - 37 - BitmapFont_Height, 16, 0},
	{'s',445,45,22,24,0, 45 - 37 - BitmapFont_Height, 22, 0},
	{'t',470,38,15,31,0, 38 - 37 - BitmapFont_Height, 15, 0},
	{'u',489,45,21,24,0, 45 - 37 - BitmapFont_Height, 21, 0},
	{'v',512,40,24,30,0, 40 - 37 - BitmapFont_Height, 24, 0},
	{'w',538,45,33,24,0, 45 - 37 - BitmapFont_Height, 33, 0},
	{'x',572,45,25,24,0, 45 - 37 - BitmapFont_Height, 25, 0},
	{'y',598,45,24,32,0, 45 - 37 - BitmapFont_Height, 24, 0},
	{'z',624,45,20,24,0, 45 - 37 - BitmapFont_Height, 20, 0},
	{'0',1,78,21,32,0, 78 - 78 - BitmapFont_Height, 21, 0},
	{'1',29,78,15,32,0, 78 - 78 - BitmapFont_Height, 15, 0},
	{'2',52,78,22,32,0, 78 - 78 - BitmapFont_Height, 22, 0},
	{'3',78,78,22,32,0, 78 - 78 - BitmapFont_Height, 22, 0},
	{'4',104,78,22,32,0, 78 - 78 - BitmapFont_Height, 22, 0},
	{'5',130,78,22,32,0, 78 - 78 - BitmapFont_Height, 22, 0},
	{'6',155,78,22,32,0, 78 - 78 - BitmapFont_Height, 22, 0},
	{'7',181,78,21,32,0, 78 - 78 - BitmapFont_Height, 21, 0},
	{'8',206,78,22,32,0, 78 - 78 - BitmapFont_Height, 22, 0},
	{'9',232,78,22,32,0, 78 - 78 - BitmapFont_Height, 22, 0},
	{'!',3,119,8,32,0, 119 - 119 - BitmapFont_Height, 8, 0},
	{'"',18,119,17,13,0, 119 - 119 - BitmapFont_Height, 17, 0},
	{'£',38,119,24,32,0, 119 - 119 - BitmapFont_Height, 24, 0},
	{'$',65,117,22,38,0, 117 - 119 - BitmapFont_Height, 22, 0},
	{'%',91,119,33,33,0, 119 - 119 - BitmapFont_Height, 33, 0},
	{'^',129,119,21,18,0, 119 - 119 - BitmapFont_Height, 21, 0},
	{'&',155,119,29,32,0, 119 - 119 - BitmapFont_Height, 29, 0},
	{'*',187,119,15,16,0, 119 - 119 - BitmapFont_Height, 15, 0},
	{'(',208,119,13,40,0, 119 - 119 - BitmapFont_Height, 13, 0},
	{')',224,119,13,40,0, 119 - 119 - BitmapFont_Height, 13, 0},
	{'-',242,135,13,8,0, 135 - 119 - BitmapFont_Height, 13, 0},
	{'_',255,152,26,7,0, 152 - 119 - BitmapFont_Height, 26, 0},
	{'+',283,125,22,22,0, 125 - 119 - BitmapFont_Height, 22, 0},
	{'=',310,127,22,17,0, 127 - 119 - BitmapFont_Height, 22, 0},
	{'[',339,119,12,40,0, 119 - 119 - BitmapFont_Height, 12, 0},
	{']',352,119,13,40,0, 119 - 119 - BitmapFont_Height, 13, 0},
	{'{',370,119,16,40,0, 119 - 119 - BitmapFont_Height, 16, 0},
	{'}',390,119,16,40,0, 119 - 119 - BitmapFont_Height, 16, 0},
	{':',412,127,9,24,0, 127 - 119 - BitmapFont_Height, 9, 0},
	{';',430,127,8,31,0, 127 - 119 - BitmapFont_Height, 8, 0},
	{'\'',446,119,8,13, 119 -0, 119 - BitmapFont_Height, 8, 0},
	{'@',458,119,41,41,0, 119 - 119 - BitmapFont_Height, 41, 0},
	{'#',501,119,22,32,0, 119 - 119 - BitmapFont_Height, 22, 0},
	{'~',527,130,22,11,0, 130 - 119 - BitmapFont_Height, 22, 0},
	{'\\',552,119,14,32, 119 -0, 119 - BitmapFont_Height, 14, 0},
	{'|',570,119,6,41,0, 119 - 119 - BitmapFont_Height, 6, 0},
	{',',585,142,8,16,0, 142 - 119 - BitmapFont_Height, 8, 0},
	{'<',597,123,23,25,0, 123 - 119 - BitmapFont_Height, 23, 0},
	{'.',626,142,9,9,0, 142 - 119 - BitmapFont_Height, 9, 0},
	{'>',629,123,22,25,0, 123 - 119 - BitmapFont_Height, 22, 0},
	{'/',664,119,14,32,0, 119 - 119 - BitmapFont_Height, 14, 0},
	{'?',681,119,23,32,0, 119 - 119 - BitmapFont_Height, 23, 0},
	{0,0,0,0,0,0,0,0,0}	
};

class App
{
public:
	CL_Label *main_label;

	int main(const std::vector<CL_String> &args)
	{
		try
		{
			CL_GUIManager gui;

			CL_ResourceManager resources("../../../Resources/GUIThemeLuna/resources.xml");

			CL_GUIWindowManagerSystem wm;
			gui.set_window_manager(wm);

			CL_GUIThemeDefault theme;
			theme.set_resources(resources);
			gui.set_theme(theme); 
			gui.set_css_document("theme.css");

			CL_DisplayWindowDescription win_desc;
			win_desc.set_allow_resize(true);
			win_desc.set_title("Button WM System Test Application");
			win_desc.set_position(CL_Rect(200, 200, 940, 640), false);
			CL_Window window(&gui, win_desc);
			window.func_close().set(this, &App::on_close, &window);

			CL_VirtualFileSystem vfs("./");
			CL_VirtualDirectory vdir = vfs.get_root_directory();

			CL_PixelBuffer pb_font = CL_PNGProvider::load("clanfont.png", vdir);

			CL_GraphicContext gc = window.get_gc();

			CL_Font_Texture font_bitmap(gc, &pb_font, BitmapFont_Positions);

			// Load the standard font onto font_bitmap, as there are might be missing glyphs from my font (Required for the vertical bar)
			CL_FontDescription desc;
			desc.set_typeface_name("Tahoma");
			desc.set_height(32);
			font_bitmap.load_font(gc, desc);

			// Set the gui named font
			CL_FontDescription clanlib_font_desc;
			clanlib_font_desc.set_typeface_name("ClanLib Font");
			clanlib_font_desc.set_height(32);
			clanlib_font_desc.set_weight(400);
			gui.set_named_font(font_bitmap, clanlib_font_desc);

			CL_Label label(&window);
			label.set_geometry(CL_Rect(10, 160, 330, 180));
			label.set_text("Click the button");
			main_label = &label;

			CL_PushButton button(&window);
			button.set_geometry(CL_Rect(10, 10, 400, 100));
			button.func_clicked().set(this, &App::on_button_clicked, &button);
			button.set_class_name("redtext");
			button.set_text("Button Normal");

			CL_LineEdit lineedit1(&window);
			lineedit1.set_geometry(CL_Rect(20, 300, 700, 380));
			lineedit1.set_class_name("greentext");
			lineedit1.set_text("Select, Click and Edit me :)"); 

			CL_AcceleratorTable accel_table;
			gui.exec(accel_table);
		}
		catch (CL_Exception e)
		{
			CL_ConsoleWindow console("Console");
			CL_Console::write_line(e.message);
			console.display_close_message();
		}

		return 0;
	}

private:
	void on_button_clicked(CL_PushButton *button)
	{
		main_label->set_text("You clicked " + button->get_text());
	}

	void on_close(CL_Window *win)
	{
		win->exit_with_code(0);
	}
};

// This is the Program class that is called by CL_ClanApplication
class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		CL_SetupCore setup_core;
		CL_SetupDisplay setup_display;
		CL_SetupGL setup_gl;

		// Start the Application
		App app;
		int retval = app.main(args);
		return retval;
	}
};

// Instantiate CL_ClanApplication, informing it where the Program is located
CL_ClanApplication app(&Program::main);

