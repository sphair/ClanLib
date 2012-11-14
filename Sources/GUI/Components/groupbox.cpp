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
**    Harry Storbacka
**    Kenneth Gangstoe
**    Magnus Norddahl
*/

#include "GUI/precomp.h"
#include "API/Core/Text/string_format.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/groupbox.h"
#include "../gui_css_strings.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GroupBox_Impl Class:

class GroupBox_Impl
{
public:
	void on_process_message(std::shared_ptr<GUIMessage> &msg);
	void on_render(Canvas &canvas, const Rect &update_rect);

	GroupBox *groupbox;
};

/////////////////////////////////////////////////////////////////////////////
// GroupBox Construction:

GroupBox::GroupBox(GUIComponent *parent)
: GUIComponent(parent), impl(new GroupBox_Impl)
{
	set_tag_name(CssStr::GroupBox::type_name);
	impl->groupbox = this;
	func_process_message().set(impl.get(), &GroupBox_Impl::on_process_message);
	func_render().set(impl.get(), &GroupBox_Impl::on_render);
}

GroupBox::~GroupBox()
{
}

/////////////////////////////////////////////////////////////////////////////
// GroupBox Attributes:

GroupBox *GroupBox::get_named_item(GUIComponent *reference_component, const std::string &id)
{
	GroupBox *object = NULL;
	if (reference_component)
		object = dynamic_cast<GroupBox*>(reference_component->get_named_item(id));

	if (!object)
		throw Exception(string_format("Cannot find GroupBox named item: %1", id));

	return object;
}
/////////////////////////////////////////////////////////////////////////////
// GroupBox Operations:

/////////////////////////////////////////////////////////////////////////////
// GroupBox Implementation:

void GroupBox_Impl::on_process_message(std::shared_ptr<GUIMessage> &msg)
{
}

void GroupBox_Impl::on_render(Canvas &canvas, const Rect &update_rect)
{

}

}
