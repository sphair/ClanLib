
#include "precomp.h"
#include "component_type.h"
#include "custom_component.h"

/////////////////////////////////////////////////////////////////////////////
// ComponentTypes

void ComponentTypes::initialize()
{
	int next_id = 1;
	types.push_back(new ComponentTypeTemplate<CL_CheckBox>("CheckBox", next_id++, "gfx/check.png"));
	types.push_back(new ComponentTypeTemplate<CL_RadioButton>("RadioButton", next_id++, "gfx/radio.png"));
	types.push_back(new ComponentTypeTemplate<CL_Label>("Label", next_id++, "gfx/label.png"));
	types.push_back(new ComponentTypeTemplate<CL_LineEdit>("LineEdit", next_id++, "gfx/lineedit.png"));
	types.push_back(new ComponentTypeTemplate<CL_ComboBox>("ComboBox", next_id++, "gfx/combobox.png"));
	types.push_back(new ComponentTypeTemplate<CL_ListView>("ListView", next_id++, "gfx/listview.png"));
	types.push_back(new ComponentTypeTemplate<CL_MenuBar>("MenuBar", next_id++, "gfx/menu.png"));
	types.push_back(new ComponentTypeTemplate<CL_PushButton>("PushButton", next_id++, "gfx/pushbutton.png"));
	types.push_back(new ComponentTypeTemplate<CL_Slider>("Slider", next_id++, "gfx/slider.png"));
	types.push_back(new ComponentTypeTemplate<CL_Spin>("Spin", next_id++, "gfx/spin.png"));
	types.push_back(new ComponentTypeTemplate<CL_ImageView>("ImageView", next_id++, "gfx/image.png"));
	types.push_back(new ComponentTypeTemplate<CL_ScrollBar>("Scroll Bar", next_id++, "gfx/scroll.png"));
	types.push_back(new ComponentTypeTemplate<CL_Tab>("Tab", next_id++, "gfx/tab.png"));
	types.push_back(new ComponentTypeTemplate<CL_Frame>("Frame", next_id++, "gfx/frame.png"));
	types.push_back(new ComponentTypeTemplate<CL_StatusBar>("StatusBar", next_id++, "gfx/statusbar.png"));
	types.push_back(new ComponentTypeTemplate<CL_ToolBar>("ToolBar", next_id++, "gfx/toolbar.png"));
//	types.push_back(new ComponentTypeTemplate<CL_TextEdit>("Text", next_id++, "gfx/custom.png"));
//	types.push_back(new ComponentTypeTemplate<CL_ToolTip>("Tip", next_id++, "gfx/custom.png"));
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
			return types[index]->create_component(parent);
	}
	throw CL_Exception(cl_format("Unable to create component, unknown id: %1", id));
}

const std::vector<ComponentType *> &ComponentTypes::get_types()
{
	return types;
}

std::vector<ComponentType *> ComponentTypes::types;
