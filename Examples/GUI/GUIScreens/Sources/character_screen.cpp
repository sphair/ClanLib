
#include "precomp.h"
#include "character_screen.h"

CharacterScreen::CharacterScreen(CL_GUIManager *gui)
: UIScreen(gui)
{
	button_character1 = new CL_PushButton(this);
	button_character1->set_geometry(CL_Rect(10, 10, CL_Size(300, 40)));
	button_character1->set_text("Select Character 1");

	button_character2 = new CL_PushButton(this);
	button_character2->set_geometry(CL_Rect(10, 50, CL_Size(300, 40)));
	button_character2->set_text("Select Character 2");

	button_character3 = new CL_PushButton(this);
	button_character3->set_geometry(CL_Rect(10, 90, CL_Size(300, 40)));
	button_character3->set_text("Select Character 3");
}

CharacterScreen::~CharacterScreen()
{
}
