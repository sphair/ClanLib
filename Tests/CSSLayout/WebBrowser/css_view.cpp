
#include "precomp.h"
#include "css_view.h"

using namespace clan;

CSSView::CSSView(GUIComponent *parent)
: GUIComponent(parent, "cssview"), scrollbar(0)
{
	func_resized().set(this, &CSSView::on_resized);
	func_render().set(this, &CSSView::on_render);

	scrollbar = new ScrollBar(this);
	scrollbar->set_vertical();
	scrollbar->set_ranges(0, 30000, 12, 800);
	scrollbar->func_scroll().set(this, &CSSView::on_scroll);

	on_resized();
}

CSSView::~CSSView()
{
	reset();
}

void CSSView::navigate(const std::string &page_url, const std::string &refererer_url)
{
	//for (size_t i = 0; i < 100; i++)
	{
		page.load(page_url, refererer_url);
		load_html();
	}
	last_layout_viewport = Rect();
	scrollbar->set_position(0);
	request_repaint();
}

void CSSView::on_resized()
{
	client_box = get_size();
	scrollbar_box = Rect(client_box.right-scrollbar->get_width(), client_box.top, client_box.right, client_box.bottom);
	view_box = Rect(client_box.left, client_box.top, scrollbar_box.left, client_box.bottom);
	scrollbar->set_geometry(scrollbar_box);
	request_repaint();
}

void CSSView::on_render(Canvas &canvas, const Rect &update_rect)
{
	if (!layout.is_null())
	{
		if (view_box != last_layout_viewport)
		{
			layout.layout(canvas, view_box);
			/*std::string s = layout.get_root_element().print_node();
			File f("C:\\Development\\layout debug.txt", File::create_always, File::access_write, File::share_write);
			std::string s8 = StringHelp::text_to_utf8(s);
			f.write(s8.data(), s8.length());
			f.close();*/
			last_layout_viewport = view_box;
		}
		set_cliprect(canvas, get_size());
		canvas.clear(Colorf::white);
		canvas.push_translate(0.0f, (float)-scrollbar->get_position());
		layout.render(canvas, this);

		for (size_t i = 0; i < replaced_objects.size(); i++)
			replaced_objects[i]->render(canvas);

		canvas.pop_modelview();
		reset_cliprect(canvas);
	}
}

void CSSView::on_scroll()
{
	request_repaint();
}

Image CSSView::on_layout_get_image(Canvas &canvas, const std::string &uri)
{
	try
	{
		if (image_cache.find(uri) != image_cache.end())
			return image_cache[uri];
		image_cache[uri] = page.load_image(canvas, uri);
		return image_cache[uri];
	}
	catch (Exception e)
	{
		image_cache[uri] = Image();
		return Image();
	}
}

void CSSView::reset()
{
	layout = CSSLayout();
	layout.func_get_image().set(this, &CSSView::on_layout_get_image);
	css_document = CSSDocument();
	for (size_t i = 0; i < replaced_objects.size(); i++)
		delete replaced_objects[i];
	replaced_objects.clear();
	image_cache.clear();
}

void CSSView::load_html()
{
	reset();

	std::string html_filename = "htmlpage.html";
	HTMLUrl document_url = page.pageurl;

	css_document.add_default_html_sheet();
	for (size_t i = 0; i < page.css_files.size(); i++)
	{
		IODevice_Memory device;
		device.write(page.css_files[i].css.data(), page.css_files[i].css.length());
		device.seek(0);
		css_document.add_sheet(author_sheet_origin, device, page.css_files[i].base_uri);
	}

	layout.set_css_document(css_document);

	File file(html_filename, File::open_existing, File::access_read, File::share_read);
	std::string data;
	data.resize(file.get_size());
	if (!data.empty())
		file.read(&data[0], data.length());
	file.close();

	HTMLTokenizer tokenizer;
	tokenizer.append(StringHelp::utf8_to_text(data));

	int level = 0;
	std::vector<std::string> tags;
	std::vector<CSSLayoutElement> css_elements;

	Canvas canvas = get_canvas();

	HTMLToken token;
	while (true)
	{
		tokenizer.tokenize(token);
		if (token.type == HTMLToken::type_null)
			break;

		if (token.type == HTMLToken::type_tag_begin || token.type == HTMLToken::type_tag_single)
		{
			if (token.name == "p" || token.name == "table") // Close <p>
			{
				for (size_t i = tags.size(); i > 0; i--)
				{
					if (tags.back() == "p")
					{
						tags.erase(tags.begin()+(i-1), tags.end());
						css_elements.erase(css_elements.begin()+(i-1), css_elements.end());
						level = tags.size();
						break;
					}
				}
			}

			CSSLayoutElement element;

			if (token.name == "img")
			{
				std::string src = token.get_attribute("src");
				Img *image = new Img(on_layout_get_image(canvas, HTMLUrl(src, document_url).to_string()));
				CSSLayoutObject obj = layout.create_object();
				obj.set_component(image);
				if (!image->image.is_null())
				{
					obj.set_intrinsic_width(image->get_intrinsic_width());
					obj.set_intrinsic_height(image->get_intrinsic_height());
					if (image->has_intrinsic_ratio())
						obj.set_intrinsic_ratio(image->get_intrinsic_ratio());
				}
				replaced_objects.push_back(image);
				element = obj;
			}
			else
			{
				 element = layout.create_element(token.name);
			}

			for (size_t i = 0; i < token.attributes.size(); i++)
				element.set_attribute(token.attributes[i].name, token.attributes[i].value);

			if (!css_elements.empty())
			{
				css_elements.back().append_child(element);
			}

			if (!is_end_tag_forbidden(token.name))
			{
				level++;
				tags.push_back(token.name);
				css_elements.push_back(element);
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
				CSSLayoutText text = layout.create_text(token.value);
				css_elements.back().append_child(text);
			}
		}
	}

	if (!css_elements.empty())
		layout.set_document_element(css_elements.front());
}

bool CSSView::is_end_tag_forbidden(const std::string &name)
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

#if defined(OLD_HTML_LOAD)
void CSSView::load_html()
{
	reset();

	std::string html_filename = "htmlpage.html";
	HTMLUrl document_url = page.pageurl;

	css_document.add_default_html_sheet();
	for (size_t i = 0; i < page.css_files.size(); i++)
	{
		IODevice_Memory device;
		device.write(page.css_files[i].css.data(), page.css_files[i].css.length());
		device.seek(0);
		css_document.add_sheet(author_sheet_origin, device, page.css_files[i].base_uri);
	}

	File file(html_filename, File::open_existing, File::access_read, File::share_read);
	std::string data;
	data.resize(file.get_size());
	if (!data.empty())
		file.read(&data[0], data.length());
	file.close();

	HTMLTokenizer tokenizer;
	tokenizer.append(StringHelp::utf8_to_text(data));

	Canvas canvas = get_canvas();
	
	DomDocument dom_document;
	int level = 0;
	std::vector<std::string> tags;
	std::vector<CSSLayoutElement> css_elements;
	std::vector<DomElement> dom_elements;
	
	HTMLToken token;
	while (true)
	{
		tokenizer.tokenize(token);
		if (token.type == HTMLToken::type_null)
			break;

		if (token.type == HTMLToken::type_tag_begin || token.type == HTMLToken::type_tag_single)
		{
			if (token.name == "p" || token.name == "table") // Close <p>
			{
				for (size_t i = tags.size(); i > 0; i--)
				{
					if (tags.back() == "p")
					{
						tags.erase(tags.begin()+(i-1), tags.end());
						css_elements.erase(css_elements.begin()+(i-1), css_elements.end());
						dom_elements.erase(dom_elements.begin()+(i-1), dom_elements.end());
						level = tags.size();
						break;
					}
				}
			}

			DomElement dom_element = dom_document.create_element(token.name);
			for (size_t i = 0; i < token.attributes.size(); i++)
				dom_element.set_attribute(token.attributes[i].name, token.attributes[i].value);
			if (!dom_elements.empty())
				dom_elements.back().append_child(dom_element);

/*			if (dom_element.get_attribute("class") == "title icon-home")
			{
				Console::write_line("test");
			}*/

			CSSLayoutElement element;
			if (token.name == "img")
			{
				std::string src = dom_element.get_attribute("src");
				Img *image = new Img(on_layout_get_image(canvas, HTMLUrl(src, document_url).to_string()));
				CSSLayoutObject obj = layout.create_object();
				obj.set_component(image);
				if (!image->image.is_null())
				{
					obj.set_intrinsic_width(image->get_intrinsic_width());
					obj.set_intrinsic_height(image->get_intrinsic_height());
					if (image->has_intrinsic_ratio())
						obj.set_intrinsic_ratio(image->get_intrinsic_ratio());
				}
				replaced_objects.push_back(image);
				element = obj;
			}
			else if (token.name == "input")
			{
				std::string type = dom_element.get_attribute("type");
				int size = dom_element.get_attribute_int("size");
				ReplacedObject *replaced = 0;
				if (HTMLTokenizer::compare(type, "text"))
				{
					int num_chars = size;
					replaced = new TextInput(canvas, num_chars);
				}
				else if (HTMLTokenizer::compare(type, "password"))
				{
					int num_chars = size;
					replaced = new TextInput(canvas, num_chars);
				}
				else if (HTMLTokenizer::compare(type, "checkbox"))
				{
					replaced = new CheckboxInput(canvas);
				}
				else if (HTMLTokenizer::compare(type, "radio"))
				{
					replaced = new RadioInput(canvas);
				}
				else if (HTMLTokenizer::compare(type, "submit"))
				{
					replaced = new ButtonInput(canvas, dom_element.get_attribute("value"));
				}
				else if (HTMLTokenizer::compare(type, "reset"))
				{
					replaced = new ButtonInput(canvas, dom_element.get_attribute("value"));
				}
				else if (HTMLTokenizer::compare(type, "file"))
				{
				}
				else if (HTMLTokenizer::compare(type, "hidden"))
				{
					continue;
				}
				else if (HTMLTokenizer::compare(type, "image"))
				{
					std::string src = dom_element.get_attribute("src");
					replaced = new Img(on_layout_get_image(canvas, HTMLUrl(src, document_url).to_string()));
				}
				else if (HTMLTokenizer::compare(type, "button"))
				{
					replaced = new ButtonInput(canvas, dom_element.get_attribute("value"));
				}

				if (replaced)
				{
					CSSLayoutObject obj = layout.create_object();
					obj.set_component(replaced);
					if (replaced->has_intrinsic_values())
					{
						obj.set_intrinsic_width(replaced->get_intrinsic_width());
						obj.set_intrinsic_height(replaced->get_intrinsic_height());
						if (replaced->has_intrinsic_ratio())
							obj.set_intrinsic_ratio(replaced->get_intrinsic_ratio());
					}
					replaced_objects.push_back(replaced);
					element = obj;
				}
				else
				{
					element = layout.create_element();
				}
			}
			else if (token.name == "button")
			{
				element = layout.create_element();
				// To do: create formatting context for the button?
			}
			else if (token.name == "select")
			{
				int size = dom_element.get_attribute_int("size");
				element = layout.create_element();
				// To do: eat all children
			}
			else if (token.name == "textarea")
			{
				int cols = dom_element.get_attribute_int("cols");
				int rows = dom_element.get_attribute_int("rows");
				element = layout.create_element();
				// To do: eat all children
			}
			else
			{
				element = layout.create_element();
			}
			element.set_col_span(dom_element.get_attribute_int("colspan", 1));
			element.set_row_span(dom_element.get_attribute_int("rowspan", 1));
			element.set_name(string_format("%1.%2#%3", token.name, dom_element.get_attribute("class"), dom_element.get_attribute("id")));

			if (token.name == "body")
			{
				if (layout.get_html_body_element().is_null())
					layout.set_html_body_element(element);
			}
/*
			if (dom_element.get_attribute("id") == "container")
			{
				Sleep(1);
			}

			if (dom_element.get_attribute("class") == "descr")
			{
				Sleep(1);
			}
*/
			DomSelectNode select_node(dom_element);
			element.apply_properties(css_document.select(&select_node));
			if (token.name == "img")
			{
				if (dom_element.has_attribute("width"))
				{
					std::string width_attr = dom_element.get_attribute("width");
					if (width_attr.find("%") == std::string::npos)
						element.apply_properties("width: " + width_attr + "px", page.pageurl.to_string());
					else
						element.apply_properties("width: " + width_attr, page.pageurl.to_string());
				}
			}
			element.apply_properties(dom_element.get_attribute("style"), page.pageurl.to_string());

			if (!css_elements.empty())
			{
				css_elements.back().append_child(element);

				CSSLayoutElement pseudo_before = layout.create_element();
				pseudo_before.set_name(":before");
				pseudo_before.apply_properties(css_document.select(&select_node, "before"));
				css_elements.back().insert_before(pseudo_before, element);

				CSSLayoutElement pseudo_after = layout.create_element();
				pseudo_after.set_name(":after");
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
				CSSLayoutText text = layout.create_text(token.value);
				css_elements.back().append_child(text);
			}
		}
	}

	if (!css_elements.empty())
		layout.set_root_element(css_elements.front());
}
#endif
