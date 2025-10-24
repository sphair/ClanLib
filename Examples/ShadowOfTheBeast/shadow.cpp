#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/sound.h>
#include <ClanLib/mikmod.h>
#include <stdlib.h>

class App
{
public:
	virtual int main(const std::vector<CL_String> &args)
	{
		try
		{	
			CL_DisplayWindowDescription desc;
			desc.show_caption(false);	// No Title
			desc.set_decorations(false);
			desc.set_visible(true);
			desc.set_topmost(false);
			desc.set_tool_window(false);
			desc.set_drop_shadow(false);
			desc.set_fullscreen(true);	// Set to full screen
			desc.set_title("Shadow of the Beast");
			CL_DisplayWindow window(desc);

			// Get the graphic context
			CL_GraphicContext gc = window.get_gc();

			gc.set_map_mode(CL_MapMode(cl_map_2d_upper_left));

			CL_Sprite s1(gc, "resources/bgd1_ciel.png");
			CL_Sprite s2(gc, "resources/bgd2_montagnes.png");
			CL_Sprite s3(gc, "resources/bgd3_sol1.png");
			CL_Sprite s4(gc, "resources/bgd4_sol2.png");
			CL_Sprite s5(gc, "resources/bgd5_sol3.png");
			CL_Sprite s6(gc, "resources/sprite_nuages1.png");
			CL_Sprite s7(gc, "resources/sprite_nuages2.png");
			CL_Sprite s8(gc, "resources/sprite_nuages3.png");
			CL_Sprite s9(gc, "resources/sprite_nuages4.png");
			CL_Sprite s10(gc, "resources/sprite_barriere.png");
			CL_Sprite s11(gc, "resources/fireworks.png");
			CL_Sprite s12(gc, "resources/sprite_arbre.png");
			CL_Sprite s13(gc, "resources/scrolltext.png");

			CL_SoundOutput sound_output(44100);
			CL_SoundBuffer sample("resources/b-title.mod");
			CL_SoundBuffer_Session playback(sample.prepare());
			playback.play();

			// Define the initial scrolling values
			signed int xspeed = 2;
			signed int xscroll5 = rand() % 640 + 640;
			signed int xscroll5b = rand() % 640 + 640;
			
			signed int xscroll=0, xscrollb=0;
			signed int xscroll1=0, xscroll2=0, xscroll3=0, xscroll4=0, xscroll6=0;
			
			// Loop until the user hits escape:
			while (!window.get_ic().get_keyboard().get_keycode(CL_KEY_ESCAPE))
			{
				xscroll+=xspeed;
				
				if (xscroll == 320) xspeed = -2;
				if (xscroll == -960) xspeed = 2;

				xscrollb = xscroll;
				if (xscrollb<-640) xscrollb=-640;
				if (xscrollb>0) xscrollb=0;

				xscroll1--;
				if (xscroll1==-640) xscroll1=0;

				xscroll2-=2;
				if (xscroll2==-640) xscroll2=0;

				xscroll3-=3;
				if (xscroll3<-640) xscroll3+=640;

				xscroll4-=4;
				if (xscroll4<-640) xscroll4+=640;

				xscroll5-=5;
				if (xscroll5<-640) xscroll5+=1280;

				xscroll5b-=2;
				if (xscroll5b<-640) xscroll5b+=1280;

				xscroll6-=5;
				if (xscroll6<-640) xscroll6+=640;

				//  Paste the images
				gc.push_scale( (float) gc.get_width() / 640.0f, (float) gc.get_height() / 480.0f );
				s1.draw(gc, 0, 0);
				s2.draw(gc, xscroll1, 200);
				s2.draw(gc, xscroll1+640, 200);
				s3.draw(gc, xscroll2, 420);
				s3.draw(gc, xscroll2+640, 420);
				s4.draw(gc, xscroll3, 430);
				s4.draw(gc, xscroll3+640, 430);
				s5.draw(gc, xscroll4, 450);
				s5.draw(gc, xscroll4+640, 450);
				s10.draw(gc, xscroll5, 440);
				s6.draw(gc, xscroll6, 0);
				s6.draw(gc, xscroll6+640, 0);
				s7.draw(gc, xscroll4, 82);
				s7.draw(gc, xscroll4+640, 82);
				s8.draw(gc, xscroll3, 120);
				s8.draw(gc, xscroll3+640, 120);
				s9.draw(gc, xscroll2, 138);
				s9.draw(gc, xscroll2+640, 138);
				s12.draw(gc, xscroll5b, 140);
				s11.draw(gc, xscrollb, 0);
				s13.draw(gc, xscrollb+640, 0);
				gc.pop_modelview();

				// Flip front and backbuffer. This makes the changes visible:
				// Using parameter 1 to sync to refresh.
				window.flip(1);

				// Update keyboard input and handle system events:
				CL_KeepAlive::process();
			}
		}
		catch(CL_Exception& exception)
		{
			CL_ConsoleWindow console("Console", 80, 160);
			CL_Console::write_line("Exception caught: " + exception.get_message_and_stack_trace());
			console.display_close_message();

			return -1;
		}

		return 0;
	}
};

// This is the Program class that is called by CL_ClanApplication
class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		// Initialize ClanLib base components
		CL_SetupCore setup_core;

		// Initialize the ClanLib display component
		CL_SetupDisplay setup_display;

		// Initilize the OpenGL drivers
		CL_SetupGL setup_gl;

		CL_SetupSound setup_sound;
		CL_SetupMikMod setup_mikmod;

		// Start the Application
		App app;
		int retval = app.main(args);
		return retval;
	}
};

// Instantiate CL_ClanApplication, informing it where the Program is located
CL_ClanApplication app(&Program::main);

