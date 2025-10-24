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

#include "SWRender/precomp.h"
#include "swr_occlusion_query_provider.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SWRenderOcclusionQueryProvider Construction:

CL_SWRenderOcclusionQueryProvider::CL_SWRenderOcclusionQueryProvider()
{
}

CL_SWRenderOcclusionQueryProvider::~CL_SWRenderOcclusionQueryProvider()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_SWRenderOcclusionQueryProvider Attributes:

bool CL_SWRenderOcclusionQueryProvider::is_result_ready() const
{
	return false;
}

int CL_SWRenderOcclusionQueryProvider::get_result() const
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SWRenderOcclusionQueryProvider Operations:

void CL_SWRenderOcclusionQueryProvider::begin()
{
}

void CL_SWRenderOcclusionQueryProvider::end()
{
}

void CL_SWRenderOcclusionQueryProvider::create()
{
}

void CL_SWRenderOcclusionQueryProvider::destroy()
{
	delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SWRenderOcclusionQueryProvider Implementation:
