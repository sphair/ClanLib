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
*/

#include "precomp.h"
#include "component_types.h"
#include "component_type.h"
#include "custom_component.h"

/////////////////////////////////////////////////////////////////////////////
// ComponentTypes

void ComponentTypes::initialize()
{
	int next_id = 1;
	types.push_back(new CheckBoxComponentType("CheckBox", next_id++, "gfx/check.png"));
	types.push_back(new RadioButtonComponentType("RadioButton", next_id++, "gfx/radio.png"));
	types.push_back(new LabelComponentType("Label", next_id++, "gfx/label.png"));
	types.push_back(new ComponentTypeTemplate<CL_LineEdit>("LineEdit", next_id++, "gfx/lineedit.png"));
	types.push_back(new ComponentTypeTemplate<CL_ComboBox>("ComboBox", next_id++, "gfx/combobox.png"));
	types.push_back(new ComponentTypeTemplate<CL_ListView>("ListView", next_id++, "gfx/listview.png"));
	types.push_back(new ComponentTypeTemplate<CL_MenuBar>("MenuBar", next_id++, "gfx/menu.png"));
	types.push_back(new ButtonComponentType("PushButton", next_id++, "gfx/pushbutton.png"));
	types.push_back(new SliderComponentType("Slider", next_id++, "gfx/slider.png"));
	types.push_back(new ComponentTypeTemplate<CL_Spin>("Spin", next_id++, "gfx/spin.png"));
	types.push_back(new ComponentTypeTemplate<CL_ImageView>("ImageView", next_id++, "gfx/image.png"));
	types.push_back(new ScrollBarHorizontalComponentType("H Scroll Bar", next_id++, "gfx/scroll_horizontal.png"));
	types.push_back(new ScrollBarVerticalComponentType("V Scroll Bar", next_id++, "gfx/scroll_vertical.png"));
	types.push_back(new TabComponentType("Tab", next_id++, "gfx/tab.png"));
	types.push_back(new FrameComponentType("Frame", next_id++, "gfx/frame.png"));
	types.push_back(new ComponentTypeTemplate<CL_StatusBar>("StatusBar", next_id++, "gfx/statusbar.png"));
	types.push_back(new ComponentTypeTemplate<CL_ToolBar>("ToolBar", next_id++, "gfx/toolbar.png"));
	types.push_back(new ComponentTypeTemplate<CustomComponent>("Custom", next_id++, "gfx/custom.png"));
}

void ComponentTypes::deinitialize()
{
	for (std::vector<ComponentType *>::size_type index = 0; index < types.size(); index++)
		delete types[index];
	types.clear();
}

CL_GUIComponent *ComponentTypes::create_component(int id, CL_GUIComponent *parent)
{
	std::vector< CL_SharedPtr<ComponentType> >::size_type index, size;
	size = types.size();
	for (index = 0; index < size; index++)
	{
		if (types[index]->id == id)
		{
			CL_GUIComponent *new_component = types[index]->create_component(parent);
			set_id_name(new_component, id);
			return new_component;
		}
	}
	throw CL_Exception(cl_format("Unable to create component, unknown id: %1", id));
}

void ComponentTypes::set_id_name(CL_GUIComponent *new_component, int id)
{
	if (component_count.find(id) == component_count.end())
		component_count[id] = 1;

	new_component->set_id_name(cl_format("%1%2", new_component->get_type_name(), component_count[id]++));
}

const std::vector<ComponentType *> &ComponentTypes::get_types()
{
	return types;
}

std::vector<ComponentType *> ComponentTypes::types;
std::map<int, int> ComponentTypes::component_count;
