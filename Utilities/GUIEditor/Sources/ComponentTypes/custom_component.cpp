/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
*/

#include "precomp.h"
#include "custom_component.h"

class CustomComponent_Impl
{
public:
	CustomComponent_Impl() : component(0)
	{
	}

	void on_process_message(CL_GUIMessage &msg);
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_resized();

	CustomComponent *component;
};

CustomComponent::CustomComponent(CL_GUIComponent *parent)
: CL_GUIComponent(parent), impl(new CustomComponent_Impl())
{
	set_type_name("customcomponent");
	func_process_message().set(impl.get(), &CustomComponent_Impl::on_process_message);
	func_render().set(impl.get(), &CustomComponent_Impl::on_render);
	func_resized().set(impl.get(), &CustomComponent_Impl::on_resized);

	impl->component = this;
}

void CustomComponent_Impl::on_process_message(CL_GUIMessage &msg)
{
/*	if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
	{
		CL_GUIMessage_Input input_msg = msg;
		CL_InputEvent e = input_msg.get_event();

		if (e.type == CL_InputEvent::pressed && e.id == CL_MOUSE_LEFT)
		{
		}
		else if (e.type == CL_InputEvent::released && e.id == CL_MOUSE_LEFT)
		{
		}
		else if (e.type == CL_InputEvent::pointer_moved)
		{
		}
	}
*/
}

void CustomComponent_Impl::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_Rect g = component->get_geometry().get_size();

	gc.reset_texture(0);
	CL_Draw::gradient_fill(gc, g, CL_Gradient(CL_Colorf::lightgray, CL_Colorf::floralwhite));
	CL_Draw::box(gc, g, CL_Colorf::gray);
	g.shrink(2,2,2,2);
	CL_Draw::box(gc, g, CL_Colorf::lightskyblue);
}

void CustomComponent_Impl::on_resized()
{
}
