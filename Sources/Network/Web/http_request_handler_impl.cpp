/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

#include "Network/precomp.h"
#include "http_request_handler_impl.h"
#include "API/Network/Web/http_request_handler_provider.h"

/////////////////////////////////////////////////////////////////////////////
// CL_HTTPRequestHandler_Impl Construction:

CL_HTTPRequestHandler_Impl::CL_HTTPRequestHandler_Impl()
: provider(0)
{
}

CL_HTTPRequestHandler_Impl::~CL_HTTPRequestHandler_Impl()
{
	if (provider)
		provider->destroy();
}

/////////////////////////////////////////////////////////////////////////////
// CL_HTTPRequestHandler_Impl Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_HTTPRequestHandler_Impl Operations:

/////////////////////////////////////////////////////////////////////////////
// CL_HTTPRequestHandler_Impl Implementation:
