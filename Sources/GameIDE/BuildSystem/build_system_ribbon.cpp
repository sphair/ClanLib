
#include "precomp.h"
#include "build_system_ribbon.h"
#include "build_system.h"
#include "UIController/document_editor.h"
#include "UIController/ui_controller.h"

using namespace clan;

BuildSystemRibbon::BuildSystemRibbon(UIController *controller)
	: UIControllerListener(controller)
{
	build_section = controller->get_ribbon_section("Home", "Build");

	ResourceManager resources = build_section->get_resources();
	Canvas canvas = build_section->get_canvas();

	RibbonGroup *build_group1 = new RibbonGroup(build_section);
	RibbonGroup *build_group2 = new RibbonGroup(build_section);

	button_build = new PushButton(build_group1);
	button_build->set_text("Build");
	button_build->func_clicked().set(this, &BuildSystemRibbon::on_button_build);

	button_rebuild = new PushButton(build_group1);
	button_rebuild->set_text("Rebuild");
	button_rebuild->func_clicked().set(this, &BuildSystemRibbon::on_button_build);

	button_clean = new PushButton(build_group1);
	button_clean->set_text("Clean");
	button_clean->func_clicked().set(this, &BuildSystemRibbon::on_button_clean);

	button_show_output = new PushButton(build_group2);
	button_show_output->set_text("Output");
	button_show_output->func_clicked().set(this, &BuildSystemRibbon::on_button_show_output);

	// Always show the build section
	get_controller()->show_ribbon_section("Home", "Build");
}

void BuildSystemRibbon::set_active(DocumentEditor *doc_editor)
{
}

void BuildSystemRibbon::editor_destroyed(DocumentEditor *doc_editor)
{
}

void BuildSystemRibbon::on_button_build()
{
}

void BuildSystemRibbon::on_button_rebuild()
{
}

void BuildSystemRibbon::on_button_clean()
{
}

void BuildSystemRibbon::on_button_show_output()
{
}
