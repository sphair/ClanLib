/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
**    Magnus Norddahl
**    Mark Page
*/

#include "Display/precomp.h"
#include "display_target_impl.h"
#include "API/Display/TargetProviders/display_target_provider.h"
#include "API/Display/display.h"
#include "API/Display/display_target.h"

namespace clan
{

std::weak_ptr<DisplayTarget_Impl> DisplayTarget_Impl::current_target;
bool DisplayTarget_Impl::current_target_valid = false;

std::vector< std::weak_ptr<DisplayTarget_Impl> > DisplayTarget_Impl::display_targets;

DisplayTarget_Impl::DisplayTarget_Impl()
: provider(nullptr)
{
}

DisplayTarget_Impl::~DisplayTarget_Impl()
{
	if (provider)
	{
		remove_target(self);
		delete provider;
	}
}

void DisplayTarget_Impl::set_provider(DisplayTargetProvider *new_provider, std::weak_ptr<DisplayTarget_Impl> base)
{
	provider =  new_provider;
	self = base;
	display_targets.push_back(base);
}

DisplayTarget DisplayTarget_Impl::get_current_target()
{
	if (!current_target_valid || current_target.expired())
	{
		if (display_targets.empty())
			return DisplayTarget();

		current_target = display_targets[0];
		current_target_valid = true;
	}
	return DisplayTarget(current_target.lock()->self);
}

void DisplayTarget_Impl::set_current_target(const DisplayTarget &target)
{
	current_target = target.impl;
	current_target_valid = true;
}

void DisplayTarget_Impl::remove_target(const DisplayTarget &target)
{
	DisplayTarget_Impl::remove_target(target.impl);
}

void DisplayTarget_Impl::remove_target(std::weak_ptr<DisplayTarget_Impl> target_impl)
{
	if (!target_impl.expired())
	{
		if (current_target.lock() == target_impl.lock())
		{
			current_target_valid = false;
		}

		for (auto it = display_targets.begin(); it != display_targets.end(); ++it)
		{
			if (it->lock() == target_impl.lock())
			{
				display_targets.erase(it);
				break;
			}
		}
	}
}

}
