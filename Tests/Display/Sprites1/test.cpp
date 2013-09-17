#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
using namespace clan;

class App
{
public:
	static int main(const std::vector<std::string> &args)
	{
		SetupCore setup_core; 
		SetupDisplay setup_disp;
		SetupGL setup_gl;

		ConsoleWindow console("Console");

		try
		{
			DisplayWindow window("über sprite test", 1024, 1024);
			Canvas canvas(window);
			GraphicContext gc = canvas.get_gc();
			
//			gc.enable_blending(true);

			canvas.set_map_mode(map_2d_upper_left);

			Sprite background(canvas, "Images/background.png");
			Sprite test1_facit(canvas, "Images/test1_facit.png");

			Sprite testsprite1(canvas, "Images/testsprite1.png");

//			PixelBuffer buffer = ImageProviderFactory::load("Images/pacman.pcx");
//			int texture_width = buffer.get_width();
//			int texture_height = buffer.get_height();
//			Texture texture(gc, texture_width, texture_height);
//			texture.set_image(buffer);
//			SpriteDescription desc_pacman;
//			desc_pacman.add_gridclipped_frames(texture, 126, 93, 26, 30, 12, 2, 2, 2);
//			Sprite spr_pacman(desc_pacman, gc);

			bool quit = false;

//			SpriteRenderBatch batch(gc);

			while((!quit) && (!window.get_ic().get_keyboard().get_keycode(keycode_escape)))
			{
				if(window.get_ic().get_keyboard().get_keycode(keycode_space))
				{
					test1_facit.draw(canvas, 0, 0);
				}
				else
				{
					background.draw(canvas, 0, 0);

					// Normal
					testsprite1.draw(canvas, 56, 91);
//					testsprite1.draw(batch, 56, 91 + 128);
					testsprite1.draw(
						canvas, 
						Rectf(0,0,testsprite1.get_width(),testsprite1.get_height()),
						Rectf(56, 91 + 256, 56 + testsprite1.get_width(), 91 + 256 + testsprite1.get_height()));
/*					testsprite1.draw(
						batch, 
						Rectf(0,0,testsprite1.get_width(),testsprite1.get_height()),
						Rectf(56, 91 + 384, 56 + testsprite1.get_width(), 91 + 384 + testsprite1.get_height()));
*/					testsprite1.draw(
						canvas, 
						Rectf(56, 91 + 512, 56 + testsprite1.get_width(), 91 + 512 + testsprite1.get_height()));
/*					testsprite1.draw(
						batch, 
						Rectf(56, 91 + 640, 56 + testsprite1.get_width(), 91 + 640 + testsprite1.get_height()));
*/
					// Scale - Note: Src/Dest functions doesn't care about scale-settings
					testsprite1.set_scale(2.0, 2.0);
					testsprite1.draw(canvas, 128 + 56, 91);
//					testsprite1.draw(batch, 128 + 56, 91 + 128);
					testsprite1.draw(	
						canvas, 
						Rectf(0,0,testsprite1.get_width(),testsprite1.get_height()),
						Rectf(128 + 56, 91 + 256, 128 + 56 + testsprite1.get_width() * 2, 91 + 256 + testsprite1.get_height() * 2));
/*					testsprite1.draw(
						batch, 
						Rectf(0,0,testsprite1.get_width(),testsprite1.get_height()),
						Rectf(128 + 56, 91 + 384, 128 + 56 + testsprite1.get_width() * 2, 91 + 384 + testsprite1.get_height() * 2));
*/					testsprite1.draw(
						canvas, 
						Rectf(128 + 56, 91 + 512, 128 + 56 + testsprite1.get_width() * 2, 91 + 512 + testsprite1.get_height() * 2));
/*					testsprite1.draw(
						batch, 
						Rectf(128 + 56, 91 + 640, 128 + 56 + testsprite1.get_width() * 2, 91 + 640 + testsprite1.get_height() * 2));
*/					testsprite1.set_scale(1.0, 1.0);

					// Rotate
					testsprite1.set_angle(Angle(45.0, angle_degrees));
					testsprite1.draw(canvas, 256 + 56, 91);
//					testsprite1.draw(batch, 256 + 56, 91 + 128);
					testsprite1.draw(	
						canvas, 
						Rectf(0,0,testsprite1.get_width(),testsprite1.get_height()),
						Rectf(256 + 56, 91 + 256, 256 + 56 + testsprite1.get_width(), 91 + 256 + testsprite1.get_height()));
/*					testsprite1.draw(
						batch, 
						Rectf(0,0,testsprite1.get_width(),testsprite1.get_height()),
						Rectf(256 + 56, 91 + 384, 256 + 56 + testsprite1.get_width(), 91 + 384 + testsprite1.get_height()));
*/					testsprite1.draw(
						canvas, 
						Rectf(256 + 56, 91 + 512, 256 + 56 + testsprite1.get_width(), 91 + 512 + testsprite1.get_height()));
/*					testsprite1.draw(
						batch, 
						Rectf(256 + 56, 91 + 640, 256 + 56 + testsprite1.get_width(), 91 + 640 + testsprite1.get_height()));
*/					testsprite1.set_angle(Angle(0.0f, angle_degrees));

					// Scale / Rotate - default hotspot
					testsprite1.set_angle(Angle(45.0f, angle_degrees));
					testsprite1.set_scale(2.0, 2.0);
					testsprite1.draw(canvas, 384 + 56, 91);
//					testsprite1.draw(batch, 384 + 56, 91 + 128);
					testsprite1.draw(	
						canvas, 
						Rectf(0,0,testsprite1.get_width(),testsprite1.get_height()),
						Rectf(384 + 56, 91 + 256, 384 + 56 + testsprite1.get_width() * 2, 91 + 256 + testsprite1.get_height() * 2));
/*					testsprite1.draw(
						batch, 
						Rectf(0,0,testsprite1.get_width(),testsprite1.get_height()),
						Rectf(384 + 56, 91 + 384, 384 + 56 + testsprite1.get_width() * 2, 91 + 384 + testsprite1.get_height() * 2));
*/					testsprite1.draw(
						canvas, 
						Rectf(384 + 56, 91 + 512, 384 + 56 + testsprite1.get_width() * 2, 91 + 512 + testsprite1.get_height() * 2));
/*					testsprite1.draw(
						batch, 
						Rectf(384 + 56, 91 + 640, 384 + 56 + testsprite1.get_width() * 2, 91 + 640 + testsprite1.get_height() * 2));
*/					testsprite1.set_angle(Angle(0.0f, angle_degrees));
					testsprite1.set_scale(1.0, 1.0);

					// Scale / Rotate - hotspot(0, 0)
					testsprite1.set_rotation_hotspot(origin_top_left, 0, 0);
					testsprite1.set_angle(Angle(45.0f, angle_degrees));
					testsprite1.set_scale(2.0, 2.0);
					testsprite1.draw(canvas, 512 + 56, 91);
//					testsprite1.draw(batch, 512 + 56, 91 + 128);
					testsprite1.draw(	
						canvas, 
						Rectf(0,0,testsprite1.get_width(),testsprite1.get_height()),
						Rectf(512 + 56, 91 + 256, 512 + 56 + testsprite1.get_width() * 2, 91 + 256 + testsprite1.get_height() * 2));
/*					testsprite1.draw(
						batch, 
						Rectf(0,0,testsprite1.get_width(),testsprite1.get_height()),
						Rectf(512 + 56, 91 + 384, 512 + 56 + testsprite1.get_width() * 2, 91 + 384 + testsprite1.get_height() * 2));
*/					testsprite1.draw(
						canvas, 
						Rectf(512 + 56, 91 + 512, 512 + 56 + testsprite1.get_width() * 2, 91 + 512 + testsprite1.get_height() * 2));
/*					testsprite1.draw(
						batch, 
						Rectf(512 + 56, 91 + 640, 512 + 56 + testsprite1.get_width() * 2, 91 + 640 + testsprite1.get_height() * 2));
*/					testsprite1.set_angle(Angle(0.0f, angle_degrees));
					testsprite1.set_scale(1.0, 1.0);
					testsprite1.set_rotation_hotspot(origin_center, 0, 0);

					// Blend - TODO

					// Alignment - TODO
					testsprite1.set_alignment(origin_bottom_right, 0, 0);
					testsprite1.draw(canvas, 768 + 56, 91);
//					testsprite1.draw(batch, 768 + 56, 91 + 128);
					testsprite1.draw(
						canvas, 
						Rectf(0,0,testsprite1.get_width(),testsprite1.get_height()),
						Rectf(768 + 56, 91 + 256, 768 + 56 + testsprite1.get_width(), 91 + 256 + testsprite1.get_height()));
/*					testsprite1.draw(
						batch, 
						Rectf(0,0,testsprite1.get_width(),testsprite1.get_height()),
						Rectf(768 + 56, 91 + 384, 768 + 56 + testsprite1.get_width(), 91 + 384 + testsprite1.get_height()));
*/					testsprite1.draw(
						canvas, 
						Rectf(768 + 56, 91 + 512, 768 + 56 + testsprite1.get_width(), 91 + 512 + testsprite1.get_height()));
/*					testsprite1.draw(
						batch, 
						Rectf(768 + 56, 91 + 640, 768 + 56 + testsprite1.get_width(), 91 + 640 + testsprite1.get_height()));
*/					testsprite1.set_color(Color::white);
					testsprite1.set_alignment(origin_top_left, 0, 0);

					// Color
					testsprite1.set_color(Color::yellow);
					testsprite1.draw(canvas, 896 + 56, 91);
//					testsprite1.draw(batch, 896 + 56, 91 + 128);
					testsprite1.draw(
						canvas, 
						Rectf(0,0,testsprite1.get_width(),testsprite1.get_height()),
						Rectf(896 + 56, 91 + 256, 896 + 56 + testsprite1.get_width(), 91 + 256 + testsprite1.get_height()));
/*					testsprite1.draw(
						batch, 
						Rectf(0,0,testsprite1.get_width(),testsprite1.get_height()),
						Rectf(896 + 56, 91 + 384, 896 + 56 + testsprite1.get_width(), 91 + 384 + testsprite1.get_height()));
*/					testsprite1.draw(
						canvas, 
						Rectf(896 + 56, 91 + 512, 896 + 56 + testsprite1.get_width(), 91 + 512 + testsprite1.get_height()));
/*					testsprite1.draw(
						batch, 
						Rectf(896 + 56, 91 + 640, 896 + 56 + testsprite1.get_width(), 91 + 640 + testsprite1.get_height()));
*/					testsprite1.set_color(Color::white);

//					batch.flush();
				}

				/*				Draw::fill(gc, 
					0, 0, 
					640, 480, 
					Color::green);

				Draw::line(gc, 10,0,100,100, Color::yellow);

				spr_pacman.set_frame(0);
				spr_pacman.draw(canvas, 0, 0);

				Draw::line(gc, 0,0,100,100, Color::yellow);

				spr_pacman.set_frame(1);
				spr_pacman.draw(canvas, 50, 0);

				spr_pacman.set_frame(2);
				spr_pacman.draw(canvas, 100, 0);
*/
				canvas.flush();

				window.flip(1);
				KeepAlive::process();

				System::sleep(10);
			}

			return 0;
		}
		catch(Exception error)
		{
			Console::write_line("Exception caught:");
			Console::write_line(error.message);
			console.display_close_message();

			return -1;
		}

		return 0;
	}
};

Application app(&App::main);
