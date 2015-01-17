
#include "precomp.h"
#include "workspace_view_controller.h"

using namespace clan;

WorkspaceViewController::WorkspaceViewController()
{
	view->box_style.set_layout_vbox();

	labels_group = std::make_shared<View>();
	labels_group->box_style.set_flex(0.0f, 0.0f);
	labels_group->box_style.set_padding(7.5f, 12.0f, 8.0f, 0.0f);
	labels_group->box_style.set_height(30.0f);
	labels_group->box_style.set_layout_hbox();

	toolbar = std::make_shared<View>();
	toolbar->box_style.set_flex(0.0f, 0.0f);
	toolbar->box_style.set_height(40.0f);
	toolbar->box_style.set_margin(8.0f, 0.0f);
	toolbar->box_style.set_background_gradient_to_bottom(Colorf(235, 243, 252), Colorf(219, 234, 249));
	toolbar->box_style.set_border(Colorf::gray60, 0.0f, 0.0f, 0.0f, 1.0f);

	pages_group = std::make_shared<View>();
	pages_group->box_style.set_flex(1.0f, 1.0f);
	pages_group->box_style.set_margin(8, 0, 8, 8);
	pages_group->box_style.set_background(Colorf::white);
	pages_group->box_style.set_layout_vbox();

	view->add_subview(labels_group);
	view->add_subview(toolbar);
	view->add_subview(pages_group);
}

void WorkspaceViewController::add_page(const std::string &label_text, std::shared_ptr<ViewController> controller)
{
	TabPage page;

	page.tab = std::make_shared<View>();
	page.tab->box_style.set_flex(0.0f, 1.0f);
	page.tab->box_style.set_border_radius(2.0f, 2.0f, 0.0f, 0.0f);
	page.tab->box_style.set_padding(10.0f, 2.0f, 7.0f, 2.0f);
	page.tab->box_style.set_layout_line();

	page.label = std::make_shared<LabelView>();
	page.label->text_style().set_font("Segoe UI", 12.0f, 26.0f);
	page.label->text_style().set_target_transparent();
	page.label->set_text(label_text);
	page.tab->add_subview(page.label);

	page.close = std::make_shared<ImageView>();
	page.close->set_image(ImageSource::from_resource("Icons/cross-small.png"));
	page.close->box_style.set_margin(8.0f, 9.0f, 0.0f, 9.0f);
	page.tab->add_subview(page.close);

	page.page = controller;

	auto on_click = [=](PointerEvent &e)
	{
		for (auto &tab : tabs)
		{
			set_selected(tab, tab.page == controller, true);
		}
	};

	slots.connect(page.tab->sig_pointer_press(EventUIPhase::at_target), on_click);
	slots.connect(page.tab->sig_pointer_press(EventUIPhase::bubbling), on_click);

	labels_group->add_subview(page.tab);
	tabs.push_back(page);
	add_child_controller(controller);

	set_selected(tabs.back(), tabs.size() == 1);
}

void WorkspaceViewController::set_selected(TabPage &page, bool selected, bool animated)
{
	if (animated)
	{
		if (selected == !page.page->view->hidden()) return;

		float from = selected ? 0.0f : 1.0f;
		float to = selected ? 1.0f : 0.0f;
		auto tab = page.tab;

		page.tab->animate(from, to, [=](float t)
		{
			if (t > 0.0f)
			{
				tab->box_style.set_background_gradient_to_bottom(Colorf(255, 255, 255, (int)((0.2f + t * 0.8f) * 255.0f)), Colorf(235, 243, 252, (int)((0.2f + t * 0.8f) * 255.0f)));
				tab->box_style.set_border(Colorf(102, 102, 102, (int)(t * 255.0f)), 0.5f, 0.5f, 0.5f, 0.0f);
			}
			else
			{
				tab->box_style.set_background(Colorf(1.0f, 1.0f, 1.0f, 0.2f));
				tab->box_style.set_border(Colorf::transparent, 0.5f, 0.5f, 0.5f, 0.0f);
			}
		});
	}
	else
	{
		if (selected)
		{
			page.tab->box_style.set_background_gradient_to_bottom(Colorf::white, Colorf(235, 243, 252));
			page.tab->box_style.set_border(Colorf::gray40, 0.5f, 0.5f, 0.5f, 0.0f);
		}
		else
		{
			page.tab->box_style.set_background(Colorf(1.0f, 1.0f, 1.0f, 0.2f));
			page.tab->box_style.set_border(Colorf::transparent, 0.5f, 0.5f, 0.5f, 0.0f);
		}
	}

	page.page->view->set_hidden(!selected);
}

void WorkspaceViewController::child_controller_added(const std::shared_ptr<ViewController> &view_controller)
{
	pages_group->add_subview(view_controller->view);
	view_controller->view->set_hidden(true);
}

void WorkspaceViewController::child_controller_removed(const std::shared_ptr<ViewController> &view_controller)
{
	view_controller->view->remove_from_super();
	for (size_t i = 0; i < tabs.size(); i++)
	{
		if (tabs[i].page == view_controller)
		{
			tabs[i].tab->remove_from_super();
			tabs.erase(tabs.begin() + i);
			break;
		}
	}
}
