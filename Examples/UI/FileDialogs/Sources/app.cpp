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
#include "app.h"
#include "../../../ThemeAero/Sources/theme.h"
#include <ClanLib/UI/SystemDialogs/folder_browse_dialog.h>
#include <ClanLib/UI/SystemDialogs/open_file_dialog.h>
#include <ClanLib/UI/SystemDialogs/save_file_dialog.h>

using namespace clan;

clan::ApplicationInstance<App> clanapp;

App::App()
{
#if defined(WIN32) && !defined(__MINGW32__)
	clan::D3DTarget::set_current();
#else
	clan::OpenGLTarget::set_current();
#endif

	// Create a window:
	DisplayWindowDescription desc;
	desc.set_title("UICore: Hello World");
	desc.set_allow_resize(true);
	window = std::make_shared<TopLevelWindow>(desc);
	auto pRootView = window->root_view();
	pRootView->slots.connect(window->root_view()->sig_close(), [&](CloseEvent &e) { RunLoop::exit(); });
	pRootView->slots.connect(pRootView->sig_key_press(), [&](clan::KeyEvent &e)
	{ if (e.key() == clan::Key::escape) RunLoop::exit(); }
	);

	// Need for receive a keyboard events.
	pRootView->set_focus();

	// Create a source for our resources
	FileResourceDocument doc(FileSystem("../../ThemeAero"));
	ResourceManager resources = FileResourceManager::create(doc);

	// Mark this thread as the UI thread
	ui_thread = UIThread(resources);

	// Style the root view to use rounded corners and a bit of drop shadow
	pRootView->style()->set("padding: 11px");
	pRootView->style()->set("background: #efefef");
	pRootView->style()->set("flex-direction: column");

	// First (top) panel with button and text
	//
	auto panel1 = std::make_shared<View>();
	panel1->style()->set("background: white");
	panel1->style()->set("padding: 11px");
	panel1->style()->set("flex-direction: row");
	panel1->style()->set("flex: auto");
	pRootView->add_child(panel1);

	auto button1 = Theme::create_button();
	button1->style()->set("height: 40px");
	button1->style()->set("width: 120px");
	button1->label()->set_text("Folder browse");
	button1->style()->set("flex: none");
	button1->image_view()->set_image(clan::Image(pRootView->canvas(), "./document_open.png"));
	button1->func_clicked() = clan::bind_member(this, &App::on_button1_down);
	panel1->add_child(button1);

	label1 = std::make_shared<LabelView>();
	label1->style()->set("font: 20px/40px 'Ravie'");
	label1->style()->set("padding: 0px 10px");
	label1->set_text("Press the button for select a folder");
	panel1->add_child(label1);

	// Second panel with button and text
	//
	auto panel2 = std::make_shared<View>();
	panel2->style()->set("background: white");
	panel2->style()->set("padding: 11px");
	panel2->style()->set("flex-direction: row");
	panel2->style()->set("flex: auto");
	pRootView->add_child(panel2);

	auto button2 = Theme::create_button();
	button2->style()->set("height: 40px");
	button2->style()->set("width: 120px");
	button2->label()->set_text("Open file");
	button2->style()->set("flex: none");
	button2->func_clicked() = clan::bind_member(this, &App::on_button2_down);
	panel2->add_child(button2);

	label2 = std::make_shared<LabelView>();
	label2->style()->set("font: 20px/40px 'Ravie'");
	label2->style()->set("padding: 0px 10px");
	label2->set_text("Press the button for select only existing file");
	panel2->add_child(label2);

	// Third panel with button and text
	//
	auto panel3 = std::make_shared<View>();
	panel3->style()->set("background: white");
	panel3->style()->set("padding: 11px");
	panel3->style()->set("flex-direction: row");
	panel3->style()->set("flex: auto");
	pRootView->add_child(panel3);

	auto button3 = Theme::create_button();
	button3->style()->set("height: 40px");
	button3->style()->set("width: 120px");
	button3->label()->set_text("Save file");
	button3->style()->set("flex: none");
	button3->func_clicked() = clan::bind_member(this, &App::on_button3_down);
	panel3->add_child(button3);

	label3 = std::make_shared<LabelView>();
	label3->style()->set("font: 20px/40px 'Ravie'");
	label3->style()->set("padding: 0px 10px");
	label3->set_text("Press the button for select existing or new file");
	panel3->add_child(label3);

	// Fourth panel with button and text
	//
	auto panel4 = std::make_shared<View>();
	panel4->style()->set("background: white");
	panel4->style()->set("padding: 11px");
	panel4->style()->set("flex-direction: row");
	panel4->style()->set("flex: auto");
	pRootView->add_child(panel4);

	button4 = Theme::create_button();
	button4->style()->set("height: 40px");
	button4->style()->set("width: 120px");
	button4->label()->set_text("Sticky button");
	button4->style()->set("flex: none");
	button4->func_clicked() = clan::bind_member(this, &App::on_button4_down);
	button4->set_sticky(true);
	button4->set_pressed(true);
	panel4->add_child(button4);

	label4 = std::make_shared<LabelView>();
	label4->style()->set("font: 20px/40px 'Ravie'");
	label4->style()->set("padding: 0px 10px");
	panel4->add_child(label4);
	on_button4_down();	// Manual setting button's "pressed" property doesn't call user event handler automatically.

}

void App::on_button1_down()
{
	auto dlg = std::make_shared<BrowseFolderDialog>(window->root_view().get());
	label1->set_text(dlg->show() ? dlg->selected_path() : "Canceled"); 
}

void App::on_button2_down()
{
	auto dlg = std::make_shared<OpenFileDialog>(window->root_view().get());
	label2->set_text(dlg->show() ? dlg->filename() : "Canceled");
}

void App::on_button3_down()
{
	auto dlg = std::make_shared<SaveFileDialog>(window->root_view().get());
	label3->set_text(dlg->show() ? dlg->filename() : "Canceled");
}

void App::on_button4_down()
{
	label4->set_text(button4->pressed() ? "Sticky button is pressed" : "Sticky button is unpressed");
}

bool App::update()
{
	// This needs only if nothing is drawn. Otherwise, use display_window().flip().
	window->display_window().request_repaint();

	//window->display_window().flip();

	return true;
}