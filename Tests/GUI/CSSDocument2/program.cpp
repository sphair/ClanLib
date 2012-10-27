
#include "precomp.h"
#include "program.h"
#include <ClanLib/application.h>

using namespace clan;

Application clanapp(&Program::main);

int Program::main(const std::vector<std::string> &args)
{
	SetupCore setup_core;
	SetupDisplay setup_display;
	SetupGL setup_gl;
	SetupGUI setup_gui;

	GUIManager gui(".");

	GUITopLevelDescription window_desc;
	window_desc.set_title("GUI Test");
	window_desc.set_size(Size(1024, 768), false);
	window_desc.set_allow_resize(true);

	GUIComponent *root = new GUIComponent(&gui, window_desc);
	root->func_close().set(&Program::on_close, root);
	GUIComponent *child1 = new GUIComponent(root);
	GUIComponent *child11 = new GUIComponent(child1);
	GUIComponent *child2 = new GUIComponent(root);
	GUIComponent *child22 = new GUIComponent(child2);
	GUIComponent *child23 = new GUIComponent(child2);

	gui.exec();

	return 0;
}

bool Program::on_close(GUIComponent *component)
{
	component->exit_with_code(0);
	return true;
}
