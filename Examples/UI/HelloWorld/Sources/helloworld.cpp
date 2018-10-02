/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
**    Mark Page
**    Artem Khomenko
*/

#include "precomp.h"
#include "helloworld.h"
#include "../../../ThemeAero/Sources/theme.h"

using namespace clan;

// First of all, initialize the library.
clan::ApplicationInstance<HelloWorld> clanapp;



MainWindow::MainWindow(std::shared_ptr<clan::WindowManager>& window_manager)
{
	set_title("UICore: Hello World");
	set_content_size(Sizef(640.0f, 600.0f), true);

	const std::shared_ptr<clan::View> pRootView = root_view();

	// Main window icons
	std::vector<std::string> iconNames{ "../../ThemeAero/Resources/app_icon_16x16.png", "../../ThemeAero/Resources/app_icon_32x32.png" };
	set_icon(iconNames);

	// Exit run loop when close is clicked or ESC pressed.
	pRootView->slots.connect(pRootView->sig_close(), [&](CloseEvent &e) { RunLoop::exit(); });
	pRootView->slots.connect(pRootView->sig_key_press(), [&](clan::KeyEvent &e)
	{ if (e.key() == clan::Key::escape) RunLoop::exit(); }
	);

	// Style the root view to use rounded corners and a bit of drop shadow
	pRootView->style()->set("padding: 11px; background: #efefef; flex-direction: column");

	// Create a panel inside the root view according to the design desire.
	// Style values can be listed through a semicolon or multiple calls set().
	auto body = std::make_shared<View>();
	body->style()->set("background: white");
	body->style()->set("padding: 11px");
	body->style()->set("border-top: 5px solid #DD3B2A");
	body->style()->set("border-bottom: 5px solid #DD3B2A");
	body->style()->set("flex-direction: column");
	body->style()->set("flex: auto");
	pRootView->add_child(body);

	// Create a label with some text to have some content
	label = std::make_shared<LabelView>();
	label->style()->set("flex: none; font: 20px/40px 'Ravie'; color: #DD3B2A");
	label->set_text("Click here");
	body->add_child(label);

	// React to clicking
	label->slots.connect(label->sig_pointer_press(), [&](PointerEvent &e) {
		label->set_text(label->text() + " CLICK!");
	});

	// Create a scrollable area with automatic vertical scrollbar.
	auto scrollarea = std::make_shared<ScrollView>();
	auto scroll_view = scrollarea->content_view();
	scrollarea->style()->set("margin: 5px 0; border: 1px solid black; padding: 5px 0px 5px 5px;");
	scroll_view->style()->set("flex-direction: column; background: white;");
	Theme::initialize_scrollbar(scrollarea->scrollbar_y_view(), false);
	scrollarea->scrollbar_y_view()->style()->set("padding: 0 0 0 3px; background: white;");
	body->add_child(scrollarea);

	// Create a text field for our span layout
	auto edit = std::make_shared<TextFieldView>();
	edit->style()->set("font: 11px/20px 'Segoe UI'; margin: 5px; background: #efefef; border: 1px solid black");
	edit->style()->set("border-radius: 3px; padding: 2px 5px 2px 5px; width: 128px; box-shadow: 0 0 5px rgba(100,100,200,0.2)");
	edit->set_text("amazing!");

	// Create a span layout views with some more complex inline formatting
	auto p1 = std::make_shared<SpanLayoutView>();
	p1->style()->set("font: 13px/25px 'Segoe UI'");
	p1->text_style("bold")->set("font-weight: bold");
	p1->text_style("italic")->set("font-style: italic");
	p1->add_text("This is an example of why Sphair should never ever make fun of my ");
	p1->add_text("BEAUTIFUL", "bold");
	p1->add_text(" green 13.37deg gradients because he will never know what it is replaced with!");
	scroll_view->add_child(p1);

	auto p2 = std::make_shared<SpanLayoutView>();
	p2->style()->set("margin: 15px 0 5px 0; padding: 7px; border-top: 5px solid #CCE4FB; border-bottom: 5px solid #CCE4FB; background: #EDF6FF; font: 13px/25px 'Segoe UI'");
	p2->text_style("bold")->set("font-weight: bold");
	p2->text_style("italic")->set("font-style: italic");
	p2->add_text("If you also think Sphair made a ");
	p2->add_text("BIG MISTAKE", "bold");
	p2->add_text(" please consider typing ");
	p2->add_text("Yes, yes, yes, yes, yes, yes, yes yes, YES!", "italic");
	p2->add_text(" in the text field: ");
	p2->add_child(edit);
	p2->add_text(" You know you want to!", "bold");
	scroll_view->add_child(p2);

	auto p3 = std::make_shared<SpanLayoutView>();
	p3->style()->set("font: 13px/25px 'Segoe UI'");
	p3->text_style("bold")->set("font-weight: bold");
	p3->text_style("italic")->set("font-style: italic");
	p3->add_text("Since we both know you typed ");
	p3->add_text("Yes, yes, yes..", "italic");
	p3->add_text(" into the text field (who wouldn't!?), here's the amazing gradient:");
	scroll_view->add_child(p3);

	auto gradient_box = std::make_shared<View>();
	gradient_box->style()->set("margin: 15px auto; width: 120px; height: 75px; border: 1px solid #777");
	gradient_box->style()->set("background: linear-gradient(13.37deg, #f0f0f0, rgb(120,240,120) 50%, #f0f0f0)");
	gradient_box->style()->set("box-shadow: 7px 7px 7px rgba(0,0,0,0.2)");
	scroll_view->add_child(gradient_box);

	auto listbox = Theme::create_listbox();
	listbox->style()->set("flex: none; height: 60px; margin: 7px 0; border: 1px solid black; padding: 5px; background: #f0f0f0");
	listbox->set_items<std::string>(
	{ "Item 1", "Item 2", "Item 3", "Item 4", "Item 5", "More items", "Even more items!!", "No more items!!!!!" },
		[](const std::string &s) -> std::shared_ptr<View>
	{
		auto item = Theme::create_listbox_label(s);
		return item;
	});
	scroll_view->add_child(listbox);

	// A separately located scroll bar.
	auto scrollbar = Theme::create_scrollbar();
	scrollbar->set_range(0.0, 1.0);
	scrollbar->set_position(0.5);
	scrollbar->set_page_step(0.1);
	scrollbar->set_line_step(0.01);
	scroll_view->add_child(scrollbar);

	// A button.
	auto button = Theme::create_button();
	button->label()->set_text("This is a button");
	scroll_view->add_child(button);

	// A separately located slider.
	auto slider = Theme::create_slider();
	//slider->set_disabled();
	slider->set_min_position(0);
	slider->set_max_position(1000);
	slider->set_tick_count(100);
	slider->set_lock_to_ticks(false);
	slider->set_page_step(100);
	slider->set_position(slider->max_position() / 2);
	scroll_view->add_child(slider);

	// A separately located check box.
	auto checkbox = Theme::create_checkbox();
	//checkbox->set_disabled();
	checkbox->style()->set("margin: 12px");
	checkbox->label()->set_text("Checkbox");
	scroll_view->add_child(checkbox);

	// Three radio buttons. They know each other.
	// Hint - parent of the radiobutton must have an opaque background due to sub-pixel rendering effect.
	for (int cnt = 0; cnt != 3; ++cnt)
	{
		auto radio = Theme::create_radiobutton();
		//radio->set_disabled(true);
		scroll_view->add_child(radio);
	}

	// Create a popup window as hint on the label.
	label->slots.connect(label->sig_pointer_enter(), [=](PointerEvent &e)
	{
		auto popup = std::make_shared<WindowController>();
		popup->root_view()->style()->set("flex-direction: column; background: #FFFFE0; margin: 5px; border: 1px solid black; border-radius: 2px");
		popup->root_view()->style()->set("padding: 2px 5px 2px 5px; box-shadow: 0 0 3px rgba(0,0,0,0.2)");

		auto text = Theme::create_label(true);
		text->style()->set("font: 12px Tahoma; color: black");
		text->set_text("This is an awesome popup");
		popup->root_view()->add_child(text);

		std::weak_ptr<WindowController> popup_weak = popup;
		popup->slots.connect(label->sig_pointer_leave(), [=](PointerEvent &e)
		{
			auto p = popup_weak.lock();
			if (p)
				p->dismiss();
		});

		window_manager->present_popup(label.get(), e.pos(label) + Pointf(10.0f, 10.0f), popup);
	});

	// Show a modal dialog
	button->func_clicked() = [=]()
	{
		auto dialog = std::make_shared<WindowController>();
		dialog->set_title("Alarm!!");
		dialog->root_view()->style()->set("flex-direction: column; background: rgb(240,240,240); padding: 11px; width: 250px");

		auto text = Theme::create_label(true);
		text->style()->set("margin-bottom: 7px; font: 12px Tahoma; color: black");
		text->set_text("This a modal dialog");
		dialog->root_view()->add_child(text);

		auto ok_button = Theme::create_button();
		ok_button->label()->set_text("OK");
		dialog->root_view()->add_child(ok_button);

		std::weak_ptr<WindowController> dialog_weak = dialog;
		ok_button->func_clicked() = [=]()
		{
			auto d = dialog_weak.lock();
			if (d)
				d->dismiss();
		};

		window_manager->present_modal(pRootView.get(), dialog);
	};
}


/////////////////////////////////////////////////////////////////////////////
// Application class.
/////////////////////////////////////////////////////////////////////////////
HelloWorld::HelloWorld() 
	: window_manager(std::make_shared<clan::WindowManager>())
{
#if defined(WIN32) && !defined(__MINGW32__)
	clan::D3DTarget::set_current();
#else
	clan::OpenGLTarget::set_current();
#endif

	// Create a source for our resources
	FileResourceDocument doc(FileSystem("../../ThemeAero"));
	ResourceManager resources = FileResourceManager::create(doc);

	// Mark this thread as the UI thread
	ui_thread = UIThread(resources);

	// Create and show the main window.
	auto main_window = std::make_shared<MainWindow>(window_manager);
	window_manager->present_main(main_window);

	// It is necessary to receive keyboard events. And need to call after present_main().
	main_window->root_view()->set_focus();
}

bool HelloWorld::update()
{
	window_manager->flip();

	return true;
}