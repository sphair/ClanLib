
#pragma once

#include "../Workspace/dockable_component.h"
#include "rollout_header.h"
#include "rollout.h"

namespace clan
{

class RolloutList : public DockableComponent
{
public:
	RolloutList();

	void clear();

	template<typename Type>
	Type *add_rollout();
};

template<typename Type>
Type *RolloutList::add_rollout()
{
	GUIComponent *frame = new GUIComponent(this);
	frame->set_tag_name("rollout-frame");

	RolloutHeader *header = new RolloutHeader(frame);
	return new Type(this, header, frame);
}

}
