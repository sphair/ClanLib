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
**    Mark Page
*/

#include "Core/precomp.h"
#include "API/Core/System/setup_core.h"
#include "API/Core/System/exception.h"
#include "API/Core/System/thread_local_storage.h"
#include "API/Core/System/system.h"

namespace clan
{


class SetupCore_Impl
{
public:
	static void init();
	static void deinit();
};

/////////////////////////////////////////////////////////////////////////////
// SetupCore Construction:

SetupCore::SetupCore()
{
	SetupCore_Impl::init();
}


SetupCore::~SetupCore()
{
	SetupCore_Impl::deinit();
}

/////////////////////////////////////////////////////////////////////////////
// SetupCore Implementation:
void SetupCore_Impl::init()
{
	ThreadLocalStorage::create_initial_instance();
#ifndef CL_DISABLE_SSE2
	if (!System::detect_cpu_extension(System::sse2))
	{
		throw Exception("Sorry, ClanLib 3.0 and higher requires a processor capable of SSE2 instructions. (Update your CPU)");
	}
#endif

#ifdef WIN32
	::CoInitialize(0);
#endif
}

void SetupCore_Impl::deinit()
{
#ifdef WIN32
	::CoUninitialize();
#endif
}

}
