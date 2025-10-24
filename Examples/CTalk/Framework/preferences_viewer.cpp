
#include "precomp.h"
#include "preferences_viewer.h"
#include "preferences_page.h"

PreferencesViewer::PreferencesViewer(CL_GUIComponent *owner)
: CL_Window(owner, get_description()), button_ok(0), button_cancel(0), current_page_index(0)
{
	CL_DisplayWindow dispwindow = get_display_window();
	dispwindow.set_large_icon(CL_PNGProvider::load("carambola-32.png"));
	dispwindow.set_small_icon(CL_PNGProvider::load("carambola-16.png"));

	set_class_name("preferences");
	func_render().set(this, &PreferencesViewer::on_render);
	func_resized().set(this, &PreferencesViewer::on_resized);
	func_input_pressed().set(this, &PreferencesViewer::on_input_pressed);
	func_input_released().set(this, &PreferencesViewer::on_input_released);
	func_input_pointer_moved().set(this, &PreferencesViewer::on_input_pointer_moved);
	func_close().set(this, &PreferencesViewer::on_close);

	part_background = CL_GUIThemePart(this);
	part_label_area = CL_GUIThemePart(this, "labelarea");
	part_label = CL_GUIThemePart(this, "label");
	part_page_area = CL_GUIThemePart(this, "pagearea");

	button_ok = new CL_PushButton(this);
	button_cancel = new CL_PushButton(this);
	button_ok->func_clicked().set(this, &PreferencesViewer::on_ok_clicked);
	button_cancel->func_clicked().set(this, &PreferencesViewer::on_cancel_clicked);

	button_ok->set_text("OK");
	button_ok->set_default(true);
	button_ok->set_geometry(CL_Size(75, 22));

	button_cancel->set_text("Cancel");
	button_cancel->set_cancel(true);
	button_cancel->set_geometry(CL_Size(75, 22));

	on_resized();
}

PreferencesViewer::~PreferencesViewer()
{
}

void PreferencesViewer::add_page(PreferencesPage *page)
{
	pages.push_back(page);
	current_page_index = pages.size()-1;
	for (size_t i = 0; i < pages.size(); i++)
		page->set_visible(i == current_page_index, false);
	on_resized();
}

CL_GUITopLevelDescription PreferencesViewer::get_description()
{
	CL_GUITopLevelDescription desc;
	desc.set_title("Preferences");
	desc.set_allow_resize(true);
	desc.set_visible(false);
	return desc;
}

void PreferencesViewer::on_render(CL_GraphicContext &gc, const CL_Rect &clip_rect)
{
	CL_Rect background = get_size();
	CL_Rect all_content = part_background.get_content_box(background);
	CL_Rect button_area(all_content.left, all_content.bottom - 30, all_content.right, all_content.bottom);
	CL_Rect label_area(all_content.left, all_content.top, all_content.left + part_label_area.get_preferred_width(), button_area.top);
	CL_Rect page_area(label_area.right, all_content.top, all_content.right, button_area.top);
	part_background.render_box(gc, background, clip_rect);

	part_label_area.render_box(gc, label_area, clip_rect);
	part_page_area.render_box(gc, page_area, clip_rect);
/*
	render_label_area(gc, label_area, clip_rect);
	render_page_area(gc, page_area, clip_rect);
*/
}

void PreferencesViewer::render_label_area(CL_GraphicContext &gc, const CL_Rect &label_area, const CL_Rect &clip_rect)
{
	part_label_area.render_box(gc, label_area, clip_rect);
	//CL_Rect area_content = part_label_area.get_content_box(label_area);
}

void PreferencesViewer::render_page_area(CL_GraphicContext &gc, const CL_Rect &page_area, const CL_Rect &clip_rect)
{
	part_page_area.render_box(gc, page_area, clip_rect);
	//CL_Rect area_content = part_page_area.get_content_box(page_area);
}

void PreferencesViewer::on_resized()
{
	CL_Rect background = get_size();
	CL_Rect all_content = part_background.get_content_box(background);
	CL_Rect button_area(all_content.left, all_content.bottom - 30, all_content.right, all_content.bottom);
	CL_Rect label_area(all_content.left, all_content.top, all_content.left + part_label_area.get_preferred_width(), button_area.top);
	CL_Rect page_area(label_area.right, all_content.top, all_content.right, button_area.top);

	CL_Rect cancel_rect = CL_Rect(CL_Point(button_area.right-6-button_cancel->get_width(), button_area.top), button_cancel->get_size());
	CL_Rect ok_rect = CL_Rect(CL_Point(cancel_rect.left-6-button_ok->get_width(), button_area.top), button_ok->get_size());

	button_ok->set_geometry(ok_rect);
	button_cancel->set_geometry(cancel_rect);

	CL_Rect page_content(page_area.left+8, page_area.top+75, page_area.right-8, page_area.bottom-8);
	for (size_t i = 0; i < pages.size(); i++)
		pages[i]->set_geometry(page_content);

	request_repaint();
}

bool PreferencesViewer::on_input_pressed(const CL_InputEvent &input_event)
{
	if (input_event.id == CL_MOUSE_LEFT)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool PreferencesViewer::on_input_released(const CL_InputEvent &input_event)
{
	if (input_event.id == CL_MOUSE_LEFT)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool PreferencesViewer::on_input_pointer_moved(const CL_InputEvent &input_event)
{
	return true;
}

void PreferencesViewer::on_ok_clicked()
{
	exit_with_code(1);
}

void PreferencesViewer::on_cancel_clicked()
{
	exit_with_code(0);
}

bool PreferencesViewer::on_close()
{
	exit_with_code(0);
	return true;
}
