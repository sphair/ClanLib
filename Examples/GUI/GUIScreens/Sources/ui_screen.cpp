
#include "precomp.h"
#include "ui_screen.h"
#include "postprocess_effect.h"

UIScreen::UIScreen(CL_GUIManager *gui)
: CL_GUIComponent(gui, get_toplevel_description())
{
}

UIScreen::~UIScreen()
{
}

CL_GUITopLevelDescription UIScreen::get_toplevel_description()
{
	CL_GUITopLevelDescription desc;
	desc.set_visible(false);
	return desc;
}

void UIScreen::set_postprocess_effect(std::auto_ptr<PostProcessEffect> new_effect)
{
	effect = new_effect;
}

void UIScreen::remove_postprocess_effect()
{
	effect.reset();
}
