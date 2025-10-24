#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/sound.h>
#include <ClanLib/mikmod.h>

class App : public CL_ClanApplication
{
public:
	virtual int main(int, char **)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
			CL_SetupCore setup_core;
			CL_SetupDisplay setup_display;
			CL_SetupGL setup_gl;
			CL_SetupSound setup_sound;
			CL_SetupMikMod setup_mikmod;
			
			CL_DisplayWindow window("Shadow of the Beast", 640, 480, true);

			CL_Surface s1("resources/bgd1_ciel.png");
			CL_Surface s2("resources/bgd2_montagnes.png");
			CL_Surface s3("resources/bgd3_sol1.png");
			CL_Surface s4("resources/bgd4_sol2.png");
			CL_Surface s5("resources/bgd5_sol3.png");
			CL_Surface s6("resources/sprite_nuages1.png");
			CL_Surface s7("resources/sprite_nuages2.png");
			CL_Surface s8("resources/sprite_nuages3.png");
			CL_Surface s9("resources/sprite_nuages4.png");
			CL_Surface s10("resources/sprite_barriere.png");
			CL_Surface s11("resources/fireworks.png");
			CL_Surface s12("resources/sprite_arbre.png");
			CL_Surface s13("resources/scrolltext.png");

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
			while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
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
				s1.draw(0, 0);
				s2.draw(xscroll1, 200);
				s2.draw(xscroll1+640, 200);
				s3.draw(xscroll2, 420);
				s3.draw(xscroll2+640, 420);
				s4.draw(xscroll3, 430);
				s4.draw(xscroll3+640, 430);
				s5.draw(xscroll4, 450);
				s5.draw(xscroll4+640, 450);
				s10.draw(xscroll5, 440);
				s6.draw(xscroll6, 0);
				s6.draw(xscroll6+640, 0);
				s7.draw(xscroll4, 82);
				s7.draw(xscroll4+640, 82);
				s8.draw(xscroll3, 120);
				s8.draw(xscroll3+640, 120);
				s9.draw(xscroll2, 138);
				s9.draw(xscroll2+640, 138);
				s12.draw(xscroll5b, 140);
				s11.draw(xscrollb, 0);
				s13.draw(xscrollb+640, 0);

				// Flip front and backbuffer. This makes the changes visible:
				// Using parameter 1 to sync to refresh.
				CL_Display::flip(1);

				// Update keyboard input and handle system events:
				CL_System::keep_alive();
			}
		}
		catch (CL_Error err)
		{
			std::cout << "error: " << err.message.c_str() << std::endl;

			// Display console close message and wait for a key
			console.display_close_message();

			return -1;
		}

		return 0;
	}
} app;
