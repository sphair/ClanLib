/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

#include "precomp.h"
#include "font.h"

clan::ApplicationInstance<App> clanapp;

App::App()
{
#if defined(WIN32) && !defined(__MINGW32__)
	clan::D3DTarget::set_current();
#else
	clan::OpenGLTarget::set_current();
#endif

	DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("Font Example Application");
	win_desc.set_size(Size( 1000, 700 ), false);

	window = DisplayWindow(win_desc);
	slots.connect(window.sig_window_close(), this, &App::on_window_close);
	slots.connect(window.get_keyboard().sig_key_up(), this, &App::on_input_up);

	clan::XMLResourceFactory::set_display();
	resources = clan::XMLResourceManager::create(clan::XMLResourceDocument("Resources/resources.xml"));

	canvas = Canvas(window);

	clan::Texture2D gui_texture = clan::Texture2D(canvas, (int)std::round(250 * canvas.get_pixel_ratio()), (int)std::round(500 * canvas.get_pixel_ratio()));
	gui_texture.set_pixel_ratio(canvas.get_pixel_ratio());
	gui_image = clan::Image(gui_texture, gui_texture.get_size());
	clan::FrameBuffer gui_framebuffer = clan::FrameBuffer(canvas);
	gui_framebuffer.attach_color(0, gui_texture);
	gui_canvas = clan::Canvas(canvas, gui_framebuffer);

	clan::FileResourceDocument doc(clan::FileSystem("../../ThemeAero"));
	clan::ResourceManager resources = clan::FileResourceManager::create(doc);
	ui_thread = clan::UIThread(resources);

	root = std::make_shared<clan::TextureWindow>(gui_canvas);

	root->set_window(window);

	root->set_viewport(gui_image.get_size());

	int offset_x = 10;
	int offset_y = 8;
	int width = 220;
	int small_width = 70;
	int height = 20;
	const int gap = 38;

	auto button_class_system = Theme::create_button();
	button_class_system->style()->set("position: absolute; left:%1px; top:%2px; width:%3px; height:auto;", offset_x, offset_y, width);
	button_class_system->func_clicked() = clan::bind_member(this, &App::on_button_clicked_class_system);
	button_class_system->label()->set_text("Class: System");
	root->add_child(button_class_system);
	offset_y += gap;

	auto button_class_sprite = Theme::create_button();
	button_class_sprite->style()->set("position: absolute; left:%1px; top:%2px; width:%3px; height:auto;", offset_x, offset_y, width);
	button_class_sprite->func_clicked() = bind_member(this, &App::on_button_clicked_class_sprite);
	button_class_sprite->label()->set_text("Class: Sprite");
	root->add_child(button_class_sprite);
	offset_y += gap;

	button_typeface_tahoma = Theme::create_button();
	button_typeface_tahoma->style()->set("position: absolute; left:%1px; top:%2px; width:%3px; height:auto;", offset_x, offset_y, width);
	button_typeface_tahoma->func_clicked() = bind_member(this, &App::on_button_clicked_typeface_tahoma);
	button_typeface_tahoma->label()->set_text("Typeface: Tahoma");
	root->add_child(button_typeface_tahoma);
	offset_y += gap;

	button_typeface_sans = Theme::create_button();
	button_typeface_sans->style()->set("position: absolute; left:%1px; top:%2px; width:%3px; height:auto;", offset_x, offset_y, width);
	button_typeface_sans->func_clicked() = bind_member(this, &App::on_button_clicked_typeface_sans);
	button_typeface_sans->label()->set_text("Typeface: Microsoft Sans Serif");
	root->add_child(button_typeface_sans);
	offset_y += gap;

	button_typeface_bitstream = Theme::create_button();
	button_typeface_bitstream->style()->set("position: absolute; left:%1px; top:%2px; width:%3px; height:auto;", offset_x, offset_y, width);
	button_typeface_bitstream->func_clicked() = bind_member(this, &App::on_button_clicked_typeface_bitstream);
	button_typeface_bitstream->label()->set_text("Typeface: Bitstream Vera Sans");
	root->add_child(button_typeface_bitstream);
	offset_y += gap;

	checkbox_italic = Theme::create_checkbox();
	checkbox_italic->style()->set("position: absolute; left:%1px; top:%2px", offset_x, offset_y);
	checkbox_italic->func_state_changed() = bind_member(this, &App::on_checkbox_state_italic);
	root->add_child(checkbox_italic);
	auto label = Theme::create_label();
	label->set_text("Italic");
	label->style()->set("position: absolute; left:%1px; top:%2px", offset_x + 16, offset_y - 3);
	root->add_child(label);

	checkbox_antialias = Theme::create_checkbox();
	checkbox_antialias->set_check(true);
	checkbox_antialias->style()->set("position: absolute; left:%1px; top:%2px", offset_x + 100, offset_y);
	checkbox_antialias->func_state_changed() = bind_member(this, &App::on_checkbox_state_antialias);
	root->add_child(checkbox_antialias);
	label = Theme::create_label();
	label->set_text("Anti Alias");
	label->style()->set("position: absolute; left:%1px; top:%2px", offset_x + 100+ 16, offset_y - 3);
	root->add_child(label);
	offset_y += gap;

	checkbox_subpixel = Theme::create_checkbox();
	checkbox_subpixel->set_check(true);
	checkbox_subpixel->style()->set("position: absolute; left:%1px; top:%2px", offset_x, offset_y);
	checkbox_subpixel->func_state_changed() = bind_member(this, &App::on_checkbox_state_subpixel);
	root->add_child(checkbox_subpixel);
	label = Theme::create_label();
	label->set_text("SubPixel Rendering");
	label->style()->set("position: absolute; left:%1px; top:%2px", offset_x + 16, offset_y - 3);
	root->add_child(label);
	offset_y += gap;

	auto button_weight_light = Theme::create_button();
	button_weight_light->style()->set("position: absolute; left:%1px; top:%2px; width:%3px; height:auto;", offset_x, offset_y, small_width);
	button_weight_light->func_clicked() = bind_member(this, &App::on_button_clicked_weight_light);
	button_weight_light->label()->set_text("Light");
	root->add_child(button_weight_light);
	auto button_weight_normal = Theme::create_button();
	button_weight_normal->style()->set("position: absolute; left:%1px; top:%2px; width:%3px; height:auto;", offset_x + small_width + 5, offset_y, small_width);
	button_weight_normal->func_clicked() = bind_member(this, &App::on_button_clicked_weight_normal);
	button_weight_normal->label()->set_text("Normal");
	root->add_child(button_weight_normal);
	auto button_weight_bold = Theme::create_button();
	button_weight_bold->style()->set("position: absolute; left:%1px; top:%2px; width:%3px; height:auto;", offset_x + (small_width + 5) * 2, offset_y, small_width);
	button_weight_bold->func_clicked() = bind_member(this, &App::on_button_clicked_weight_bold);
	button_weight_bold->label()->set_text("Bold");
	root->add_child(button_weight_bold);
	offset_y += gap;

	auto button_size_16 = Theme::create_button();
	button_size_16->style()->set("position: absolute; left:%1px; top:%2px; width:%3px; height:auto;", offset_x, offset_y, small_width);
	button_size_16->func_clicked() = bind_member(this, &App::on_button_clicked_size_16);
	button_size_16->label()->set_text("Size 16");
	root->add_child(button_size_16);
	auto button_size_32 = Theme::create_button();
	button_size_32->style()->set("position: absolute; left:%1px; top:%2px; width:%3px; height:auto;", offset_x + small_width + 5, offset_y, small_width);
	button_size_32->func_clicked() = bind_member(this, &App::on_button_clicked_size_32);
	button_size_32->label()->set_text("Size 32");
	root->add_child(button_size_32);
	auto button_size_64 = Theme::create_button();
	button_size_64->style()->set("position: absolute; left:%1px; top:%2px; width:%3px; height:auto;", offset_x + (small_width + 5) * 2, offset_y, small_width);
	button_size_64->func_clicked() = bind_member(this, &App::on_button_clicked_size_64);
	button_size_64->label()->set_text("Size 64");
	root->add_child(button_size_64);
	offset_y += gap + 8;

	lineedit_text = std::make_shared<clan::TextFieldView>();
	lineedit_text->style()->set("font: 11px/20px 'Segoe UI'");
	lineedit_text->style()->set("margin: 5px");
	lineedit_text->style()->set("background: #efefef");
	lineedit_text->style()->set("border: 1px solid black");
	lineedit_text->style()->set("border-radius: 3px");
	lineedit_text->style()->set("padding: 2px 5px 2px 5px");
	lineedit_text->style()->set("width: 128px");
	lineedit_text->style()->set("box-shadow: 0 0 5px rgba(100,100,200,0.2)");
	lineedit_text->style()->set("position: absolute; left:%1px; top:%2px; width:%3px; height:auto;", offset_x, offset_y, width);
	font_text = "Ω(The quick brown fox 0123456789)";
	lineedit_text->set_text(font_text);
	slots.connect(lineedit_text->sig_selection_changed(), bind_member(this, &App::on_lineedit_changed));
	root->add_child(lineedit_text);

	last_fps = 0.0f;
	selected_fontclass = font_ttf;
	font_typeface = "Microsoft Sans Serif";
	font_filename = "";
	font_desc.set_height(32);
	font_desc.set_weight(clan::FontWeight::normal);
	select_font();

	small_font = clan::Font("Tahoma", 16);

	premultiply_src_blend = BlendState(canvas, BlendStateDescription::blend(true));

	game_time.reset();
}

bool App::update()
{
	game_time.update();
	render(window, game_time);

	return !quit;
}

// A key was pressed
void App::on_input_up(const InputEvent &key)
{
	if(key.id == keycode_escape)
	{
		quit = true;
	}
}

// The window was closed
void App::on_window_close()
{
	quit = true;
}

void App::render(DisplayWindow &window, GameTime &game_time)
{
	canvas.clear(Colorf(0.0f,0.0f,0.2f, 1.0f));

	root->update();
	canvas.set_blend_state(premultiply_src_blend);
	gui_image.draw(canvas, 0, 0);
	canvas.reset_blend_state();

	draw_font_example();
	draw_font_info();

	last_fps = game_time.get_updates_per_second();

	window.flip(1);
}

void App::select_font()
{
	switch (selected_fontclass)
	{
		case font_ttf:
			if (font_filename.empty())
			{
				selected_font = clan::Font(font_typeface, font_desc);
			}
			else
			{
				selected_font = clan::Font(font_desc, font_filename);
			}
			break;
		case font_sprite:
			selected_font = clan::Font::resource(canvas, font_typeface, font_desc, resources);
			break;
	}

	font_metrics = selected_font.get_font_metrics(canvas);
	font_size = selected_font.measure_text(canvas, font_text).bbox_size;
}

void App::draw_font_example()
{
	int offset_x = 10;
	int offset_y = 600;
	int descender = (int) font_metrics.get_descent();

	// Surrounding box
	canvas.fill_rect(Rect(offset_x, offset_y+descender, offset_x + font_size.width, offset_y+descender - font_size.height), Colorf(0.0f, 0.0f, 0.0f));

	// Draw the external leading line
	int external_leading_offset = offset_y+font_metrics.get_external_leading() + font_metrics.get_descent();
	canvas.draw_line(offset_x, external_leading_offset, offset_x + font_size.width, external_leading_offset, Colorf(0.5f, 0.5f, 0.0f));

	// Draw the internal leading line
	int internal_leading_offset = offset_y - font_metrics.get_ascent() + font_metrics.get_internal_leading();
	canvas.draw_line(offset_x, internal_leading_offset, offset_x + font_size.width, internal_leading_offset, Colorf(0.5f, 0.5f, 0.0f));

	// Draw outline to the bounding box
	canvas.draw_line(offset_x, offset_y+descender, offset_x + font_size.width, offset_y+descender, Colorf(1.0f, 0.0f, 0.0f));
	canvas.draw_line(offset_x, offset_y+descender- font_size.height, offset_x + font_size.width, offset_y+descender - font_size.height, Colorf(1.0f, 0.0f, 0.0f));
	canvas.draw_line(offset_x + font_size.width, offset_y+descender, offset_x + font_size.width, offset_y+descender - font_size.height, Colorf(1.0f, 0.0f, 0.0f));
	canvas.draw_line(offset_x, offset_y+descender, offset_x, offset_y+descender - font_size.height, Colorf(1.0f, 0.0f, 0.0f));

	// Base line
	canvas.draw_line(offset_x, offset_y, offset_x + font_size.width, offset_y, Colorf(0.0f, 1.0f, 0.0f));

	selected_font.draw_text(canvas, offset_x, offset_y, font_text,  Colorf::white);
}
