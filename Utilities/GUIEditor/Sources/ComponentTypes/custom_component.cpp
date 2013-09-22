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

	void on_process_message(std::shared_ptr<clan::GUIMessage> &msg);
	void on_render(clan::Canvas &canvas, const clan::Rect &update_rect);
	void on_resized();

	CustomComponent *component;
};

CustomComponent::CustomComponent(clan::GUIComponent *parent)
: clan::GUIComponent(parent), impl(new CustomComponent_Impl())
{
	set_tag_name("customcomponent");
	func_process_message().set(impl.get(), &CustomComponent_Impl::on_process_message);
	func_render().set(impl.get(), &CustomComponent_Impl::on_render);
	func_resized().set(impl.get(), &CustomComponent_Impl::on_resized);

	impl->component = this;
}

void CustomComponent_Impl::on_process_message(std::shared_ptr<clan::GUIMessage> &msg)
{
/*	std::shared_ptr<clan::GUIMessage_Input> input_msg = std::dynamic_pointer_cast<clan::GUIMessage_Input>(msg);X	if (input_msg)
	{
		clan::GUIMessage_Input input_msg = msg;
		const clan::InputEvent &e = input_msg->input_event;

		if (e.type == clan::InputEvent::pressed && e.id == clan::mouse_left)
		{
		}
		else if (e.type == clan::InputEvent::released && e.id == clan::mouse_left)
		{
		}
		else if (e.type == clan::InputEvent::pointer_moved)
		{
		}
	}
*/
}

void CustomComponent_Impl::on_render(clan::Canvas &canvas, const clan::Rect &update_rect)
{
	clan::Rect g = component->get_geometry().get_size();

	canvas.fill_rect( g, clan::Gradient(clan::Colorf::lightgray, clan::Colorf::floralwhite));
	canvas.draw_box( g, clan::Colorf::gray);
	g.shrink(2,2,2,2);
	canvas.draw_box( g, clan::Colorf::lightskyblue);
}

void CustomComponent_Impl::on_resized()
{
}
