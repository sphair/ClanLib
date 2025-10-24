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
**    Magnus Norddahl
*/

#include "Display/precomp.h"
#include "API/Display/display_target.h"
#include "API/Display/TargetProviders/display_target_provider.h"
#include "API/Display/display.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayTarget_Impl class:

class CL_DisplayTarget_Impl
{
public:
	CL_DisplayTarget_Impl()
	: provider(0)
	{
	}

	~CL_DisplayTarget_Impl()
	{
		delete provider;
	}

	CL_DisplayTargetProvider *provider;
};

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayTarget Construction:

CL_DisplayTarget::CL_DisplayTarget()
{
	if (CL_Display::get_current_target().is_null() == false)
		*this = CL_Display::get_current_target();
}

CL_DisplayTarget::CL_DisplayTarget(CL_DisplayTargetProvider *provider)
: impl(new CL_DisplayTarget_Impl)
{
	impl->provider = provider;
	CL_Display::set_current_target(*this, true);
}

CL_DisplayTarget::CL_DisplayTarget(const CL_WeakPtr<CL_DisplayTarget_Impl> impl)
: impl(impl.lock())
{
}

CL_DisplayTarget::~CL_DisplayTarget()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayTarget Attributes:

CL_DisplayTargetProvider *CL_DisplayTarget::get_provider()
{
	if (impl)
		return impl->provider;
	else
		return 0;
}

void CL_DisplayTarget::throw_if_null() const
{
	if (!impl)
		throw CL_Exception("CL_DisplayTarget is null");
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayTarget Operations:

void CL_DisplayTarget::set_current()
{
	CL_Display::set_current_target(*this, false);
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayTarget Implementation:
