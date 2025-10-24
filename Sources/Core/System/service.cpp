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

#include "Core/precomp.h"
#include "API/Core/System/service.h"
#include "service_impl.h"
#ifdef WIN32
#include "Win32/service_win32.h"
#else
#include "Unix/service_unix.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_Service Construction:

CL_Service::CL_Service(const CL_String &service_name)
{
#ifdef WIN32
	impl = CL_SharedPtr<CL_Service_Impl>(new CL_Service_Win32(this, service_name));
#else
	impl = CL_SharedPtr<CL_Service_Impl>(new CL_Service_Unix(this, service_name));
#endif
}

CL_Service::~CL_Service()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Service Attributes:

const CL_String &CL_Service::get_service_name() const
{
	return impl->service_name;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Service Operations:

int CL_Service::main(int argc, char **argv)
{
	return impl->main(argc, argv);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Service Implementation:
