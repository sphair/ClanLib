
#pragma once

#include "ui_screen.h"
	
class CharacterScreen : public UIScreen
{
public:
	CharacterScreen(CL_GUIManager *gui);
	~CharacterScreen();

private:
	CL_PushButton *button_character1;
	CL_PushButton *button_character2;
	CL_PushButton *button_character3;
};
