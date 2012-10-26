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
**    Magnus Norddahl
*/

#include "Display/precomp.h"
#include "API/Display/display_target.h"
#include "API/Display/TargetProviders/display_target_provider.h"
#include "API/Display/display.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// DisplayTarget_Impl class:

class DisplayTarget_Impl
{
public:
	DisplayTarget_Impl()
	: provider(0)
	{
	}

	~DisplayTarget_Impl()
	{
		delete provider;
	}

	DisplayTargetProvider *provider;
};

/////////////////////////////////////////////////////////////////////////////
// DisplayTarget Construction:

DisplayTarget::DisplayTarget()
{
	if (Display::get_current_target().is_null() == false)
		*this = Display::get_current_target();
}

DisplayTarget::DisplayTarget(DisplayTargetProvider *provider)
: impl(new DisplayTarget_Impl)
{
	impl->provider = provider;
	Display::set_current_target(*this, true);
}

DisplayTarget::DisplayTarget(const std::weak_ptr<DisplayTarget_Impl> impl)
: impl(impl.lock())
{
}

DisplayTarget::~DisplayTarget()
{
}

/////////////////////////////////////////////////////////////////////////////
// DisplayTarget Attributes:

DisplayTargetProvider *DisplayTarget::get_provider()
{
	if (impl)
		return impl->provider;
	else
		return 0;
}

void DisplayTarget::throw_if_null() const
{
	if (!impl)
		throw Exception("DisplayTarget is null");
}

/////////////////////////////////////////////////////////////////////////////
// DisplayTarget Operations:

void DisplayTarget::set_current()
{
	Display::set_current_target(*this, false);
}

/////////////////////////////////////////////////////////////////////////////
// DisplayTarget Implementation:

}
