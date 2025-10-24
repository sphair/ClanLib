/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

/////////////////////////////////////////////////////////////////////////////
// CL_GroupBox_Impl Class:

class CL_GroupBox_Impl
{
public:
	void on_process_message(CL_GUIMessage &msg);
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_style_changed();

	CL_GroupBox *groupbox;
};

/////////////////////////////////////////////////////////////////////////////
// CL_GroupBox Construction:

CL_GroupBox::CL_GroupBox(CL_GUIComponent *parent)
: CL_GUIComponent(parent), impl(new CL_GroupBox_Impl)
{
	set_type_name(CssStr::GroupBox::type_name);
	impl->groupbox = this;
	func_process_message().set(impl.get(), &CL_GroupBox_Impl::on_process_message);
	func_render().set(impl.get(), &CL_GroupBox_Impl::on_render);
	func_style_changed().set(impl.get(), &CL_GroupBox_Impl::on_style_changed);
}

CL_GroupBox::~CL_GroupBox()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GroupBox Attributes:

CL_GroupBox *CL_GroupBox::get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id)
{
	CL_GroupBox *object = NULL;
	if (reference_component)
		object = dynamic_cast<CL_GroupBox*>(reference_component->get_named_item(id));

	if (!object)
		throw CL_Exception(cl_format("Cannot find CL_GroupBox named item: %1", id));

	return object;
}
/////////////////////////////////////////////////////////////////////////////
// CL_GroupBox Operations:

/////////////////////////////////////////////////////////////////////////////
// CL_GroupBox Implementation:

void CL_GroupBox_Impl::on_process_message(CL_GUIMessage &msg)
{
}

void CL_GroupBox_Impl::on_style_changed()
{
}

void CL_GroupBox_Impl::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_GUIThemePart part_component(groupbox);
	CL_Rect rect(0, 0, groupbox->get_geometry().get_size());
	part_component.render_box(gc, rect, update_rect);
}
