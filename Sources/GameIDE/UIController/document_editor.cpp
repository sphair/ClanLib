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
#include "API/GameIDE/UIController/document_editor.h"
#include "API/GameIDE/UIController/ui_controller.h"

namespace clan
{

DocumentEditor::DocumentEditor(UIController *controller, std::string filename)
: controller(controller), filename(filename)
{
	func_visibility_change().set(this, &DocumentEditor::on_visibility_change);
}

void DocumentEditor::on_visibility_change(bool new_state)
{
	if (new_state)
		controller->set_active(this);
}

void DocumentEditor::set_dirty(bool dirty)
{
	std::string title = get_title();
	if(title.length() > 1 && title[0] == '*' && title[1] == ' ')
		title = title.substr(2);

	if(dirty)
		set_title("* " + title);
	else
		set_title(title);
}

}

