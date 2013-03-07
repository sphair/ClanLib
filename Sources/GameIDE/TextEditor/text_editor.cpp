/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
*/

#include "GameIDE/precomp.h"
#include "text_editor.h"
#include "text_editor_ribbon.h"
#include "API/GameIDE/UIController/ui_controller.h"

namespace clan
{

#define SCWS_INVISIBLE 0
#define SCWS_VISIBLEALWAYS 1
#define SCWS_VISIBLEAFTERINDENT 2

TextEditor::TextEditor(UIController *controller, std::string filename)
: DocumentEditor(controller, filename), textedit(0)
{
	set_title(PathHelp::get_filename(filename));
	set_dockable_window_geometry(Rect(Point(50, 50), Size(300, 400)));

	textedit = new ScintillaEditor(this);
	textedit->set_class("borderless", true);
	textedit->set_tab_indents(true);
	textedit->set_tab_width(4);
	textedit->func_macro_record().set(this, &TextEditor::on_macro_recorded);
	textedit->func_save_point_reached().set(this, &TextEditor::on_save_point_reached);
	textedit->func_save_point_left().set(this, &TextEditor::on_save_point_left);

	initialize_xml_style_and_document(PathHelp::get_extension(filename));
}

void TextEditor::register_ui(UIController *controller)
{
	static bool registered = false;
	if (!registered)
	{
		controller->add_listener(std::shared_ptr<UIControllerListener>(new TextEditorRibbon(controller)));
		registered = true;
	}
}

void TextEditor::load()
{
	textedit->set_text(File::read_text(get_filename()));
	textedit->empty_undo_buffer();
	textedit->set_savepoint();
}

void TextEditor::save()
{
	File file(get_filename(), File::create_always, File::access_read_write);
	std::string text = textedit->get_text();
	file.write(text.data(), text.length());
	textedit->set_savepoint();
}

void TextEditor::undo()
{
	textedit->undo();
}

void TextEditor::redo()
{
	textedit->redo();
}

void TextEditor::lowercase()
{
	textedit->lowercase();
}

void TextEditor::uppercase()
{
	textedit->uppercase();
}

void TextEditor::copy()
{
	textedit->copy();
}

void TextEditor::cut()
{
	textedit->cut();
}

void TextEditor::paste()
{
	textedit->paste();
}

void TextEditor::macro_run()
{
	for (size_t i = 0; i < recorded_macro.size(); i++)
		textedit->play_record(recorded_macro[i]);
}

void TextEditor::macro_record()
{
	recorded_macro.clear();
	textedit->start_record();
}

void TextEditor::macro_stop()
{
	textedit->stop_record();
}

void TextEditor::on_macro_recorded(const std::string &recorded)
{
	recorded_macro.push_back(recorded);
}

void TextEditor::on_save_point_reached()
{
	set_dirty(false);
}

void TextEditor::on_save_point_left()
{
	set_dirty(true);
}

void TextEditor::eol_checked()
{
	textedit->set_view_eol(true);
}

void TextEditor::eol_unchecked()
{
	textedit->set_view_eol(false);
}

void TextEditor::wsmode_invisible()
{
	textedit->set_view_ws(SCWS_INVISIBLE);
}

void TextEditor::wsmode_visible_always()
{
	textedit->set_view_ws(SCWS_VISIBLEALWAYS);
}

void TextEditor::wsmode_visible_after_indent()
{
	textedit->set_view_ws(SCWS_VISIBLEAFTERINDENT);
}

void TextEditor::on_resized()
{
	textedit->set_geometry(get_size());
}

void TextEditor::initialize_xml_style_and_document(std::string language)
{
#ifndef USE_MONOKAI_DARK
	// This style is based on Shiny-Light

	if (language == "js")
		language = "escript";
	textedit->set_lexer_language(language);

	for (int i = 0; i < 256; i++)
	{
		textedit->style_set_font(i, "Consolas");
		textedit->style_set_foreground(i, Colorf::black);
	}

	textedit->style_set_foreground(0 /*SCE_H_DEFAULT*/, Colorf::black);
	textedit->style_set_foreground(1 /*SCE_H_TAG*/, Colorf::navy);
	textedit->style_set_foreground(2 /*SCE_H_TAGUNKNOWN*/, Colorf::mintcream);
	textedit->style_set_foreground(3 /*SCE_H_ATTRIBUTE*/, Colorf::darkslategray);
	textedit->style_set_foreground(4 /*SCE_H_ATTRIBUTEUNKNOWN*/, Colorf::darksalmon);
	textedit->style_set_foreground(5 /*SCE_H_NUMBER*/, Colorf::darkgoldenrod);
	textedit->style_set_foreground(6 /*SCE_H_DOUBLESTRING*/, Colorf::darkmagenta);
	textedit->style_set_foreground(7 /*SCE_H_SINGLESTRING*/, Colorf::darkmagenta);
	textedit->style_set_foreground(8 /*SCE_H_OTHER*/, Colorf::darkorange);	// =
	textedit->style_set_foreground(9 /*SCE_H_COMMENT*/, Colorf::crimson); 
	textedit->style_set_foreground(10 /*SCE_H_ENTITY*/, Colorf::darkcyan); // &entity_name; or &#entity_number;
	textedit->style_set_foreground(11 /*SCE_H_TAGEND*/, Colorf::navy); // Is /> only, not </sometag>
	textedit->style_set_foreground(12 /*SCE_H_XMLSTART*/, Colorf::darkblue); // <?
	textedit->style_set_foreground(13 /*SCE_H_XMLEND*/, Colorf::darkblue); // ?>
	textedit->style_set_foreground(17 /*SCE_H_CDATA*/, Colorf::midnightblue); // <![CDATA[ to ]]>
	textedit->style_set_foreground(21 /*SCE_H_SGML_DEFAULT*/, Colorf::steelblue); // <! >
	textedit->style_set_foreground(22 /*SCE_H_SGML_COMMAND*/, Colorf::slateblue); 
	textedit->style_set_foreground(23 /*SCE_H_SGML_1ST_PARAM*/, Colorf::sandybrown); 
	textedit->style_set_foreground(24 /*SCE_H_SGML_DOUBLESTRING*/, Colorf::royalblue); 
	textedit->style_set_foreground(25 /*SCE_H_SGML_SIMPLESTRING*/, Colorf::rosybrown); 
	textedit->style_set_foreground(26 /*SCE_H_SGML_ERROR*/, Colorf::seagreen); 
	textedit->style_set_foreground(27 /*SCE_H_SGML_SPECIAL*/, Colorf::palegoldenrod); 
	textedit->style_set_foreground(28 /*SCE_H_SGML_ENTITY*/, Colorf::paleturquoise); 
	textedit->style_set_foreground(29 /*SCE_H_SGML_COMMENT*/, Colorf::papayawhip); 
	textedit->style_set_foreground(30 /*SCE_H_SGML_1ST_PARAM_COMMENT*/, Colorf::peachpuff);
	textedit->style_set_foreground(31 /*SCE_H_SGML_BLOCK_DEFAULT*/, Colorf::powderblue); 


#else
	// This style is based on Monokai-Dark
	// https://github.com/Anomareh/Monokai-Dark.tmTheme/blob/master/Monokai%20Dark.tmTheme
	// 
	textedit->set_lexer_language("xml");
	for (int i = 0; i < 256; i++)
	{
		textedit->style_set_font(i, "Consolas");
		textedit->style_set_bold(i, false);
		textedit->style_set_foreground(i, i % 4 == 0 ? Colorf::blue : Colorf::black);
		textedit->style_set_background(i, Colorf("#272822"));
	}

	textedit->style_set_foreground(0 /*SCE_H_DEFAULT*/, Colorf("#ffffff"));
	textedit->style_set_foreground(1 /*SCE_H_TAG*/, Colorf("#f92672"));
	textedit->style_set_foreground(2 /*SCE_H_TAGUNKNOWN*/, Colorf("#ff0000"));	// #f92672
	textedit->style_set_foreground(3 /*SCE_H_ATTRIBUTE*/, Colorf("#a6e22e"));
	textedit->style_set_foreground(4 /*SCE_H_ATTRIBUTEUNKNOWN*/, Colorf("#ff0000")); // #a6e22e
	textedit->style_set_foreground(5 /*SCE_H_NUMBER*/, Colorf("#e6db74"));
	textedit->style_set_foreground(6 /*SCE_H_DOUBLESTRING*/, Colorf("#e6db74"));
	textedit->style_set_foreground(7 /*SCE_H_SINGLESTRING*/, Colorf("#e6db74"));
	textedit->style_set_foreground(8 /*SCE_H_OTHER*/, Colorf("#ffffff"));	// =
	textedit->style_set_foreground(9 /*SCE_H_COMMENT*/, Colorf("#75715e")); 
	textedit->style_set_foreground(10 /*SCE_H_ENTITY*/, Colorf("#ae81ff")); // &entity_name; or &#entity_number;
	textedit->style_set_foreground(11 /*SCE_H_TAGEND*/, Colorf("#f92672")); // Is /> only, not </sometag>
	textedit->style_set_foreground(12 /*SCE_H_XMLSTART*/, Colorf("#ffffff")); // <?
	textedit->style_set_foreground(13 /*SCE_H_XMLEND*/, Colorf("#ffffff")); // ?>
	textedit->style_set_foreground(17 /*SCE_H_CDATA*/, Colorf("#e6db74")); // <![CDATA[ to ]]>
	textedit->style_set_foreground(21 /*SCE_H_SGML_DEFAULT*/, Colorf("#ffffff")); // <! >
	textedit->style_set_foreground(22 /*SCE_H_SGML_COMMAND*/, Colorf("#ffffff")); 
	textedit->style_set_foreground(23 /*SCE_H_SGML_1ST_PARAM*/, Colorf("#ffffff")); 
	textedit->style_set_foreground(24 /*SCE_H_SGML_DOUBLESTRING*/, Colorf("#ffffff")); 
	textedit->style_set_foreground(25 /*SCE_H_SGML_SIMPLESTRING*/, Colorf("#ffffff")); 
	textedit->style_set_foreground(26 /*SCE_H_SGML_ERROR*/, Colorf("#ffffff")); 
	textedit->style_set_foreground(27 /*SCE_H_SGML_SPECIAL*/, Colorf("#ffffff")); 
	textedit->style_set_foreground(28 /*SCE_H_SGML_ENTITY*/, Colorf("#ffffff")); 
	textedit->style_set_foreground(29 /*SCE_H_SGML_COMMENT*/, Colorf("#ffffff")); 
	textedit->style_set_foreground(30 /*SCE_H_SGML_1ST_PARAM_COMMENT*/, Colorf("#ffffff"));
	textedit->style_set_foreground(31 /*SCE_H_SGML_BLOCK_DEFAULT*/, Colorf("#ffffff")); 
	
	textedit->style_set_background(32 /* Background */, Colorf("#272822"));
	textedit->style_set_background(33 /* Selection margin */, Colorf("#272822"));
#endif

/*	<LexerType name="xml" desc="XML" ext="">
		<WordsStyle name="XMLSTART" styleID="12" fgColor="FF0000" bgColor="FFFF00" fontName="" fontStyle="0" fontSize=""/>
		<WordsStyle name="XMLEND" styleID="13" fgColor="FF0000" bgColor="FFFF00" fontName="" fontStyle="0" fontSize=""/>
		<WordsStyle name="DEFAULT" styleID="0" fgColor="000000" bgColor="FFFFFF" fontName="" fontStyle="1" fontSize=""/>
		<WordsStyle name="COMMENT" styleID="9" fgColor="008000" bgColor="FFFFFF" fontName="" fontStyle="0" fontSize=""/>
		<WordsStyle name="NUMBER" styleID="5" fgColor="FF0000" bgColor="FFFFFF" fontName="" fontStyle="0" fontSize=""/>
		<WordsStyle name="DOUBLESTRING" styleID="6" fgColor="8000FF" bgColor="FFFFFF" fontName="" fontStyle="1" fontSize=""/>
		<WordsStyle name="SINGLESTRING" styleID="7" fgColor="8000FF" bgColor="FFFFFF" fontName="" fontStyle="1" fontSize=""/>
		<WordsStyle name="TAG" styleID="1" fgColor="0000FF" bgColor="FFFFFF" fontName="" fontStyle="0" fontSize=""/>
		<WordsStyle name="TAGEND" styleID="11" fgColor="0000FF" bgColor="FFFFFF" fontName="" fontStyle="0" fontSize=""/>
		<WordsStyle name="TAGUNKNOWN" styleID="2" fgColor="0000FF" bgColor="FFFFFF" fontName="" fontStyle="0" fontSize=""/>
		<WordsStyle name="ATTRIBUTE" styleID="3" fgColor="FF0000" bgColor="FFFFFF" fontName="" fontStyle="0" fontSize=""/>
		<WordsStyle name="ATTRIBUTEUNKNOWN" styleID="4" fgColor="FF0000" bgColor="FFFFFF" fontName="" fontStyle="0" fontSize=""/>
		<WordsStyle name="SGMLDEFAULT" styleID="21" fgColor="000000" bgColor="A6CAF0" fontName="" fontStyle="0" fontSize=""/>
		<WordsStyle name="CDATA" styleID="17" fgColor="FF8000" bgColor="FFFFFF" fontName="" fontStyle="0" fontSize=""/>
		</LexerType>
*/

/*	<WidgetStyle name="Global override" styleID="0" fgColor="FFFF80" bgColor="FF8000" fontName="Courier New" fontStyle="0" fontSize="10"/>
		<WidgetStyle name="Default Style" styleID="32" fgColor="000000" bgColor="FFFFFF" fontName="Courier New" fontStyle="0" fontSize="10"/>
		<WidgetStyle name="Indent guideline style" styleID="37" fgColor="C0C0C0" bgColor="FFFFFF" fontName="" fontStyle="0" fontSize=""/>
		<WidgetStyle name="Brace highlight style" styleID="34" fgColor="FF0000" bgColor="FFFFFF" fontName="" fontStyle="1" fontSize="12"/>
		<WidgetStyle name="Bad brace colour" styleID="35" fgColor="800000" bgColor="FFFFFF" fontName="" fontStyle="0" fontSize=""/>
		<WidgetStyle name="Current line background colour" styleID="0" bgColor="E8E8FF"/>
		<WidgetStyle name="Selected text colour" styleID="0" bgColor="C0C0C0"/>
		<WidgetStyle name="Caret colour" styleID="2069" fgColor="8000FF"/>
		<WidgetStyle name="Edge colour" styleID="0" fgColor="80FFFF"/>
		<WidgetStyle name="Line number margin" styleID="33" fgColor="808080" bgColor="E4E4E4" fontName="" fontStyle="0" fontSize=""/>
		<WidgetStyle name="Fold" styleID="0" fgColor="808080" bgColor="F3F3F3"/>
		<WidgetStyle name="Fold active" styleID="0" fgColor="FF0000"/>
		<WidgetStyle name="Fold margin" styleID="0" fgColor="FFFFFF" bgColor="E9E9E9"/>
		<WidgetStyle name="White space symbol" styleID="0" fgColor="FFB56A"/>
		<WidgetStyle name="Smart HighLighting" styleID="29" bgColor="00FF00"/>
		<WidgetStyle name="Find Mark Style" styleID="31" bgColor="FF0000"/>
		<WidgetStyle name="Mark Style 1" styleID="25" bgColor="00FFFF"/>
		<WidgetStyle name="Mark Style 2" styleID="24" bgColor="FF8000"/>
		<WidgetStyle name="Mark Style 3" styleID="23" bgColor="FFFF00"/>
		<WidgetStyle name="Mark Style 4" styleID="22" bgColor="8000FF"/>
		<WidgetStyle name="Mark Style 5" styleID="21" bgColor="008000"/>
		<WidgetStyle name="Incremental highlight all" styleID="28" bgColor="0080FF"/>
		<WidgetStyle name="Tags match highlighting" styleID="27" bgColor="8000FF"/>
		<WidgetStyle name="Tags attribute" styleID="26" bgColor="FFFF00"/>
		<WidgetStyle name="Active tab focused indicator" styleID="0" fgColor="FAAA3C"/>
		<WidgetStyle name="Active tab unfocused indicator" styleID="0" fgColor="FFCAB0"/>
		<WidgetStyle name="Active tab text" styleID="0" fgColor="000000"/>
		<WidgetStyle name="Inactive tabs" styleID="0" fgColor="808080" bgColor="C0C0C0"/>
*/}

}
