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
**    Mark Page
*/

#include "Core/precomp.h"
#include "core_global.h"

CL_CoreGlobal cl_core_global;

#ifdef WIN32
#elif !defined(HAVE_TLS)
#else
__thread CL_ThreadLocalStorage_Impl *CL_CoreGlobal::cl_tls_impl = 0;
#endif

// This class controls the destruction order of clanCore global variables

/////////////////////////////////////////////////////////////////////////////
// CL_CoreGlobal Construction:

CL_CoreGlobal::CL_CoreGlobal()
{
#ifdef WIN32
	cl_tls_index = TLS_OUT_OF_INDEXES;
#elif !defined(HAVE_TLS)
	cl_tls_index_created = false;
	cl_tls_index = 0;
#else
	cl_tls_impl = NULL;
#endif

	cl_tls = NULL;
}

CL_CoreGlobal::~CL_CoreGlobal()
{
	destroy_tls();
}

/////////////////////////////////////////////////////////////////////////////
// CL_CoreGlobal Implementation:

void CL_CoreGlobal::destroy_tls()
{

#ifdef WIN32
	if (cl_tls)
	{
		delete (cl_tls);
		cl_tls = NULL;
	}
	if (cl_tls_index != TLS_OUT_OF_INDEXES)
	{
		TlsFree(cl_tls_index);
		cl_tls_index = TLS_OUT_OF_INDEXES;
	}

#elif !defined(HAVE_TLS)
	if (cl_tls)
	{
		delete (cl_tls);
		cl_tls = NULL;
	}

	if (cl_tls_index_created)
	{
		pthread_key_delete(cl_tls_index);
		cl_tls_index_created = false;
		cl_tls_index = 0;
	}
#endif
}
