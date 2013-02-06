
#pragma once

#include "../Workspace/dockable_component.h"

class PropertyManager : public DockableComponent
{
public:
	PropertyManager();

private:
	void on_resized();

	clan::ListView *listview;
};
