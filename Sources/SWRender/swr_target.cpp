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

#include "SWRender/precomp.h"
#include "API/SWRender/swr_target.h"
#include "swr_target_provider.h"
#include "setup_swrender_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// SWRenderTarget Construction:

SWRenderTarget::SWRenderTarget()
: DisplayTarget(new SWRenderTargetProvider)
{
}

SWRenderTarget::~SWRenderTarget()
{
}

/////////////////////////////////////////////////////////////////////////////
// SWRenderTarget Attributes:
bool SWRenderTarget::is_current()
{
	DisplayTarget target = Display::get_current_target();
	DisplayTargetProvider *ptr = target.get_provider();
	if (!ptr)
		return false;

	SWRenderTargetProvider *provider = dynamic_cast<SWRenderTargetProvider*>(ptr);
	return (provider != NULL);
}
/////////////////////////////////////////////////////////////////////////////
// SWRenderTarget Operations:
void SWRenderTarget::set_current()
{
	MutexSection mutex_lock(&SetupSWRender_Impl::cl_swrender_mutex);
	if (!SetupSWRender_Impl::cl_swrender_target)
		throw Exception("clanSWRender has not been initialised");
	SetupSWRender_Impl::cl_swrender_target->DisplayTarget::set_current();
}
/////////////////////////////////////////////////////////////////////////////
// SWRenderTarget Implementation:

}
