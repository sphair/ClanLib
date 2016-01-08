/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Mark Page
*/

#pragma once


class App : public clan::Application
{
public:
	App();
	bool update() override;

private:
	void on_input_up(const clan::InputEvent &key);
	void on_window_close();

	void read_write_pixel_buffer(clan::Canvas &canvas, clan::PixelBuffer &pbuff);
	void upload_pixel_buffer(clan::Canvas &canvas, clan::PixelBuffer &pbuff_source, clan::PixelBuffer &pbuff_dest);

	void draw_cpu(clan::Canvas &canvas, clan::PixelBuffer &cpu_buffer, clan::PixelBuffer &tux, clan::Texture2D &texture_to_write_into, clan::Texture2D &texture_to_draw);
	void draw_gpu(clan::Canvas &canvas, clan::PixelBuffer &gpu_buffer_to_write_into, clan::PixelBuffer &gpu_buffer_to_draw, clan::PixelBuffer &tux, clan::Texture2D &texture_to_write_into, clan::Texture2D &texture_to_draw);
	void draw_texture(clan::Canvas &canvas, clan::Texture2D &texture, int xpos, int ypos);
private:
	clan::DisplayWindow window;
	clan::SlotContainer sc;
	clan::Canvas canvas;
	clan::Font font;
	clan::PixelBuffer tux;
	clan::PixelBuffer cpu_buffer;

	static const int num_gpu_buffers = 2;
	clan::TransferTexture gpu_buffer[num_gpu_buffers];

	static const int num_textures = 2;
	clan::Texture2D textures[num_textures];

	bool quit = false;
	bool cpu_active = true;
	int gpu_buffer_cycle = 0;
	int texture_cycle = 0;

	clan::GameTime game_time;

};
