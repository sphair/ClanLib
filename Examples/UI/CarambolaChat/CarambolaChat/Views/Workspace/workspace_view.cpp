
#include "precomp.h"
#include "workspace_view.h"
#include "workspace_tab_view.h"

using namespace clan;

WorkspaceView::WorkspaceView()
{
	style()->set("flex: 1 1 main-size");
	style()->set("flex-direction: column");

	labels_group = std::make_shared<View>();
	labels_group->style()->set("flex: 0 0 main-size");
	labels_group->style()->set("margin: 0 -0.5px");
	labels_group->style()->set("padding: 0");
	labels_group->style()->set("height: 30px");
	labels_group->style()->set("flex-direction: row");

	toolbar = std::make_shared<View>();
	toolbar->style()->set("flex: 0 0 main-size");
	toolbar->style()->set("height: 40px");
	toolbar->style()->set("margin: 0");
	toolbar->style()->set("background: linear-gradient(to bottom, rgb(235, 243, 252), rgb(219, 234, 249))");
	toolbar->style()->set("border-bottom: 1px solid rgb(153,153,153)");

	pages_group = std::make_shared<View>();
	pages_group->style()->set("flex: 1 1 main-size");
	pages_group->style()->set("margin: 0");
	pages_group->style()->set("background: white");
	pages_group->style()->set("flex-direction: column");

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

	slots.connect(page.tab->sig_pointer_press(), on_click);

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
			page.tab->style()->set("background: linear-gradient(to bottom, %1, %2)", Style::to_rgba(mix(background, foreground1, t)), Style::to_rgba(mix(background, foreground2, t)));
			page.tab->style()->set("border: 0.5px solid %1; border-bottom: none", Style::to_rgba(mix(background_border, foreground_border, t)));
		});
	}
	else
	{
		if (selected)
		{
			page.tab->style()->set("background: linear-gradient(to bottom, %1, %2)", Style::to_rgba(foreground1), Style::to_rgba(foreground2));
			page.tab->style()->set("border: 0.5px solid %1; border-bottom: none", Style::to_rgba(foreground_border));
		}
		else
		{
			page.tab->style()->set("background: %1", Style::to_rgba(background));
			page.tab->style()->set("border: 0.5px solid %1; border-bottom: none", Style::to_rgba(background_border));
		}
	}
	
	page.page->set_hidden(!selected);
}
