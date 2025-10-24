/*
	GlyphBuffer Example
*/

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

#include <vector>
#include <map>
#include <iostream>

using namespace std;

void gen_circle_text(CL_GlyphBuffer& gb)
{
	//Use 0,0 as the center of the circle
	//It doesnt really matter where we put the center, as drawing the GlyphBuffer into a rect aligns its contents
	const CL_Point circle_center(0, 0);
	
	//Use 150 as the initial diameter
	const int diameter = 150;
	
	//We move glyphs around by messing the glyphs vector within the GlyphBuffer
	//This vector which contains each character and its position
	vector<CL_GlyphBuffer::Glyph>& glyphs = gb.get_glyphs();
	const int glyphs_count = glyphs.size();
	
	//And we rotate each character by using the angle effects map in GlyphBuffer
	//This map lets you set each glyph's individual angle
	map<int, float>& angle_effects = gb.get_angle_effects();
	
	//Find the average of the widths of every glyph
	float avg = 0.0;
	int x;
	for (x = 0; x < glyphs_count; ++x)
		avg += (float)(gb.get_font_for(x)->second.get_width(glyphs[x].character));
	avg /= (float)glyphs_count;
	
	//The current distance around the circle
	float angle = 0.0;
	
	//Rearrange the glyphs around the center, by moving and rotating each glyph to its new position
	for (x = 0; x < glyphs_count; ++x)
	{
		unsigned char letter = glyphs[x].character; //The character for the current glyph
		float glyph_width = (float)(gb.get_font_for(x)->second.get_width(letter)); //The width of the current glyph
		
		//Our initial position is 0 degrees along the circle's edge, directly above the center
		CL_Point new_pos(circle_center.x, circle_center.y - diameter);
		
		//Rotate the glyph around the circle to our currently calculated angle
		glyphs[x].pos = new_pos.rotate(circle_center, angle);
		
		//Change the glyph's drawing angle
		angle_effects[x] = angle;
		
		//Advance around the circle for this glyph, after having drawn it
		angle += (glyph_width/avg)*(360/glyphs_count);
	}
}

class GlyphBufferApp : public CL_ClanApplication
{
public:
	int main(int, char **)
	{
		try
		{
			CL_SetupCore setup_core;
			CL_SetupDisplay setup_display;
			CL_SetupGL setup_gl;
			
			CL_ResourceManager resources("glyphbuffer.xml");
                	CL_DisplayWindow window("ClanLib GlyphBuffer Example", 640, 480);
			CL_Rect screen_rect(0, 0, 640, 480);
			
			//Load the font from the resource "Font" in the resources file
			CL_Font font("Font", &resources);
			
			//Create a GlyphBuffer, and render some text into it
			CL_GlyphBuffer gb;
			gb.set_alignment(origin_center);
			std::string message;
			message += "A and D to rotate, W and S to resize, ";
			message += "R to reset, Esc to quit. ClanLib: Phear the Power!";
			font.draw_to_gb(message, gb, CL_Size(0,0));
			
			//Post process the text so that it's circular
			gen_circle_text(gb);
			
			//Scale the GlyphBuffer so that it fills the window vertically
			float initial_scale = (float)screen_rect.get_height()/(float)gb.get_height();
			gb.set_scale(initial_scale, initial_scale);
			
			// Connect the Window close event
			CL_Slot slot_quit = window.sig_window_close().connect(this, &GlyphBufferApp::on_window_close);
			quit = false;

			while(!CL_Keyboard::get_keycode(CL_KEY_ESCAPE) && !quit)
			{
				CL_Display::clear(CL_Color::white);
				
				//Reset the circle to its initial state
				if (CL_Keyboard::get_keycode(CL_KEY_R)) {
					gb.set_angle(0);
					gb.set_scale(initial_scale, initial_scale);
				}
				
				//Use the GlyphBuffer rotation property to rotate the glyphs when drawn
				if (CL_Keyboard::get_keycode(CL_KEY_A))
					gb.rotate(-0.5);
				if (CL_Keyboard::get_keycode(CL_KEY_D))
					gb.rotate(0.5);
				
				//Alter the diameter if requested, using the GlyphBuffer scale proprety 
				if (CL_Keyboard::get_keycode(CL_KEY_W))
				{
					float gbscale_x, gbscale_y;
					gb.get_scale(gbscale_x, gbscale_y);
					gb.set_scale(gbscale_x + 0.01f, gbscale_y + 0.01f);
				}
				if (CL_Keyboard::get_keycode(CL_KEY_S))
				{
					float gbscale_x, gbscale_y;
					gb.get_scale(gbscale_x, gbscale_y);
					
					if (gbscale_x > 0.01f)
						gb.set_scale(gbscale_x - 0.01f, gbscale_y - 0.01f);
				}
				
				//Draw the GlyphBuffer centered on-screen (we set gb's alignment to center above)
				gb.draw(screen_rect);
				
				CL_Display::flip();
				CL_System::keep_alive(15);
			}
		}
		catch (CL_Error err)
		{
			std::cout << "Exception caught: " << err.message.c_str() << std::endl;
			return -1;
		}
		
		return 0;
	}

private:
	void on_window_close()
	{
		quit = true;
	}

	bool quit;
} app;
