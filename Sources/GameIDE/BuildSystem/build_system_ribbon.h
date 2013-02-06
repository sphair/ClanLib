
#pragma once

#include "../UIController/ui_controller_listener.h"

class BuildSystem;

class BuildSystemRibbon : public UIControllerListener
{
public:
	BuildSystemRibbon(UIController *controller);
	void set_active(DocumentEditor *editor);
	void editor_destroyed(DocumentEditor *editor);

private:
	void on_button_build();
	void on_button_rebuild();
	void on_button_clean();
	void on_button_show_output();

	clan::RibbonSection *build_section;
	clan::PushButton *button_build;
	clan::PushButton *button_rebuild;
	clan::PushButton *button_clean;
	clan::PushButton *button_show_output;
};
