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

#pragma once

#include "API/GameIDE/UIController/ui_controller_listener.h"

namespace clan
{

class BuildSystem;
class UIRibbonSection;

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

	UIRibbonSection *build_section;
	PushButton *button_build;
	PushButton *button_rebuild;
	PushButton *button_clean;
	PushButton *button_show_output;
};

}
