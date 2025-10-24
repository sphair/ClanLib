#include <ClanLib/application.h>
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

class Application : public CL_ClanApplication
{
public:
	virtual int main(int argc, char** argv)
	{
		CL_ConsoleWindow debug_console("Debug console");
		debug_console.redirect_stdio();

		try
		{
			CL_SetupCore setup_core;
			CL_SetupDisplay setup_display;
			CL_SetupGL setup_gl;

			CL_DisplayWindow window("ClanLib Stencil Demo", 800,600, false);

			stencil_test(window);
		}
		catch (CL_Error error)
		{
			std::cout << "Uncaught error: " << error.message.c_str() << std::endl;
			debug_console.display_close_message();
		}

		return 0;
	}

	void stencil_test(CL_DisplayWindow &window)
	{
		// load surfaces
		CL_Surface wheel("images/wheel.png");
		CL_Surface text("images/stencil_text.png");
		CL_Surface highlight("images/clouds.png");
		CL_Surface spot("images/spot.png");

		// set alignment of surfaces to centered.
		text.set_alignment(origin_center);		
		wheel.set_alignment(origin_center);		
		highlight.set_alignment(origin_center);		

		// set rotation hotspots to center.
		wheel.set_rotation_hotspot(origin_center);		
		highlight.set_rotation_hotspot(origin_center);		

		// change blendfunc of highlight to additive
		highlight.set_blend_func( blend_src_alpha, blend_one );

		spot.set_alignment(origin_bottom_center);
		spot.set_rotation_hotspot(origin_bottom_center);

		// get graphics context handle
		CL_GraphicContext *gc = window.get_gc();

		int width = CL_Display::get_width();
		int height = CL_Display::get_height();
		
		float sin_count = 0;
		int angle = 0;
		int highlight_angle = 0;

		while(!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
		{
			sin_count += (float) (M_PI/180.0);
			angle += 2;
			highlight_angle += 8;

			int a=int(255.f*fabs(sin(sin_count))) - 30;
			int b=int(255.f*fabs(sin(sin_count+(0.25*M_PI)))) - 30;
			int c=int(255.f*fabs(sin(sin_count+(0.5*M_PI)))) - 30;

			if( a < 0 ) a = 0;
			if( b < 0 ) b = 0;
			if( c < 0 ) c = 0;

			// draw screen background: a blue shifting gradient.
			CL_Display::fill_rect(CL_Rect(0,0,width,height), 
					CL_Gradient( 
						CL_Color(0,0,a,255),
						CL_Color(0,0,c,255),
						CL_Color(0,0,b,255),
						CL_Color(0,0,a,255)));
			
			/* Lower Text:
			 *
			 * This demonstrates the simplest use of stenciling.
			 *
			 * 1. Create the stencil mask.
			 * 	1.1 Stencil buffer writing is enabled
			 * 	1.2 A surface (or other graphics primitive) is drawn as usual
			 * 	1.3 Stencil buffer writing is disabled
			 * 2. Draw, testing against the mask.
			 * 	2.1 Stencil buffer testing is enabled
			 * 	2.2 A surface is drawn as usual.
			 * 	2.3 Stencil testing is disabled.
			 *  
			 *  This will clip the surface drawn in step 2.2 to the area covered by the surface(s)
			 *  drawn to the stencil buffer in step 1.2.
			*/

			// erase old contents from the stencil buffer.
			gc->clear_stencil();

			// Enable drawing to the stencil buffer, disable color buffer writing
			gc->enable_stencil_write(true, false); 

			// Draw into the stencil buffer.
			text.set_scale(1.0f, 1.0f); // reset text size.
			text.draw(width/2,height/2+height/4);

			// Disable stencil writing.
			gc->enable_stencil_write(false);

			// Enable stencil testing.
			gc->enable_stencil_test(true);

			// Draw, testing each pixel against the stencil mask.
			spot.set_angle(angle);
			spot.draw(width/2,height);

			// Disable stencil testing.
			gc->enable_stencil_test(false);


			/* Upper Text:
			 *
			 * Here is a bit more complicated example: 
			 *
			 * Three spinning wheels are drawn inside the text. Each wheel
			 * is overlayed with 2 highlighting surfaces (a grayscale "cloud").
			 * 
			 * here are the steps:
			 *  
			 * 1. The text is drawn into the stencil buffer with color buf write off.
			 *      - Where something is drawn the stencil gets the value 1. 
			 * 2. The wheels are drawn on top of the text into the stencil (not color buf).
			 *      - Stencil testing is also enabled. Only where the stencil
			 *        contains a 1, do we modify the value. Each pixel, successfully drawn
			 *        into the stencil buffer will increase the corresponding value in the stencil.
			 *        Because there are overlapping parts, the parts overlapping the text
			 *        will get the value 2, and the rest remain 1 or 0.
			 * 3. Enable Stencil testing and fill the text with a gradient.
			 *		- the parameters specify that only stecil values equalling 1 get drawn over.
			 *        That means the text, excluding the wheels.
			 * 4. Draw the wheels and their highlight surfaces
			 *      - Stencil is tested for == 2, the value the wheels on top of text have.
			 *        
			 * For further reading about stencil buffer operations search the web
			 * for "OpenGL red book", and the opengl man pages, which are also available
			 * online.
			 */
	
			wheel.set_angle(angle);
			
			gc->clear_stencil();

			text.set_scale(1.2f,1.9f);

			// 1. write text into stencil.
			gc->enable_stencil_write(true, false); // enable stencil write, disable color buf write
			text.draw(width/2,height/2);
			gc->enable_stencil_write(false);

			// 2. draw stencil of wheels clipping to stencil of the text.
			gc->set_stencil_operation( stencil_incr, stencil_keep, stencil_equal, 1);
			gc->enable_stencil_write(true, false); // enable stencil write, disable color buf write
			
			wheel.draw(width/2,height/2);     // overdraw -> value in stencil is incremented
			wheel.draw(width/2-256,height/2); // to 2 for the wheels over text
			wheel.draw(width/2+256,height/2);
			
			// reset to original settings.
			gc->set_stencil_operation( stencil_incr, stencil_keep, stencil_always, 0 );

			// disable stencil writing.
			gc->enable_stencil_write(false);

			// 3. Enable stencil testing and fill the font.
			gc->enable_stencil_test(true, stencil_equal, 1); // pass stencil test if stencil == 1

			CL_Display::fill_rect(CL_Rect(0,0,width,height), 
				CL_Gradient( 
					CL_Color(a,b,c,a),
					CL_Color(b,c,a,b),
					CL_Color(c,b,a,c),
					CL_Color(c,a,b,a)));

			gc->enable_stencil_test(false); 

			// 4. Draw wheels (where overlapping text). 
			gc->enable_stencil_test(true, stencil_equal, 2); // pass if stencil == 2

			// Draw the wheels using stencil testing
			wheel.draw(width/2,height/2);
			wheel.draw(width/2-256,height/2);
			wheel.draw(width/2+256,height/2);

			// Draw highlight texture over wheels.
			highlight.set_angle(angle);
			highlight.draw(width/2,height/2);
			highlight.draw(width/2+256,height/2);
			highlight.draw(width/2-256,height/2);

			highlight.set_angle(highlight_angle);
			highlight.set_color(a,b,c,1.0f);
			highlight.draw(width/2,height/2);
			highlight.draw(width/2+256,height/2);
			highlight.draw(width/2-256,height/2);

			gc->enable_stencil_test(false);

			window.flip();
			CL_System::keep_alive(20);
		}
	}
} app;
