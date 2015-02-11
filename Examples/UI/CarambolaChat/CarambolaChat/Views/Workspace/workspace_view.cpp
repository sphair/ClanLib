
#include "precomp.h"
#include "workspace_view.h"
#include "workspace_tab_view.h"

using namespace clan;

WorkspaceView::WorkspaceView()
{
	box_style.set_layout_vbox();

	labels_group = std::make_shared<View>();
	labels_group->box_style.set_flex(0.0f, 0.0f);
	labels_group->box_style.set_margin(-0.5f, 0.0f);
	labels_group->box_style.set_padding(0.0f, 0.0f, 0.0f, 0.0f);
	labels_group->box_style.set_height(30.0f);
	labels_group->box_style.set_layout_hbox();

	toolbar = std::make_shared<View>();
	toolbar->box_style.set_flex(0.0f, 0.0f);
	toolbar->box_style.set_height(40.0f);
	toolbar->box_style.set_margin(0.0f, 0.0f);
	toolbar->box_style.set_background_gradient_to_bottom(Colorf(235, 243, 252), Colorf(219, 234, 249));
	toolbar->box_style.set_border(Colorf::gray60, 0.0f, 0.0f, 0.0f, 1.0f);

	pages_group = std::make_shared<View>();
	pages_group->box_style.set_flex(1.0f, 1.0f);
	pages_group->box_style.set_margin(0, 0, 0, 0);
	pages_group->box_style.set_background(Colorf::white);
	pages_group->box_style.set_layout_vbox();

	add_subview(labels_group);
	add_subview(toolbar);
	add_subview(pages_group);
}

void WorkspaceView::add_page(const std::string &id, const std::string &label_text, std::shared_ptr<View> page_view)
{
	bool first_tab = tabs.empty();

	TabPage &page = tabs[id];

	page.tab = std::make_shared<WorkspaceTabView>(label_text);
	page.page = page_view;

	auto on_click = [=](PointerEvent &e)
	{
		for (auto &it : tabs)
		{
			set_selected(it.second, it.second.page == page_view, true);
		}
	};

	slots.connect(page.tab->sig_pointer_press(EventUIPhase::at_target), on_click);
	slots.connect(page.tab->sig_pointer_press(EventUIPhase::bubbling), on_click);

	labels_group->add_subview(page.tab);

	pages_group->add_subview(page_view);
	page_view->set_hidden(true);

	set_selected(page, first_tab);
}

void WorkspaceView::remove_page(std::shared_ptr<View> page_view)
{
	page_view->remove_from_super();

	for (auto it = tabs.begin(); it != tabs.end(); ++it)
	{
		if (it->second.page == page_view)
		{
			it->second.tab->remove_from_super();
			tabs.erase(it);
			break;
		}
	}
}

void WorkspaceView::set_label(const std::string &id, const std::string &text)
{
	tabs[id].tab->label->set_text(text);
}

void WorkspaceView::set_message_count(const std::string &id, const std::string &text)
{
	tabs[id].tab->message_count->set_text(text);
	tabs[id].tab->message_count->set_hidden(text.empty());
}

bool WorkspaceView::is_message_count_hidden(const std::string &id) const
{
	return tabs.find(id)->second.tab->message_count->hidden();
}

void WorkspaceView::set_selected(TabPage &page, bool selected, bool animated)
{
	if (selected)
	{
		page.tab->message_count->set_text("");
		page.tab->message_count->set_hidden();
	}

	Colorf background(243, 240, 244, 160);
	Colorf foreground1(239, 247, 255);
	Colorf foreground2(235, 243, 252);
	Colorf foreground_border = Colorf::gray40;
	Colorf background_border = Colorf::transparent;

	if (animated)
	{
		if (selected == !page.page->hidden()) return;

		float from = selected ? 0.0f : 1.0f;
		float to = selected ? 1.0f : 0.0f;

		page.tab->animate(from, to, [=](float t)
		{
			page.tab->box_style.set_background_gradient_to_bottom(mix(background, foreground1, t), mix(background, foreground2, t));
			page.tab->box_style.set_border(mix(background_border, foreground_border, t), 0.5f, 0.5f, 0.5f, 0.0f);
		});
	}
	else
	{
		if (selected)
		{
			page.tab->box_style.set_background_gradient_to_bottom(foreground1, foreground2);
			page.tab->box_style.set_border(foreground_border, 0.5f, 0.5f, 0.5f, 0.0f);
		}
		else
		{
			page.tab->box_style.set_background_gradient_to_bottom(background, background);
			page.tab->box_style.set_border(background_border, 0.5f, 0.5f, 0.5f, 0.0f);
		}
	}

	page.page->set_hidden(!selected);
}
