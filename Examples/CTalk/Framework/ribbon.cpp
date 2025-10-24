
#include "precomp.h"
#include "ribbon.h"

Ribbon::Ribbon(CL_GUIComponent *container)
: CL_GUIComponent(container), hot(false), current_page_index(0)
{
	set_type_name("ribbon");
	func_render().set(this, &Ribbon::on_render);
	func_resized().set(this, &Ribbon::on_resized);

	part_background = CL_GUIThemePart(this);
	part_section = CL_GUIThemePart(this, "ribbonsection");
	part_tab = CL_GUIThemePart(this, "ribbontab");
	font_tab = CL_Font(get_gc(), "Tahoma", -11);//part_background.get_font();//.reset(new Font(/*L"Segoe UI"*/L"Tahoma", 11, CL_Colorf((unsigned char)0xdd,(unsigned char)0xdd,(unsigned char)0xdd), false, true));
}

Ribbon::~Ribbon()
{
}

CL_Size Ribbon::get_preferred_size() const
{
	return part_background.get_preferred_size();
}

void Ribbon::add_page(const RibbonPage &page)
{
	pages.push_back(page);
}

void Ribbon::on_render(CL_GraphicContext &gc, const CL_Rect &clip_rect)
{
	CL_Size client_size = get_size();
	part_background.render_box(gc, client_size, clip_rect);
	paint_tabs(gc, clip_rect);
	paint_sections(gc, clip_rect);
}

void Ribbon::paint_tabs(CL_GraphicContext &gc, const CL_Rect &clip_rect)
{
	int tab_x = 15;
	for (std::vector<RibbonPage>::size_type page_index = 0; page_index < pages.size(); page_index++)
	{
		CL_Size size_tab_text = font_tab.get_text_size(gc, pages[page_index].text);
		int tab_width = cl_max(size_tab_text.width, 40)+12;
		CL_Rect current_tab(CL_Point(tab_x, 0), CL_Size(tab_width+12, part_tab.get_preferred_height()));

		if (page_index == current_page_index)
			part_tab.render_box(gc, current_tab, clip_rect);

		font_tab.draw_text(gc, current_tab.left+current_tab.get_width()/2-size_tab_text.width/2, current_tab.bottom-6, pages[page_index].text, CL_Colorf::black);

		pages[page_index].position = current_tab;
		tab_x = current_tab.right+2;
	}
}

void Ribbon::paint_sections(CL_GraphicContext &gc, const CL_Rect &clip_rect)
{
	if (current_page_index >= 0 && current_page_index < pages.size())
	{
		CL_Size client_size = get_size();
		RibbonPage &page = pages[current_page_index];
		int section_x = 10;
		for (std::vector<RibbonSection>::size_type section_index = 0; section_index < page.sections.size(); section_index++)
		{
			RibbonSection &section = page.sections[section_index];
			CL_Rect section_rect(section_x, part_tab.get_preferred_height()+2, section_x+section.size, client_size.height-6);
			part_section.render_box(gc, section_rect, clip_rect);

			CL_Size size_section_text = font_tab.get_text_size(gc, section.text);
			font_tab.draw_text(gc, section_rect.left+section_rect.get_width()/2-size_section_text.width/2, section_rect.bottom-5, section.text, CL_Colorf::black);

			section.position = section_rect;
			paint_section_buttons(gc, section, section_rect, clip_rect);

			section_x = section_rect.right + 5;
		}
	}
}

void Ribbon::paint_section_buttons(CL_GraphicContext &gc, RibbonSection &section, const CL_Rect &section_rect, const CL_Rect &clip_rect)
{
	int x = section_rect.left+6;
	for (std::vector<RibbonButton>::size_type index = 0; index < section.buttons.size(); index++)
	{
		RibbonButton &button = section.buttons[index];
		
		CL_Size size_button_text = font_tab.get_text_size(gc, button.text);
		int width = cl_max(size_button_text.width, button.icon.get_width())+20;

		CL_Rect button_area;
		button_area.left = x+2;
		button_area.right = x+width-2;
		button_area.top = section_rect.bottom-39-button.icon.get_height();
		button_area.bottom = section_rect.bottom-25;
		button.position = button_area;

		button.icon.draw(gc, CL_Rect(CL_Point(x+width/2-button.icon.get_width()/2, button_area.top), button.icon.get_size()));
		font_tab.draw_text(gc, x+width/2-size_button_text.width/2, button_area.bottom, button.text, CL_Colorf::black);
		x += width;
	}
}

/* Why is this here?
bool Ribbon::on_mouse_move(WPARAM wparam, int mouse_x, int mouse_y)
{
	if (!hot)
	{
		track_mouse_leave();
		hot = true;
	}
	CL_Point mouse_pos(mouse_x, mouse_y);
	bool found = false;
	for (std::vector<RibbonPage>::size_type page_index = 0; page_index < pages.size(); page_index++)
	{
		if (pages[page_index].position.contains(mouse_pos))
		{
			found = true;
			SetCursor(LoadCursor(0, IDC_HAND));
			break;
		}
	}
	if (current_page_index >= 0 && current_page_index < pages.size())
	{
		RibbonPage &page = pages[current_page_index];
		for (std::vector<RibbonSection>::size_type section_index = 0; section_index < page.sections.size(); section_index++)
		{
			RibbonSection &section = page.sections[section_index];
			if (section.position.contains(mouse_pos))
			{
				for (std::vector<RibbonButton>::size_type button_index = 0; button_index < section.buttons.size(); button_index++)
				{
					if (section.buttons[button_index].position.contains(mouse_pos))
					{
						SetCursor(LoadCursor(0, IDC_HAND));
						found = true;
						break;
					}
				}
				break;
			}
		}
	}
	if (!found)
		SetCursor(LoadCursor(0, IDC_ARROW));
	return true;
}

bool Ribbon::on_lbutton_down(WPARAM wparam, int mouse_x, int mouse_y)
{
	CL_Point mouse_pos(mouse_x, mouse_y);
	for (std::vector<RibbonPage>::size_type page_index = 0; page_index < pages.size(); page_index++)
	{
		if (pages[page_index].position.contains(mouse_pos))
		{
			current_page_index = page_index;
			request_repaint();
			break;
		}
	}

	if (current_page_index >= 0 && current_page_index < pages.size())
	{
		RibbonPage &page = pages[current_page_index];
		for (std::vector<RibbonSection>::size_type section_index = 0; section_index < page.sections.size(); section_index++)
		{
			RibbonSection &section = page.sections[section_index];
			if (section.position.contains(mouse_pos))
			{
				for (std::vector<RibbonButton>::size_type button_index = 0; button_index < section.buttons.size(); button_index++)
				{
					if (section.buttons[button_index].position.contains(mouse_pos))
					{
						if (!section.buttons[button_index].cb_clicked.is_null())
							section.buttons[button_index].cb_clicked.invoke();
						break;
					}
				}
				break;
			}
		}
	}

	return true;
}
*/

bool Ribbon::on_mouse_leave()
{
	hot = false;
	return true;
}

