/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
**    Magnus Norddahl
**    Mark Page
*/

#include "precomp.h"
#include "canvas.h"

clan::ApplicationInstance<ExampleCanvas> clanapp;

ExampleCanvas::ExampleCanvas()
{
	// We support all display targets, in order listed here
#ifdef WIN32
	clan::D3DTarget::set_current();
#endif
	clan::OpenGLTarget::set_current();

	quit = false;

	// Set a videomode - 1024x256
	DisplayWindowDescription desc;
	desc.set_allow_resize(false);
	desc.set_title("ClanLib Draw To Texture Example");
	desc.set_size(Size(1024, 256), true);
	window = DisplayWindow(desc);
	// Connect the Window close event
	sc.connect(window.sig_window_close(), clan::bind_member(this, &ExampleCanvas::on_window_close));

	// Connect a keyboard handler to on_key_up()
	sc.connect(window.get_ic().get_keyboard().sig_key_up(), clan::bind_member(this, &ExampleCanvas::on_keyboard_up));

	// Connect a mouse handler to on_key_down()
	sc.connect(window.get_ic().get_mouse().sig_key_up(), clan::bind_member(this, &ExampleCanvas::on_mouse_down));
	sc.connect(window.get_ic().get_mouse().sig_pointer_move(), clan::bind_member(this, &ExampleCanvas::on_mouse_move));

	canvas_window = Canvas(window);

	// Load the surfaces
	ImageImportDescription import_desc;
	import_desc.set_cached(false);

	Texture2D ground_texture = Texture2D(canvas_window, "Resources/ground.png", import_desc);
	image_ground = Image(ground_texture, ground_texture.get_size() );
	image_ground_back = Image(canvas_window, "Resources/ground.png", import_desc);
	image_cutter = Image(canvas_window, "Resources/circle1.png");

	// Create the framebuffer, and attach ground texture into its color buffer
	framebuffer_ground = FrameBuffer(canvas_window);
	framebuffer_ground.attach_color(0, ground_texture);

	canvas_framebuffer = Canvas(canvas_window, framebuffer_ground);

	BlendStateDescription blend_desc;
	blend_desc.enable_blending(true);
	blend_desc.set_blend_function( blend_zero, blend_one_minus_src_alpha, blend_zero, blend_one_minus_src_alpha );
	BlendState blend_state(canvas_framebuffer.get_gc(), blend_desc);
	canvas_framebuffer.get_gc().set_blend_state(blend_state);

}

bool ExampleCanvas::update()
{
	// Draw a nice blue gradient in the background
	canvas_window.fill_rect(window.get_viewport(), Gradient(Colorf::lightblue, Colorf::lightblue, Colorf::darkblue, Colorf::darkblue));

	// Draw the moving background
	back_pos+= 1.0f;
	image_ground_back.set_color(Colorf(0.5f, 0.5f, 0.5f, 1.0f));
	image_ground_back.draw(canvas_window, sinf(back_pos/100.0f)*100.0f, -20.0f);

	// Draw the ground
	image_ground.draw(canvas_window, 0, 0);
	
	// Draw mouse cursor
	image_cutter.set_scale(scale, scale);
	image_cutter.draw(canvas_window, mouse_pos.x - image_cutter.get_width() * 0.5f * scale, mouse_pos.y - image_cutter.get_height() * 0.5f * scale);

	// Flip the display, showing on the screen what we have drawn
	window.flip(1);

	return !quit;
}


void ExampleCanvas::on_keyboard_up(const InputEvent &key)
{
	if(key.id == keycode_escape)
		quit = true;
}

void ExampleCanvas::on_mouse_move(const InputEvent &key)
{
	mouse_pos = key.mouse_pos;
}

void ExampleCanvas::on_mouse_down(const InputEvent &key)
{
	if(key.id == mouse_left)
	{
		image_cutter.set_scale(scale, scale);
		image_cutter.draw(canvas_framebuffer, mouse_pos.x - image_cutter.get_width() * 0.5f * scale, mouse_pos.y - image_cutter.get_height() * 0.5f * scale);
	}
	
	if(key.id == mouse_wheel_up)
	{
		scale = scale * 1.1f;
	}

	if(key.id == mouse_wheel_down)
	{
		scale = scale * 0.9f;
	}
}

void ExampleCanvas::on_window_close()
{
	quit = true;
}

