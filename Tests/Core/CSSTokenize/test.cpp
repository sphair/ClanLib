/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

#include "precomp.h"
#include "test.h"
#include "css_browser.h"

CL_ClanApplication clanapp(&TestApp::main);

int TestApp::main(const std::vector<CL_String> &args)
{
	CL_SetupCore setup_core;
	CL_SetupDisplay setup_display;
	CL_SetupSWRender setup_swrender;

	try
	{
		CL_GUIManager gui("../../../Resources/GUIThemeAero");
		CSSBrowser browser(&gui);
		return gui.exec();
	}
	catch(CL_Exception error)
	{
		//CL_Console::write_line("Unhandled exception: %1", error.message);
		MessageBox(0, error.get_message_and_stack_trace().c_str(), "Uncaught Exception", MB_OK);
		return -1;
	}
}
#ifdef NOT_USED
/*
void TestApp::run_test()
{
	CSSDocument doc;
	doc.add_sheet("test.css");

	CSSSelectNode node;
	node.name = "a";
	std::vector<CSSProperty> properties = doc.select(&node);
	for (size_t i = 0; i < properties.size(); i++)
	{
		CL_Console::write_line(properties[i].name);
	}
}
*/
void TestApp::run_test()
{
	CL_CSSLayout layout;
	layout.load_xml("test.xml", "test.css");
	//layout.load_xml("acid1.xml", "acid1.css");
	//print_node(layout.get_root_element());
	//print_layout(layout.get_root_layout());

	CL_DisplayWindow window("Hello CSS", 640, 480, false, true);
	CL_GraphicContext gc = window.get_gc();
	CL_InputContext ic = window.get_ic();
	while (!ic.get_keyboard().get_keycode(CL_KEY_ESCAPE))
	{
		gc.clear(CL_Colorf::white);
		layout.layout(gc, gc.get_size());
		layout.render(gc);
		window.flip();
		CL_KeepAlive::process(-1);
	}
}
/*
void TestApp::print_layout(CSSLayoutNode *node, int indent)
{
	CSSBlockLayout *block_layout = dynamic_cast<CSSBlockLayout*>(node);
	CSSInlineLayout *inline_layout = dynamic_cast<CSSInlineLayout*>(node);
	if (block_layout)
	{
		CL_Console::write(CL_String(indent*2, ' '));
		CL_Console::write_line("block %1", block_layout->element_node->name);
		for (size_t i = 0; i < block_layout->children.size(); i++)
		{
			print_layout(block_layout->children[i], indent+1);
		}
	}
	else if (inline_layout)
	{
		CL_Console::write(CL_String(indent*2, ' '));
		for (size_t i = 0; i< inline_layout->objects.size(); i++)
		{
			CSSBoxText *text = dynamic_cast<CSSBoxText*>(inline_layout->objects[i].node);
			CSSBoxElement *element = dynamic_cast<CSSBoxElement*>(inline_layout->objects[i].node);
			if (text)
			{
				CL_Console::write("[%1] ", escape_text(text->text));
			}
			else if (element)
			{
				CL_Console::write("<%1>", element->name);
			}
		}
		CL_Console::write_line("");
	}
}
*/
void TestApp::print_node(CL_CSSBoxNode *node, int indent)
{
	CL_Console::write(CL_String(indent*2, ' '));

	CL_CSSBoxElement *element = dynamic_cast<CL_CSSBoxElement*>(node);
	CL_CSSBoxText *text = dynamic_cast<CL_CSSBoxText*>(node);

	if (element)
	{
		CL_Console::write_line("%1 { display: %2; float: %3; width: %4 }", element->name, to_string(element->computed_properties.display), to_string(element->computed_properties.float_box), to_string(element->computed_properties.width));
	}
	else if (text)
	{
		CL_Console::write_line("[%1]", escape_text(text->text));
	}
	else
	{
		CL_Console::write_line("unknown");
	}
	//Sleep(50);

	CL_CSSBoxNode *cur = node->get_first_child();
	while (cur)
	{
		print_node(cur, indent+1);
		cur = cur->get_next_sibling();
	}
}

CL_String TestApp::escape_text(CL_String text)
{
	while (true)
	{
		CL_String::size_type pos = text.find('\r');
		if (pos == CL_String::npos)
			break;
		text.replace(pos, 1, "\\r");
	}
	while (true)
	{
		CL_String::size_type pos = text.find('\n');
		if (pos == CL_String::npos)
			break;
		text.replace(pos, 1, "\\n");
	}
	while (true)
	{
		CL_String::size_type pos = text.find('\t');
		if (pos == CL_String::npos)
			break;
		text.replace(pos, 1, "\\t");
	}
	return text;
}

CL_String TestApp::to_string(const CL_CSSBoxDisplay &display)
{
	switch (display.type)
	{
	default: return "unknown";
	case CL_CSSBoxDisplay::type_inline: return "inline";
	case CL_CSSBoxDisplay::type_block: return "block";
	case CL_CSSBoxDisplay::type_list_item: return "list-item";
	case CL_CSSBoxDisplay::type_run_in: return "run-in";
	case CL_CSSBoxDisplay::type_inline_block: return "inline-block";
	case CL_CSSBoxDisplay::type_table: return "table";
	case CL_CSSBoxDisplay::type_inline_table: return "inline-table";
	case CL_CSSBoxDisplay::type_table_row_group: return "table-row-group";
	case CL_CSSBoxDisplay::type_table_header_group: return "table-header-group";
	case CL_CSSBoxDisplay::type_table_footer_group: return "table-footer-group";
	case CL_CSSBoxDisplay::type_table_row: return "table-row";
	case CL_CSSBoxDisplay::type_table_column_group: return "table-column-group";
	case CL_CSSBoxDisplay::type_table_column: return "table-column";
	case CL_CSSBoxDisplay::type_table_cell: return "table-cell";
	case CL_CSSBoxDisplay::type_table_caption: return "table-caption";
	case CL_CSSBoxDisplay::type_none: return "none";
	case CL_CSSBoxDisplay::type_inherit: return "inherit";
	}
}

CL_String TestApp::to_string(const CL_CSSBoxColor &color)
{
	switch (color.type)
	{
	default: return "unknown";
	case CL_CSSBoxColor::type_color: return cl_format("rgb(%1,%2,%3)", (int)(color.color.r*255.0f), (int)(color.color.g*255.0f), (int)(color.color.b*255.0f));
	case CL_CSSBoxColor::type_inherit: return "inherit";
	}
}

CL_String TestApp::to_string(const CL_CSSBoxFontSize &font_size)
{
	switch (font_size.type)
	{
	default: return "unknown";
	case CL_CSSBoxFontSize::type_xx_small: return "xx-small";
	case CL_CSSBoxFontSize::type_x_small: return "x-small";
	case CL_CSSBoxFontSize::type_small: return "small";
	case CL_CSSBoxFontSize::type_medium: return "medium";
	case CL_CSSBoxFontSize::type_large: return "large";
	case CL_CSSBoxFontSize::type_x_large: return "x-large";
	case CL_CSSBoxFontSize::type_xx_large: return "xx-large";
	case CL_CSSBoxFontSize::type_larger: return "larger";
	case CL_CSSBoxFontSize::type_smaller: return "smaller";
	case CL_CSSBoxFontSize::type_length: return cl_format("%1", font_size.length.value);
	case CL_CSSBoxFontSize::type_percentage: return cl_format("%1%%", font_size.percentage);
	case CL_CSSBoxFontSize::type_inherit: return "inherit";
	}
}

CL_String TestApp::to_string(const CL_CSSBoxFloat &float_box)
{
	switch (float_box.type)
	{
	default: return "unknown";
	case CL_CSSBoxFloat::type_none: return "none";
	case CL_CSSBoxFloat::type_left: return "left";
	case CL_CSSBoxFloat::type_right: return "right";
	case CL_CSSBoxFloat::type_inherit: return "inherit";
	}
}

CL_String TestApp::to_string(const CL_CSSBoxWidth &width)
{
	switch (width.type)
	{
	default: return "unknown";
	case CL_CSSBoxWidth::type_auto: return "auto";
	case CL_CSSBoxWidth::type_length: return "length";
	case CL_CSSBoxWidth::type_percentage: return "percentage";
	case CL_CSSBoxWidth::type_inherit: return "inherit";
	}
}
#endif
