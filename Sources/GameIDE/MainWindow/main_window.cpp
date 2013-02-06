/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
#include "GameIDE/precomp.h"
#include "API/GameIDE/MainWindow/main_window.h"
#include "Workspace/workspace.h"
#include "SolutionExplorer/solution_explorer.h"
#include "PropertyManager/property_manager.h"
#include "WelcomePage/welcome_page.h"
#include "dlg_add_new_project.h"

namespace clan
{

EditorMainWindow::EditorMainWindow(GUIManager *manager, const FileItemTypeFactory &new_file_item_type_factory)
: GUIComponent(manager, get_top_level_description(), "editor-main-window"), ribbon(0), workspace(0), file_item_type_factory(new_file_item_type_factory)
{
	DisplayWindow dispwindow = get_display_window();
	dispwindow.set_large_icon(PNGProvider::load("Resources/GameIDE/Icons/gameide-48.png"));
	dispwindow.set_small_icon(PNGProvider::load("Resources/GameIDE/Icons/gameide-16.png"));

	file_item_type_factory.register_type(&text_editor_file_item_type);

	func_close().set(this, &EditorMainWindow::on_close);
	func_resized().set(this, &EditorMainWindow::on_resized);

	ribbon = new Ribbon(this);
	workspace = new Workspace(this);
	ui_controller.reset(new UIController(ribbon, model));
	set_visible(true, true);

	Canvas &canvas = get_canvas();
	ResourceManager resources = get_resources();

	ribbon->get_menu()->add_item(Image(canvas, "IconNew24", &resources), "New").set(this, &EditorMainWindow::on_item_new);
	ribbon->get_menu()->add_item(Image(canvas, "IconOpen24", &resources), "Open").set(this, &EditorMainWindow::on_item_open);
	ribbon->get_menu()->add_item(Image(canvas, "IconSave24", &resources), "Save").set(this, &EditorMainWindow::on_item_save);
	ribbon->get_menu()->add_item(Image(canvas, "IconSaveAs24", &resources), "Save As..").set(this, &EditorMainWindow::on_item_save_as);
	ribbon->get_menu()->add_separator();
	ribbon->get_menu()->add_item(Image(canvas, "IconAbout24", &resources), "About Game IDE").set(this, &EditorMainWindow::on_item_about);
	ribbon->get_menu()->add_separator();
	ribbon->get_menu()->add_item(Image(canvas, "IconExit24", &resources), "Exit").set(this, &EditorMainWindow::on_item_exit);

	//ui_controller->register_ribbon_setup("RibbonSetupEditorMainWindow");

	file_item_type_factory.register_ui(ui_controller.get());

	explorer = new SolutionExplorer(file_item_type_factory, ui_controller.get());
	property_manager = new PropertyManager();
	WelcomePage *welcome_page = new WelcomePage();

	workspace->load_dockables_state();

	welcome_page->show_as_docked(workspace->get_root_dock());
	for (size_t i = 0; i < ui_controller->get_dockables().size(); i++)
		ui_controller->get_dockables()[i]->show();

	if(FileHelp::file_exists("Zombie\\Zombie.gamesolution"))
		model.open_solution("Zombie\\Zombie.gamesolution");
	else
		model.open_solution("Default\\Default.gamesolution");

	explorer->show(); // Small hack until dockable state is improved

	update_layout();
}

bool EditorMainWindow::on_close()
{
	on_item_exit();
	return true;
}

void EditorMainWindow::on_resized()
{
	update_layout();
}

void EditorMainWindow::on_item_new()
{
	DlgAddNewProject dlg(this, model);
	if(dlg.exec() == 1)
	{
		std::string title = string_format("%1 - ClanLib Game Builder", model.solution.name);
		get_display_window().set_title(title);
	}
}

void EditorMainWindow::on_item_exit()
{
	workspace->save_dockables_state();
	exit_with_code(0);
}

void EditorMainWindow::on_item_open()
{
	OpenFileDialog dlg(this);
	dlg.add_filter("Game Solution Files (*.gamesolution)", "*.gamesolution", true);
	dlg.add_filter("Project Files (*.gameproject)", "*.gameproject", false);
	dlg.add_filter("All Files (*.*)", "*.*", false);
	if (dlg.show())
	{
		model.open_solution(dlg.get_filename());
	}
}

void EditorMainWindow::on_item_save()
{
	model.save_all();
}

void EditorMainWindow::on_item_save_as()
{
	SaveFileDialog dlg(this);
	dlg.add_filter("Game Solution Files (*.gamesolution)", "*.gamesolution", true);
	dlg.add_filter("Project Files (*.gameproject)", "*.gameproject", false);
	dlg.add_filter("All Files (*.*)", "*.*", false);
	if (dlg.show())
	{
	}
}

void EditorMainWindow::on_item_about()
{
}

GUITopLevelDescription EditorMainWindow::get_top_level_description()
{
	ScreenInfo screen_info;
	int primary_screen_index = 0;
	std::vector<Rect> screen_boxes = screen_info.get_screen_geometries(primary_screen_index);

	GUITopLevelDescription desc;
	desc.set_size(Size(screen_boxes[primary_screen_index].get_width() * 90 / 100, (screen_boxes[primary_screen_index].get_height() - 50) * 90 / 100), true);
	desc.set_title("ClanLib Game Builder");
	desc.set_allow_resize(true);
	desc.show_caption(true);
	desc.set_visible(false);
	return desc;
}

}

