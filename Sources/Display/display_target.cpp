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
**    Magnus Norddahl
*/

#include "Display/precomp.h"
#include "API/Display/display_target.h"
#include "display_target_impl.h"
#include "API/Display/TargetProviders/display_target_provider.h"
#include "API/Display/display.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// DisplayTarget Construction:

DisplayTarget::DisplayTarget()
{
}

DisplayTarget::DisplayTarget(DisplayTargetProvider *provider)
: impl(std::make_shared<DisplayTarget_Impl>())
{
	impl->set_provider(provider, impl);
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
		return impl->get_provider();
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
	Display::set_current_target(*this);
}

/////////////////////////////////////////////////////////////////////////////
// DisplayTarget Implementation:

}
