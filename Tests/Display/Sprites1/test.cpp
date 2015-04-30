#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

using namespace clan;

class App : public clan::Application
{
public:
	App()
	{
		clan::OpenGLTarget::enable();
		window = DisplayWindow("über sprite test", 1024, 1024);
		canvas = Canvas(window);
	
		background = Sprite(canvas, "Images/background.png");
		test1_facit = Sprite(canvas, "Images/test1_facit.png");

		testsprite1 = Sprite(canvas, "Images/testsprite1.png");

	}
	bool update()
	{
		if (window.get_ic().get_keyboard().get_keycode(keycode_escape))
			return false;

		if(window.get_ic().get_keyboard().get_keycode(keycode_space))
		{
			test1_facit.draw(canvas, 0, 0);
		}
		else
		{
			background.draw(canvas, 0, 0);

			// Normal
			testsprite1.draw(canvas, 56, 91);
			testsprite1.draw(
				canvas, 
				Rectf(0,0,testsprite1.get_width(),testsprite1.get_height()),
				Rectf(56, 91 + 256, 56 + testsprite1.get_width(), 91 + 256 + testsprite1.get_height()));
			testsprite1.draw(
				canvas, 
				Rectf(56, 91 + 512, 56 + testsprite1.get_width(), 91 + 512 + testsprite1.get_height()));

			// Scale - Note: Src/Dest functions doesn't care about scale-settings
			testsprite1.set_scale(2.0, 2.0);
			testsprite1.draw(canvas, 128 + 56, 91);
			testsprite1.draw(	
				canvas, 
				Rectf(0,0,testsprite1.get_width(),testsprite1.get_height()),
				Rectf(128 + 56, 91 + 256, 128 + 56 + testsprite1.get_width() * 2, 91 + 256 + testsprite1.get_height() * 2));
				testsprite1.draw(
				canvas, 
				Rectf(128 + 56, 91 + 512, 128 + 56 + testsprite1.get_width() * 2, 91 + 512 + testsprite1.get_height() * 2));
				testsprite1.set_scale(1.0, 1.0);

			// Rotate
			testsprite1.set_angle(Angle(45.0, angle_degrees));
			testsprite1.draw(canvas, 256 + 56, 91);
			testsprite1.draw(	
				canvas, 
				Rectf(0,0,testsprite1.get_width(),testsprite1.get_height()),
				Rectf(256 + 56, 91 + 256, 256 + 56 + testsprite1.get_width(), 91 + 256 + testsprite1.get_height()));
			testsprite1.draw(
				canvas, 
				Rectf(256 + 56, 91 + 512, 256 + 56 + testsprite1.get_width(), 91 + 512 + testsprite1.get_height()));
			testsprite1.set_angle(Angle(0.0f, angle_degrees));

			// Scale / Rotate - default hotspot
			testsprite1.set_angle(Angle(45.0f, angle_degrees));
			testsprite1.set_scale(2.0, 2.0);
			testsprite1.draw(canvas, 384 + 56, 91);
			testsprite1.draw(	
				canvas, 
				Rectf(0,0,testsprite1.get_width(),testsprite1.get_height()),
				Rectf(384 + 56, 91 + 256, 384 + 56 + testsprite1.get_width() * 2, 91 + 256 + testsprite1.get_height() * 2));
			testsprite1.draw(
				canvas, 
				Rectf(384 + 56, 91 + 512, 384 + 56 + testsprite1.get_width() * 2, 91 + 512 + testsprite1.get_height() * 2));
			testsprite1.set_angle(Angle(0.0f, angle_degrees));
			testsprite1.set_scale(1.0, 1.0);

			// Scale / Rotate - hotspot(0, 0)
			testsprite1.set_rotation_hotspot(origin_top_left, 0, 0);
			testsprite1.set_angle(Angle(45.0f, angle_degrees));
			testsprite1.set_scale(2.0, 2.0);
			testsprite1.draw(canvas, 512 + 56, 91);
			testsprite1.draw(	
				canvas, 
				Rectf(0,0,testsprite1.get_width(),testsprite1.get_height()),
				Rectf(512 + 56, 91 + 256, 512 + 56 + testsprite1.get_width() * 2, 91 + 256 + testsprite1.get_height() * 2));
			testsprite1.draw(
				canvas, 
				Rectf(512 + 56, 91 + 512, 512 + 56 + testsprite1.get_width() * 2, 91 + 512 + testsprite1.get_height() * 2));
			testsprite1.set_angle(Angle(0.0f, angle_degrees));
			testsprite1.set_scale(1.0, 1.0);
			testsprite1.set_rotation_hotspot(origin_center, 0, 0);

			// Blend - TODO

			// Alignment - TODO
			testsprite1.set_alignment(origin_bottom_right, 0, 0);
			testsprite1.draw(canvas, 768 + 56, 91);
			testsprite1.draw(
				canvas, 
				Rectf(0,0,testsprite1.get_width(),testsprite1.get_height()),
				Rectf(768 + 56, 91 + 256, 768 + 56 + testsprite1.get_width(), 91 + 256 + testsprite1.get_height()));
			testsprite1.draw(
				canvas, 
				Rectf(768 + 56, 91 + 512, 768 + 56 + testsprite1.get_width(), 91 + 512 + testsprite1.get_height()));
			testsprite1.set_color(Color::white);
			testsprite1.set_alignment(origin_top_left, 0, 0);

			// Color
			testsprite1.set_color(Color::yellow);
			testsprite1.draw(canvas, 896 + 56, 91);
			testsprite1.draw(
				canvas, 
				Rectf(0,0,testsprite1.get_width(),testsprite1.get_height()),
				Rectf(896 + 56, 91 + 256, 896 + 56 + testsprite1.get_width(), 91 + 256 + testsprite1.get_height()));
			testsprite1.draw(
				canvas, 
				Rectf(896 + 56, 91 + 512, 896 + 56 + testsprite1.get_width(), 91 + 512 + testsprite1.get_height()));
			testsprite1.set_color(Color::white);
		}

		canvas.flush();

		window.flip(1);

		return true;
	}

	Sprite background;
	Sprite test1_facit;
	Sprite testsprite1;

	DisplayWindow window;
	Canvas canvas;

};

clan::ApplicationInstance<App> clanapp;

