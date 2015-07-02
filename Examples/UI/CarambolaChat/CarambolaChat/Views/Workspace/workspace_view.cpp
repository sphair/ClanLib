
#include "precomp.h"
#include "workspace_view.h"
#include "workspace_tab_view.h"

using namespace clan;

WorkspaceView::WorkspaceView()
{
	style()->set("flex: auto");
	style()->set("flex-direction: column");

	labels_group = std::make_shared<View>();
	labels_group->style()->set("flex: none");
	labels_group->style()->set("flex-direction: row");
	labels_group->style()->set("border-top: 1px solid rgb(159, 174, 194)");
	labels_group->style()->set("background: linear-gradient(to right, rgb(234,240,249), rgb(223,233,245))");

	toolbar = std::make_shared<View>();
	toolbar->style()->set("flex: none");
	toolbar->style()->set("height: 40px");
	toolbar->style()->set("margin: 0");
	toolbar->style()->set("background: linear-gradient(to bottom, rgb(223, 239, 254), rgb(219, 234, 249))");

	pages_group = std::make_shared<View>();
	pages_group->style()->set("flex: auto");
	pages_group->style()->set("margin: 0");
	pages_group->style()->set("border-top: 3px solid white");
	pages_group->style()->set("background: white");
	pages_group->style()->set("flex-direction: column");

	//add_subview(toolbar);
	add_subview(labels_group);
	add_subview(pages_group);
}

void WorkspaceView::add_page(const std::string &id, const std::string &label_text, std::shared_ptr<View> page_view, bool app_page)
{
	bool first_tab = tabs.empty();

	TabPage &page = tabs[id];

	page.tab = std::make_shared<WorkspaceTabView>(label_text, app_page);
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
	tabs[id].tab->message_count->set_state("highlight", !text.empty());
}

bool WorkspaceView::is_message_count_hidden(const std::string &id) const
{
	return !tabs.find(id)->second.tab->message_count->state("highlight");
}

void WorkspaceView::set_selected(TabPage &page, bool selected, bool animated)
{
	if (selected)
	{
		page.tab->message_count->set_text("");
		page.tab->message_count->set_state("highlight", false);
	}

	page.tab->set_state("selected", selected);

	/*
	Colorf background(243, 240, 244, 160);
	Colorf foreground1(239, 247, 255);
	Colorf foreground2(235, 243, 252);
	//Colorf foreground_border = Colorf::gray40;
	//Colorf background_border = Colorf::transparent;

	if (animated)
	{
		if (selected == !page.page->hidden()) return;

		float from = selected ? 0.0f : 1.0f;
		float to = selected ? 1.0f : 0.0f;

		page.tab->animate(from, to, [=](float t)
		{
			page.tab->style()->set("background: linear-gradient(to bottom, %1, %2)", Style::to_rgba(mix(background, foreground1, t)), Style::to_rgba(mix(background, foreground2, t)));
			//page.tab->style()->set("border: 1px solid %1; border-bottom: none", Style::to_rgba(mix(background_border, foreground_border, t)));
		});
	}
	else
	{
		if (selected)
		{
			page.tab->style()->set("background: linear-gradient(to bottom, %1, %2)", Style::to_rgba(foreground1), Style::to_rgba(foreground2));
			//page.tab->style()->set("border: 1px solid %1; border-bottom: none", Style::to_rgba(foreground_border));
		}
		else
		{
			page.tab->style()->set("background: %1", Style::to_rgba(background));
			//page.tab->style()->set("border: 1px solid %1; border-bottom: none", Style::to_rgba(background_border));
		}
	}
	*/
	page.page->set_hidden(!selected);
}
