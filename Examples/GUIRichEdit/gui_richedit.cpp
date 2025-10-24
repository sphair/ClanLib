#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/guistylesilver.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

// The main application
class App : public CL_ClanApplication
{
public:
	App()
		: richedit_ ( NULL )
		, display_	( NULL )
	{}

	virtual ~App()
	{}

	int main(int argc, char** argv)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
			// Initialization
			CL_SetupCore setup_core;
			CL_SetupDisplay setup_display;
			CL_SetupGL setup_gl;
			CL_SetupGUI setup_gui;

			// Creating a window
			display_ = new CL_DisplayWindow("RichEdit simple example", 640, 480, false, true);
			
			{
				// Create the GUI using the silver style
				CL_ResourceManager gui_resources("../../Resources/GUIStyleSilver/gui.xml");
				CL_StyleManager_Silver style(&gui_resources);
				CL_GUIManager gui(&style);

				// Add custom images
				gui_resources.add_resources( CL_ResourceManager( "resources.xml" ) );

				font_ = new CL_Font("InputBox/font", &gui_resources);

				CL_SlotContainer slots;

				// Setup hook on the resize event
				slots.connect( display_->sig_resize		(), this, &App::on_resize		);
				slots.connect( display_->sig_window_close(), this, &App::on_close, &gui);

				// Setup hook for keypresses
				slots.connect(CL_Keyboard::sig_key_down(), this, &App::on_keypress, &gui);

				// Clear screen on each frame
				slots.connect( gui.sig_paint		(), this, &App::on_paint		);

				// Add the richedit control
				richedit_ = new CL_RichEdit( get_richedit_rect(), &gui, &style );

				fill_richedit( gui_resources );

				// Main loop
				gui.run();

				delete richedit_;
				delete font_;
			}

			delete display_;
		}
		catch (CL_Error e)
		{
 			std::cout << e.message.c_str() << std::endl;

			// Display console close message and wait for a key
			console.display_close_message();
		}

		return 0;
	}


private:
	void fill_richedit(CL_ResourceManager & gui_resources)
	{
		richedit_->make_new_paragraph();
		richedit_->make_new_paragraph();

		//
		richedit_->add_text( "Example of a text.", font_, CL_Color::black );
		richedit_->add_text( "Example of another colored text.", font_, CL_Color::red );
		richedit_->add_text( "GREEN TEXT.", font_, CL_Color::green );
		richedit_->make_new_paragraph();

		//
		std::string s;
		for (int i=0; i<200; ++i)
			s += "A lot of text in this paragraph. ";

		richedit_->add_text( s, font_, CL_Color::black );
		richedit_->make_new_paragraph();

		//
		// there is no need to delete these images - it will be deleted automatically
		richedit_->add_img( new CL_Sprite("img2", &gui_resources), "Description of this image" );
		richedit_->add_img( new CL_Sprite("img3", &gui_resources), "Description of this image" );
		richedit_->make_new_paragraph();

		//
		for (int i=0; i<50; ++i)
		{
			richedit_->add_img( new CL_Sprite("img1", &gui_resources), "Description of this image" );
			richedit_->add_text( "text between images", font_, CL_Color::brown );
		}
		richedit_->make_new_paragraph();
	}


	CL_Rect get_richedit_rect()
	{
		return CL_Rect( 8, 8, display_->get_width() - 8, display_->get_height() - 8 );
	}

	void on_resize(int old_width, int old_height)
	{
		CL_Rect rc = get_richedit_rect();
		richedit_->set_size( rc.right - rc.left, rc.bottom - rc.top );
	}

	void on_keypress(const CL_InputEvent &key, CL_GUIManager *gui)
	{
		if (key.id == CL_KEY_ESCAPE  ||
			CL_Keyboard::get_keycode(CL_KEY_F4) && CL_Keyboard::get_keycode(CL_KEY_LMENU) )
		{
			gui->quit();
		}
	}

	void on_close(CL_GUIManager *gui)
	{
		gui->quit();
	}

	void on_paint()
	{
		CL_Display::clear(CL_Color::antiquewhite);
	}

private:
	CL_DisplayWindow	* display_;
	CL_RichEdit			* richedit_;
	CL_Font				* font_;

} app;
