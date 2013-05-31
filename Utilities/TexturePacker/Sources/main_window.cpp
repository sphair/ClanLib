/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
#include "main_window.h"
#include "texture_packer.h"
#include "sprite_component.h"
#include "texturegroup_component.h"
#include "view_workspace.h"
#include "view.h"
#include "Views/resource_viewer_view.h"
#include "Views/welcome_view.h"
#include "Views/texture_packer_view.h"
#include "Views/css_packer_view.h"
#include "Views/ico_creator_view.h"
#include <algorithm>

MainWindow::MainWindow(GUIManager *gui, DisplayCache *resources)
: GUIComponent(gui, get_window_description(), "mainwindow"), resources(resources), workspace(0)
{
	func_close().set(this, &MainWindow::on_close);
	func_resized().set(this, &MainWindow::on_resized);

	workspace = new ViewWorkspace(this);
	workspace->cb_view_close.set(this, &MainWindow::on_view_close);

	create_components();

	create_welcome_view();
	create_texture_packer_view();
	create_css_packer_view();
	create_ico_creator_view();
	create_resource_viewer_view();

	on_resized();
}

MainWindow::~MainWindow()
{
}

GUITopLevelDescription MainWindow::get_window_description()
{
	GUITopLevelDescription desc;
	desc.set_title("ClanLib Texture Packer");
	desc.set_allow_resize(true);
	desc.set_position(Rect(200, 200, 1024, 768), false);
	return desc;
}

void MainWindow::add_view(View *view)
{
	views.push_back(view);
	workspace->add_view(view);
	workspace->show_view(view);
}

void MainWindow::remove_view(View *view)
{
	workspace->remove_view(view);
	int index = find_view_index(view);
	delete views[index];
	views.erase(views.begin() + index);
}

int MainWindow::find_view_index(View *view)
{
	int size = views.size();
	for (int i=0; i<size; i++)
	{
		if (views[i] == view)
		{
			return i;
		}
	}
	throw Exception("View not found in collection");
}

void MainWindow::on_view_close(View *view)
{
	remove_view(view);
}

void MainWindow::create_welcome_view()
{
	View *view = new WelcomeView(workspace, this);
	view->set_closable(false);
	add_view(view);
}

void MainWindow::create_resource_viewer_view()
{
	add_view(new ResourceViewerView(workspace, this));
}

void MainWindow::create_texture_packer_view()
{
	add_view(new TexturePackerView(workspace, this));
}

void MainWindow::create_css_packer_view()
{
	add_view(new CssPackerView(workspace, this));
}

void MainWindow::create_ico_creator_view()
{
	add_view(new IcoCreatorView(workspace, this));
}

void MainWindow::create_components()
{
	menubar = new MenuBar(this);

	PopupMenu menu_file;

	PopupMenuItem item_new_resource_viewer = menu_file.insert_item("New resource viewer");
	item_new_resource_viewer.func_clicked().set(this, &MainWindow::create_resource_viewer_view);

	PopupMenuItem item_new_texture_packer = menu_file.insert_item("New texture packer");
	item_new_texture_packer.func_clicked().set(this, &MainWindow::create_texture_packer_view);
	
	PopupMenuItem item_new_css_packer = menu_file.insert_item("New CSS packer");
	item_new_css_packer.func_clicked().set(this, &MainWindow::create_css_packer_view);
	
	PopupMenuItem item_exit = menu_file.insert_item("Exit");
	item_exit.func_clicked().set(this, &MainWindow::on_close2);

	menubar->add_menu("File", menu_file);
}

bool MainWindow::on_close()
{
	exit_with_code(0);
	return true;
}

void MainWindow::on_close2()
{
	exit_with_code(0);
}
void MainWindow::on_resized()
{
	Rect client_area = get_content_box();

	menubar->set_geometry(Rect(client_area.left, client_area.top, client_area.right, client_area.top + 22));
	workspace->set_geometry(Rect(client_area.left, client_area.top + 22, client_area.right, client_area.bottom));
}
