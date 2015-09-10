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
**    Mark Page
*/

#include "precomp.h"
#include "language.h"

// #define ENABLE_THIS_IF_YOU_WANT_TO_USE_FRIBIDI
// See http://clanlib.org/external-libraries.html - For Visual Studio download of fribidi

#ifdef ENABLE_THIS_IF_YOU_WANT_TO_USE_FRIBIDI
#define DONT_HAVE_FRIBIDI_CONFIG_H 1
#define FRIBIDI_CHARSETS 0
#define FRIBIDI_USE_GLIB 0
#define FRIBIDI_NO_DEPRECATED 0
#define FRIBIDI_ENTRY

#ifdef DEBUG
#pragma comment(lib, "fribidi-static-mt-debug.lib")
#else
#pragma comment(lib, "fribidi-static-mt.lib")
#endif

#include <fribidi/fribidi.h>
#endif

clan::ApplicationInstance<Language> clanapp;

Language::Language()
{
#ifdef WIN32
	clan::D3DTarget::set_current();
#else
	clan::OpenGLTarget::set_current();
#endif

	// Set the window
	clan::DisplayWindowDescription desc;
	desc.set_title("ClanLib Language Example");
	desc.set_size(clan::Size(640, 480), true);
	desc.set_allow_resize(true);

	window = clan::DisplayWindow(desc);

	// Connect the Window close event
 	sc.connect(window.sig_window_close(), clan::bind_member(this, &Language::on_window_close));

	// Connect a keyboard handler to on_key_up()
	sc.connect(window.get_keyboard().sig_key_up(), clan::bind_member(this, &Language::on_input_up));

	// Get the graphic context
	canvas = clan::Canvas(window);

	clan::File file("Resources/test.xml");
	document = clan::DomDocument(file);
	document_element = document.get_document_element();
	if (document_element.is_null())
		throw clan::Exception("Cannot obtain the document element");

	font_english = clan::Font("arial", 30);

	clan::FontDescription desc_chinese;
	desc_chinese.set_height(30);
	desc_chinese.set_charset(clan::FontDescription::charset_chinesebig5);
	font_chinese = clan::Font("simsun", desc_chinese);

	clan::FontDescription desc_arabic;
	desc_arabic.set_height(30);
	desc_arabic.set_charset(clan::FontDescription::charset_arabic);
	font_arabic = clan::Font("arial", desc_arabic);

}

bool Language::update()
{
	canvas.clear(clan::Colorf(0.0f,0.0f,0.2f));
		
	std::string text;
		
	text = document_element.get_child_string("ENGLISH");
	font_english.draw_text(canvas, 10, 60, text);

	text = document_element.get_child_string("CHINESE");
	font_chinese.draw_text(canvas, 10, 130, text);

#ifdef ENABLE_THIS_IF_YOU_WANT_TO_USE_FRIBIDI
	text = document_element.get_child_string("ARABIC");

	/* input */
	std::wstring text_16 = clan::StringHelp::utf8_to_ucs2(text);
	FriBidiChar *fri_str = (FriBidiChar *) text_16.c_str();
	FriBidiStrIndex fri_len = text_16.length();
	FriBidiCharType fri_base_dir = FRIBIDI_TYPE_ON;
	FriBidiCharType *fri_pbase_dir = &fri_base_dir;

	/* output */
	std::vector<wchar_t> output_buffer;
	output_buffer.resize(text_16.length() + 1);
	FriBidiChar *fri_visual_str = &output_buffer[0];
	FriBidiStrIndex *fri_position_L_to_V_list = NULL;
	FriBidiStrIndex *fri_position_V_to_L_list = NULL;
	FriBidiLevel    *fri_embedding_level_list = NULL;
	fribidi_boolean fri_result;
	fri_result = fribidi_log2vis(fri_str,  fri_len, fri_pbase_dir, fri_visual_str, fri_position_L_to_V_list, fri_position_V_to_L_list, fri_embedding_level_list);
	if (fri_result)
	{
		output_buffer[text_16.length()] = 0;
		std::string new_text = clan::StringHelp::ucs2_to_utf8(&output_buffer[0]);
		font_arabic.draw_text(canvas, 10, 230, new_text);
	}
#endif
	window.flip(1);

	return !quit;
}

// A key was pressed
void Language::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
	{
		quit = true;
	}
}

// The window was closed
void Language::on_window_close()
{
	quit = true;
}


