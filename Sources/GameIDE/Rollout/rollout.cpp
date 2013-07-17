/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
#include "API/GameIDE/Rollout/rollout.h"
#include "API/GameIDE/Rollout/rollout_header.h"
#include "API/GameIDE/Rollout/rollout_list.h"

namespace clan
{

Rollout::Rollout(RolloutList *list, RolloutHeader *header, GUIComponent *parent)
	: GUIComponent(parent), header(header)
{
	set_tag_name("rollout");
}

Rollout::~Rollout()
{
}

void Rollout::set_title(const std::string &title)
{
	header->set_title(title);
}

LineEdit *Rollout::create_line_edit(const std::string &label_text)
{
	GUIComponent *row = new GUIComponent(this);
	row->set_tag_name("rollout-row");

	Label *label = new Label(row);
	label->set_text(label_text);

	return new LineEdit(row);
}

LineEdit *Rollout::create_line_edit_button(const std::string &label_text, PushButton **button)
{
	GUIComponent *row = new GUIComponent(this);
	row->set_tag_name("rollout-row");

	Label *label = new Label(row);
	label->set_text(label_text);

	LineEdit *line_edit = new LineEdit(row);

	*button = new PushButton(row);

	return line_edit;
}

}
