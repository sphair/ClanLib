/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
**    Kenneth Gangstoe
*/

/// \brief <p>ClanLib GDI target library.</p>
//! Global=GDI

#pragma once


#if _MSC_VER > 1000
#pragma once
#endif

#ifdef __cplusplus_cli
#pragma managed(push, off)
#endif

#include "GDI/gdi_target.h"
#include "GDI/setup_gdi.h"

#ifdef __cplusplus_cli
#pragma managed(pop)
#endif

#if defined (_MSC_VER)
	#if !defined (UNICODE)
		#if defined (CL_DLL)
			#if !defined (_DEBUG)
				#if defined(_M_X64)
					#pragma comment(lib, "clanGDI-x64-dll.lib")
				#else
					#pragma comment(lib, "clanGDI-dll.lib")
				#endif
			#else
				#if defined(_M_X64)
					#pragma comment(lib, "clanGDI-x64-dll-debug.lib")
				#else
					#pragma comment(lib, "clanGDI-dll-debug.lib")
				#endif
			#endif
		#elif defined (_DLL)
			#if !defined (_DEBUG)
				#if defined(_M_X64)
					#pragma comment(lib, "clanGDI-x64-static-mtdll.lib")
				#else
					#pragma comment(lib, "clanGDI-static-mtdll.lib")
				#endif
			#else
				#if defined(_M_X64)
					#pragma comment(lib, "clanGDI-x64-static-mtdll-debug.lib")
				#else
					#pragma comment(lib, "clanGDI-static-mtdll-debug.lib")
				#endif
			#endif
			#pragma comment(lib, "winmm.lib")
		#else
			#if !defined (_DEBUG)
				#if defined(_M_X64)
					#pragma comment(lib, "clanGDI-x64-static-mt.lib")
				#else
					#pragma comment(lib, "clanGDI-static-mt.lib")
				#endif
			#else
				#if defined(_M_X64)
					#pragma comment(lib, "clanGDI-x64-static-mt-debug.lib")
				#else
					#pragma comment(lib, "clanGDI-static-mt-debug.lib")
				#endif
			#endif
			#pragma comment(lib, "winmm.lib")
		#endif
	#else
		#if defined (CL_DLL)
			#if !defined (_DEBUG)
				#if defined(_M_X64)
					#pragma comment(lib, "clanGDI-x64-dll-uc.lib")
				#else
					#pragma comment(lib, "clanGDI-dll-uc.lib")
				#endif
			#else
				#if defined(_M_X64)
					#pragma comment(lib, "clanGDI-x64-dll-uc-debug.lib")
				#else
					#pragma comment(lib, "clanGDI-dll-uc-debug.lib")
				#endif
			#endif
		#elif defined (_DLL)
			#if !defined (_DEBUG)
				#if defined(_M_X64)
					#pragma comment(lib, "clanGDI-x64-static-mtdll-uc.lib")
				#else
					#pragma comment(lib, "clanGDI-static-mtdll-uc.lib")
				#endif
			#else
				#if defined(_M_X64)
					#pragma comment(lib, "clanGDI-x64-static-mtdll-uc-debug.lib")
				#else
					#pragma comment(lib, "clanGDI-static-mtdll-uc-debug.lib")
				#endif
			#endif
			#pragma comment(lib, "winmm.lib")
		#else
			#if !defined (_DEBUG)
				#if defined(_M_X64)
					#pragma comment(lib, "clanGDI-x64-static-mt-uc.lib")
				#else
					#pragma comment(lib, "clanGDI-static-mt-uc.lib")
				#endif
			#else
				#if defined(_M_X64)
					#pragma comment(lib, "clanGDI-x64-static-mt-uc-debug.lib")
				#else
					#pragma comment(lib, "clanGDI-static-mt-uc-debug.lib")
				#endif
			#endif
			#pragma comment(lib, "winmm.lib")
		#endif
	#endif
#endif


