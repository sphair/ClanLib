#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

#define USE_GDI_BECAUSE_IT_IS_COOL

#ifdef USE_GDI_BECAUSE_IT_IS_COOL
#include <ClanLib/d3d.h>
#else
#include <ClanLib/gl.h>
#endif
using namespace clan;

class App
{
private:
	bool quit;

public:
	int start(const std::vector<std::string> &args)
	{
		ConsoleWindow console("Console");

		try
		{
			DisplayWindow window("Image test", 1024, 768);

			Canvas canvas(window);
			GraphicContext &gc = canvas.get_gc();

			// Connect the Window close event
			Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);
			//gc.enable_blending(true);

			quit = false;

			DisplayCache resources("resources.xml");

			Texture2D texture(gc, "Images/square.png");

			Image image_texture(gc, texture, Rect(0, 0, texture.get_size()));
			Image image_loaded(gc, "Images/square.png");
			Image image_resources(gc, "entire_image", &resources);
			Image image_copy(image_texture);
			Image image_top_right(gc, "image_top_right", &resources);
			Image image_bottom_right(gc, "image_bottom_right", &resources);
			Image image_black(gc, "image_black", &resources);

			Font small_font = Font(gc, "Tahoma", 12);

			//Console::write_line("Color: %1,%2,%3,%4", image_resources.get_color().r, image_resources.get_color().g, image_resources.get_color().b, image_resources.get_color().a);
			//Console::write_line("Scale: %1,%2", image_resources.get_scale_x(), image_resources.get_scale_y());
			//Console::write_line("Translation: %1,%2,%3", image_resources.get_alignment());

			while((!quit) && (!window.get_ic().get_keyboard().get_keycode(keycode_escape)))
			{
				gc.clear(Colorf(0.5f,0.5f,0.5f));

				small_font.draw_text(canvas, 10, 40, "Image From Texture (10,60)");
				image_texture.draw(canvas, 10, 60);

				small_font.draw_text(canvas, 150, 40, "Image From Load (150,60)");
				image_loaded.draw(canvas, 150, 60);

				small_font.draw_text(canvas, 300, 40, "Image From Resources (300,60)");
				image_resources.draw(canvas, 300, 60);

				small_font.draw_text(canvas, 450, 40, "Image Copied (450,60)");
				image_copy.draw(canvas, 450, 60);

				small_font.draw_text(canvas, 10, 190, "Image - Top Right (10,200)");
				image_top_right.draw(canvas, 10, 200);

				small_font.draw_text(canvas, 150, 190, "Image - Top Right (150,200)");
				image_texture.draw(canvas, Rect(32, 0, Size(32, 32)), Rect(150, 200, Size(32, 32)));

				small_font.draw_text(canvas, 300, 190, "Image - Bottom Right (300,200)");
				image_bottom_right.draw(canvas, 300, 200);

				small_font.draw_text(canvas, 450, 190, "Image - Bottom Right (450,200)");
				image_texture.draw(canvas, Rect(32, 32, Size(32, 32)), Rect(450, 200, Size(32, 32)));

				small_font.draw_text(canvas, 10, 290, "700 Images (10,300)");
				for(int i=0;i<700;i++)
					image_texture.draw(canvas, 10, 300);

				small_font.draw_text(canvas, 150, 290, "br image (150,400) Size(128,256)");
				image_bottom_right.draw(canvas, Rect(150, 300, Size(128, 256)));

				small_font.draw_text(canvas, 300, 290, "Image - black");
				image_black.draw(canvas, 300, 300);

				small_font.draw_text(canvas, 300, 490, "Image - Scale (1.5, 2.5)");
				image_texture.set_scale(1.5f, 2.5f);
				image_texture.draw(canvas, 300, 500);
				image_texture.set_scale(1.0f, 1.0f);

				small_font.draw_text(canvas, 450, 460, "Image - Alignment (4 images with 8 pixel offset)");
				small_font.draw_text(canvas, 450, 475, "(top left, top right, bottom left, bottom right)");
				small_font.draw_text(canvas, 450, 490, "(Circle denotes the draw origin)");
				const int offset = 96;

				image_texture.set_alignment(origin_top_left, 8, 8);
				image_texture.draw(canvas, 450+offset, 500+offset);
				image_texture.set_alignment(origin_top_right, -8, 8);
				image_texture.draw(canvas, 450+offset, 500+offset);

				image_texture.set_alignment(origin_bottom_left, 8, -8);
				image_texture.draw(canvas, 450+offset, 500+offset);
				image_texture.set_alignment(origin_bottom_right, -8, -8);
				image_texture.draw(canvas, 450+offset, 500+offset);

				canvas.circle(450+offset, 500+offset, 4, Colorf(1.0f, 1.0f, 1.0f, 0.9f));

				small_font.draw_text(canvas, 700, 460, "Image - Center Alignment (4 images with 8 pixel offset)");
				small_font.draw_text(canvas, 700, 475, "(top center, right center, bottom center, left center)");
				small_font.draw_text(canvas, 700, 490, "(Circle denotes the draw origin)");
				
				image_texture.set_alignment(origin_top_center, 0, 8);
				image_texture.draw(canvas, 700+offset, 500+offset);
				image_texture.set_alignment(origin_bottom_center, 0, -8);
				image_texture.draw(canvas, 700+offset, 500+offset);

				image_texture.set_alignment(origin_center_left, 8, 0);
				image_texture.draw(canvas, 700+offset, 500+offset);
				image_texture.set_alignment(origin_center_right, -8, 0);
				image_texture.draw(canvas, 700+offset, 500+offset);

				canvas.circle(700+offset, 500+offset, 4, Colorf(1.0f, 1.0f, 1.0f, 0.9f));

				small_font.draw_text(canvas, 700, 160, "Image - Center Align (4 images with 64 pixel offset)");
				small_font.draw_text(canvas, 700, 175, "Also Includes a centered image (Without offset)");
				small_font.draw_text(canvas, 700, 190, "(Circle denotes the draw origin)");

				const int center_image_offset = 64;

				image_texture.set_alignment(origin_center, 0, 0);
				image_texture.draw(canvas, 700+offset, 200+offset);

				image_texture.set_alignment(origin_center, 0, center_image_offset);
				image_texture.draw(canvas, 700+offset, 200+offset);
				image_texture.set_alignment(origin_center, 0, -center_image_offset);
				image_texture.draw(canvas, 700+offset, 200+offset);

				image_texture.set_alignment(origin_center, center_image_offset, 0);
				image_texture.draw(canvas, 700+offset, 200+offset);
				image_texture.set_alignment(origin_center, -center_image_offset, 0);
				image_texture.draw(canvas, 700+offset, 200+offset);

				canvas.circle(700+offset, 200+offset, 4, Colorf(1.0f, 1.0f, 1.0f, 0.9f));

				// Restore alignment
				image_texture.set_alignment(origin_top_left, 0, 0);

				dump_fps();

				window.flip(1);
				KeepAlive::process();
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

	void on_window_close()
	{
		quit = true;
	}

	int dump_fps()
	{
		// Calculate time since last update
		static clan::ubyte64 start_time = System::get_time();
		static int fps_dump_time = 0;

		clan::ubyte64 cur_time = System::get_time();
		int delta_time = cur_time - start_time;
		start_time = cur_time;

		int fps = calc_fps(delta_time);

		fps_dump_time += delta_time;
		if(fps_dump_time >= 1000)
		{
			Console::write_line("fps:" + StringHelp::int_to_text(fps));
			fps_dump_time = 0;
		}

		return delta_time;
	}

	int calc_fps(int frame_time)
	{
		static int fps_result = 0;
		static int fps_counter = 0;
		static int total_time = 0;

		total_time += frame_time;
		if(total_time >= 1000)	// One second has passed
		{
			fps_result = fps_counter + 1;
			fps_counter = 0;
			total_time -= 1000;
		}
		fps_counter++;	// Increase fps

		return fps_result;
	}
};

class Program
{
public:
	static int main(const std::vector<std::string> &args)
	{
		SetupCore setup_core; 
		SetupDisplay setup_disp;

#ifdef USE_GDI_BECAUSE_IT_IS_COOL
		SetupD3D setup_d3d;
#else
		SetupGL setup_gl;
#endif
		App app;
		return app.start(args);
	}

};

Application app(&Program::main);
