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
#include "css_view.h"
#include "html_tokenizer.h"
#include "html_token.h"

CSSView::CSSView(CL_GUIComponent *parent)
: CL_GUIComponent(parent), scrollbar(0)
{
	func_resized().set(this, &CSSView::on_resized);
	func_render().set(this, &CSSView::on_render);

	scrollbar = new CL_ScrollBar(this);
	scrollbar->set_vertical();
	scrollbar->set_ranges(0, 30000, 12, 800);
	scrollbar->func_scroll().set(this, &CSSView::on_scroll);

	//load_html("clanlib.html", "clanlib.css");
	//load_html("acid1.xml", "acid1.css");
	//load_html("slashdot.html", "slashdot.css");
	//load_html("politiken.html", "politiken.css");
	load_html("test.xml", "test.css");
	//load_html("zengarden-sample.html", "zengarden-sample.css");
	//load_html("zengarden-sample.html", "zengarden-sample2.css");

	on_resized();
}

CSSView::~CSSView()
{
}

void CSSView::on_resized()
{
	client_box = get_size();
	scrollbar_box = CL_Rect(client_box.right-scrollbar->get_preferred_width(), client_box.top, client_box.right, client_box.bottom);
	view_box = CL_Rect(client_box.left, client_box.top, scrollbar_box.left, client_box.bottom);
	scrollbar->set_geometry(scrollbar_box);
	request_repaint();
}

void CSSView::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_Size size = view_box.get_size();
	if (size != last_layout_size)
	{
		layout.layout(gc, size);
		CL_String s = layout.get_root_element().print_node();
		CL_File f("C:\\Development\\layout debug.txt", CL_File::create_always, CL_File::access_write, CL_File::share_write);
		CL_String8 s8 = CL_StringHelp::text_to_utf8(s);
		f.write(s8.data(), s8.length());
		f.close();
		last_layout_size = size;
	}
	set_cliprect(gc, get_size());
	gc.clear(CL_Colorf::white);
	gc.push_translate(0.0f, (float)-scrollbar->get_position());
	layout.render(gc);
	gc.pop_modelview();
	reset_cliprect(gc);
}

void CSSView::on_scroll()
{
	request_repaint();
}

void CSSView::load_html(const CL_String &html_filename, const CL_String &css_filename)
{
	css_document.add_sheet("default.css");
	css_document.add_sheet(css_filename);

	CL_File file(html_filename, CL_File::open_existing, CL_File::access_read, CL_File::share_read);
	CL_String8 data;
	data.resize(file.get_size());
	data.resize(file.read(data.data(), data.length()));
	file.close();

	HTMLTokenizer tokenizer;
	tokenizer.append(CL_StringHelp::utf8_to_text(data));
	
	CL_DomDocument dom_document;
	int level = 0;
	std::vector<CL_String> tags;
	std::vector<CL_CSSLayoutElement> css_elements;
	std::vector<CL_DomElement> dom_elements;
	
	HTMLToken token;
	while (true)
	{
		tokenizer.tokenize(token);
		if (token.type == HTMLToken::type_null)
			break;

		if (token.type == HTMLToken::type_tag_begin || token.type == HTMLToken::type_tag_single)
		{
			CL_DomElement dom_element = dom_document.create_element(token.name);
			for (size_t i = 0; i < token.attributes.size(); i++)
				dom_element.set_attribute(token.attributes[i].name, token.attributes[i].value);
			if (!dom_elements.empty())
				dom_elements.back().append_child(dom_element);

			if (dom_element.get_attribute(L"class") == "deck140")
			{
				CL_Console::write_line("test");
			}

/*			if (token.name == "img")
			{
				CL_String filename;
				CL_String prop;
				if (dom_element.get_attribute(L"src") == L"gfx/clanlib.png")
				{
					filename = L"clanlib.png";
					prop = L"display: inline-block; width: 323px; height: 86px;";
				}
				else
				{
					filename = L"overview.png";
					prop = L"display: inline-block; width: 48px; height: 48px;";
				}
				CL_ImageView *imgview = new CL_ImageView(this);
				imgview->set_image(CL_PixelBuffer(filename));
				CL_CSSLayoutObject obj = layout.create_object();
				obj.set_component(imgview);

				CL_DomSelectNode select_node(dom_element);
				obj.apply_properties(css_document.select(&select_node));
				obj.apply_properties(dom_element.get_attribute("style"));
				obj.apply_properties(prop);
				if (!css_elements.empty())
					css_elements.back().append_child(obj);
			}
			else*/
			{
				CL_CSSLayoutElement element = layout.create_element();
				element.set_name(cl_format(L"%1.%2", token.name, dom_element.get_attribute(L"class")));

				CL_DomSelectNode select_node(dom_element);
				element.apply_properties(css_document.select(&select_node));
				element.apply_properties(dom_element.get_attribute("style"));
				if (!css_elements.empty())
					css_elements.back().append_child(element);

				if (!css_elements.empty())
				{
					CL_CSSLayoutElement pseudo_before = layout.create_element();
					pseudo_before.set_name(L":before");
					pseudo_before.apply_properties(css_document.select(&select_node, "before"));
					css_elements.back().insert_before(pseudo_before, element);

					CL_CSSLayoutElement pseudo_after = layout.create_element();
					pseudo_after.set_name(L":after");
					pseudo_after.apply_properties(css_document.select(&select_node, "after"));
					css_elements.back().insert_before(pseudo_after, element.get_next_sibling());
				}

				if (is_end_tag_forbidden(token.name))
				{
				}
				else
				{
					level++;
					tags.push_back(token.name);
					css_elements.push_back(element);
					dom_elements.push_back(dom_element);
				}
			}
		}
		else if (token.type == HTMLToken::type_tag_end)
		{
			for (size_t i = tags.size(); i > 0; i--)
			{
				if (tags[i-1] == token.name)
				{
					if (tags.size() > 1)
					{
						tags.erase(tags.begin()+(i-1), tags.end());
						css_elements.erase(css_elements.begin()+(i-1), css_elements.end());
						dom_elements.erase(dom_elements.begin()+(i-1), dom_elements.end());
						level = tags.size();
					}
					break;
				}
			}
		}
		else if (token.type == HTMLToken::type_text)
		{
			if (!css_elements.empty())
			{
				CL_CSSLayoutText text = layout.create_text(token.value);
				css_elements.back().append_child(text);
			}
		}
	}

	if (!css_elements.empty())
		layout.set_root_element(css_elements.front());
}

bool CSSView::is_end_tag_forbidden(const CL_String &name)
{
	return name == "meta" ||
		name == "br" ||
		name == "col" ||
		name == "link" ||
		name == "base" ||
		name == "img" ||
		name == "param" ||
		name == "area" ||
		name == "basefont" ||
		name == "hr" ||
		name == "input" ||
		name == "isindex";
}
