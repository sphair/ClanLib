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
*/

#include "precomp.h"
#include "component_types.h"
#include "component_type.h"
#include "component_type_template.h"
#include "custom_component.h"
#include "button_component_type.h"
#include "checkbox_component_type.h"
#include "frame_component_type.h"
#include "label_component_type.h"
#include "listview_component_type.h"
#include "radio_button_component_type.h"
#include "scrollbar_horz_component_type.h"
#include "scrollbar_vert_component_type.h"
#include "slider_component_type.h"
#include "tab_component_type.h"
#include "imageview_component_type.h"

void ComponentTypes::initialize()
{
	int next_id = 1;

	custom_component = new ComponentTypeTemplate<CustomComponent>("customcomponent", "Custom", next_id++, "ComponentIconCustom");

	types.push_back(new CheckBoxComponentType("checkbox", "CheckBox", next_id++, "ComponentIconCheckBox"));
	types.push_back(new RadioButtonComponentType("radiobutton", "RadioButton", next_id++, "ComponentIconRadioButton"));
	types.push_back(new LabelComponentType("label", "Label", next_id++, "ComponentIconLabel"));
	types.push_back(new ComponentTypeTemplate<LineEdit>("lineedit", "LineEdit", next_id++, "ComponentIconLineEdit"));
	types.push_back(new ComponentTypeTemplate<TextEdit>("textedit", "TextEdit", next_id++, "ComponentIconLineEdit"));
	types.push_back(new ComponentTypeTemplate<ComboBox>("combobox", "ComboBox", next_id++, "ComponentIconComboBox"));
	types.push_back(new ListViewComponentType("listview", "ListView", next_id++, "ComponentIconListView"));
	types.push_back(new ComponentTypeTemplate<MenuBar>("menubar", "MenuBar", next_id++, "ComponentIconMenuBar"));
	types.push_back(new ButtonComponentType("button", "PushButton", next_id++, "ComponentIconPushButton"));
	types.push_back(new SliderComponentType("slider", "Slider", next_id++, "ComponentIconSlider"));
	types.push_back(new ComponentTypeTemplate<Spin>("spin", "Spin", next_id++, "ComponentIconSpin"));
	types.push_back(new ImageViewComponentType("imageview", "ImageView", next_id++, "ComponentIconImageView"));
	types.push_back(new ScrollBarHorizontalComponentType("hscrollbar", "H Scroll Bar", next_id++, "ComponentIconHScrollBar"));
	types.push_back(new ScrollBarVerticalComponentType("vscrollbar", "V Scroll Bar", next_id++, "ComponentIconVScrollBar"));
	types.push_back(new TabComponentType("tab", "Tab", next_id++, "ComponentIconTab"));
	types.push_back(new FrameComponentType("frame", "Frame", next_id++, "ComponentIconFrame"));
	types.push_back(new ComponentTypeTemplate<StatusBar>("statusbar", "StatusBar", next_id++, "ComponentIconStatusBar"));
	types.push_back(new ComponentTypeTemplate<ToolBar>("toolbar", "ToolBar", next_id++, "ComponentIconToolBar"));
	types.push_back(custom_component);
}

void ComponentTypes::deinitialize()
{
	for (std::vector<ComponentType *>::size_type index = 0; index < types.size(); index++)
		delete types[index];
	types.clear();
}

ComponentType *ComponentTypes::find_component(int id)
{
	std::vector<std::shared_ptr<ComponentType> >::size_type index, size;
	size = types.size();
	for (index = 0; index < size; index++)
	{
		if (types[index]->id == id)
		{
			return types[index];
		}
	}
	throw Exception(string_format("Unable to find component type, unknown id: %1", id));
}

ComponentType *ComponentTypes::find_from_xml(const std::string &tag)
{
	std::vector<std::shared_ptr<ComponentType> >::size_type index, size;
	size = types.size();
	for (index = 0; index < size; index++)
	{
		if (types[index]->xmlname == tag)
		{
			return types[index];
		}
	}

	// No matching tag found, assume custom component
	return custom_component;
}

void ComponentTypes::set_id(GUIComponent *new_component, int id)
{
	if (component_count.find(id) == component_count.end())
		component_count[id] = 1;

	new_component->set_id(string_format("%1%2", new_component->get_tag_name(), component_count[id]++));
}

const std::vector<ComponentType *> &ComponentTypes::get_types()
{
	return types;
}

std::vector<ComponentType *> ComponentTypes::types;
std::map<int, int> ComponentTypes::component_count;
ComponentType *ComponentTypes::custom_component;