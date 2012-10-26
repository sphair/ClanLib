
#pragma once

#include "postprocess_effect.h"

class UIScreen : public CL_GUIComponent
{
public:
	UIScreen(CL_GUIManager *gui);
	~UIScreen();

public:
	void set_postprocess_effect(std::auto_ptr<PostProcessEffect> effect);
	void set_postprocess_effect(PostProcessEffect *effect) { set_postprocess_effect(std::auto_ptr<PostProcessEffect>(effect)); }
	PostProcessEffect *get_postprocess_effect() const { return effect.get(); }
	void remove_postprocess_effect();

private:
	static CL_GUITopLevelDescription get_toplevel_description();

	std::auto_ptr<PostProcessEffect> effect;
};
