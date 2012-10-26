
#pragma once

class PostProcessScene;
class PostProcessEffectTransparency;
class PostProcessEffectDarken;
class SplashScreen;
class ErrorScreen;

class Example
{
public:
	int exec();

private:
	void on_window_close();
	void on_timer();

	CL_SlotContainer slots;
	CL_GUIManager gui;

	PostProcessEffectTransparency *effect_transparency;
	PostProcessEffectDarken *effect_darken;

	SplashScreen *splash_screen;
	ErrorScreen *error_screen;

	CL_Timer timer;
};
