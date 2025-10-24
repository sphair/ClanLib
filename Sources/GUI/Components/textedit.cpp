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
**    Magnus Norddahl
*/

#include "GUI/precomp.h"
#include "API/Core/Text/string_format.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/textedit.h"
#include "API/Core/Math/rect.h"

/////////////////////////////////////////////////////////////////////////////
// CL_TextEdit_Impl Class:

class CL_TextEdit_Impl
{
public:
	void on_process_message(CL_GUIMessage &msg);

	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);

	CL_TextEdit *textedit;
};

/////////////////////////////////////////////////////////////////////////////
// CL_TextEdit Construction:

CL_TextEdit::CL_TextEdit(const CL_GUIComponentDescription &desc, CL_GUIComponent *parent)
: CL_GUIComponent(parent), impl(new CL_TextEdit_Impl)
{
	set_type_name("textedit");
	impl->textedit = this;
	func_process_message().set(impl.get(), &CL_TextEdit_Impl::on_process_message);
	func_render().set(impl.get(), &CL_TextEdit_Impl::on_render);
}

CL_TextEdit::~CL_TextEdit()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_TextEdit Attributes:

CL_TextEdit *CL_TextEdit::get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id)
{
	CL_TextEdit *object = NULL;
	if (reference_component)
		object = dynamic_cast<CL_TextEdit*>(reference_component->get_named_item(id));

	if (!object)
		throw CL_Exception(cl_format("Cannot find CL_TextEdit named item: %1", id));

	return object;
}

/////////////////////////////////////////////////////////////////////////////
// CL_TextEdit Operations:

/////////////////////////////////////////////////////////////////////////////
// CL_TextEdit Implementation:

void CL_TextEdit_Impl::on_process_message(CL_GUIMessage &msg)
{
}

void CL_TextEdit_Impl::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_GUIThemePart part_component(textedit);
	CL_Rect rect = textedit->get_geometry();
	part_component.render_box(gc, rect, update_rect);
}
