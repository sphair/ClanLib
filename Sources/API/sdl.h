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
**    Mark Page
*/

/// \brief <p>ClanLib SDL target library.</p>
//! Global=SDL

#pragma once

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef __cplusplus_cli
#pragma managed(push, off)
#endif

#include "SDL/sdl_target.h"
#include "SDL/setup_sdl.h"

#ifdef __cplusplus_cli
#pragma managed(pop)
#endif

#if defined (_MSC_VER)
	#if !defined (UNICODE)
		#if defined (CL_DLL)
			#if !defined (_DEBUG)
				#if defined(_M_X64)
					#pragma comment(lib, "clanSDL-x64-dll.lib")
					#pragma comment(lib, "SDL-x64-dll.lib")
					#pragma comment(lib, "SDLmain-x64-dll.lib")
					#pragma comment(lib, "SDLgfx-x64-dll.lib")
				#else
					#pragma comment(lib, "clanSDL-dll.lib")
					#pragma comment(lib, "SDL-dll.lib")
					#pragma comment(lib, "SDLmain-dll.lib")
					#pragma comment(lib, "SDLgfx-dll.lib")
				#endif
			#else
				#if defined(_M_X64)
					#pragma comment(lib, "clanSDL-x64-dll-debug.lib")
					#pragma comment(lib, "SDL-x64-dll-debug.lib")
					#pragma comment(lib, "SDLmain-x64-dll-debug.lib")
					#pragma comment(lib, "SDLgfx-x64-dll-debug.lib")
				#else
					#pragma comment(lib, "clanSDL-dll-debug.lib")
					#pragma comment(lib, "SDL-dll-debug.lib")
					#pragma comment(lib, "SDLmain-dll-debug.lib")
					#pragma comment(lib, "SDLgfx-dll-debug.lib")
				#endif
			#endif
		#elif defined (_DLL)
			#if !defined (_DEBUG)
				#if defined(_M_X64)
					#pragma comment(lib, "clanSDL-x64-static-mtdll.lib")
					#pragma comment(lib, "SDL-x64-static-mtdll.lib")
					#pragma comment(lib, "SDLmain-x64-static-mtdll.lib")
					#pragma comment(lib, "SDLgfx-x64-static-mtdll.lib")
				#else
					#pragma comment(lib, "clanSDL-static-mtdll.lib")
					#pragma comment(lib, "SDL-static-mtdll.lib")
					#pragma comment(lib, "SDLmain-static-mtdll.lib")
					#pragma comment(lib, "SDLgfx-static-mtdll.lib")
				#endif
			#else
				#if defined(_M_X64)
					#pragma comment(lib, "clanSDL-x64-static-mtdll-debug.lib")
					#pragma comment(lib, "SDL-x64-static-mtdll-debug.lib")
					#pragma comment(lib, "SDLmain-x64-static-mtdll-debug.lib")
					#pragma comment(lib, "SDLgfx-x64-static-mtdll-debug.lib")
				#else
					#pragma comment(lib, "clanSDL-static-mtdll-debug.lib")
					#pragma comment(lib, "SDL-static-mtdll-debug.lib")
					#pragma comment(lib, "SDLmain-static-mtdll-debug.lib")
					#pragma comment(lib, "SDLgfx-static-mtdll-debug.lib")
				#endif
			#endif
			#pragma comment(lib, "winmm.lib")
		#else
			#if !defined (_DEBUG)
				#if defined(_M_X64)
					#pragma comment(lib, "clanSDL-x64-static-mt.lib")
					#pragma comment(lib, "SDL-x64-static-mt.lib")
					#pragma comment(lib, "SDLmain-x64-static-mt.lib")
					#pragma comment(lib, "SDLgfx-x64-static-mt.lib")
				#else
					#pragma comment(lib, "clanSDL-static-mt.lib")
					#pragma comment(lib, "SDL-static-mt.lib")
					#pragma comment(lib, "SDLmain-static-mt.lib")
					#pragma comment(lib, "SDLgfx-static-mt.lib")
				#endif
			#else
				#if defined(_M_X64)
					#pragma comment(lib, "clanSDL-x64-static-mt-debug.lib")
					#pragma comment(lib, "SDL-x64-static-mt-debug.lib")
					#pragma comment(lib, "SDLmain-x64-static-mt-debug.lib")
					#pragma comment(lib, "SDLgfx-x64-static-mt-debug.lib")
				#else
					#pragma comment(lib, "clanSDL-static-mt-debug.lib")
					#pragma comment(lib, "SDL-static-mt-debug.lib")
					#pragma comment(lib, "SDLmain-static-mt-debug.lib")
					#pragma comment(lib, "SDLgfx-static-mt-debug.lib")
				#endif
			#endif
			#pragma comment(lib, "winmm.lib")
		#endif
	#else
		#if defined (CL_DLL)
			#if !defined (_DEBUG)
				#if defined(_M_X64)
					#pragma comment(lib, "clanSDL-x64-dll-uc.lib")
					#pragma comment(lib, "SDL-x64-dll-uc.lib")
					#pragma comment(lib, "SDLmain-x64-dll-uc.lib")
					#pragma comment(lib, "SDLgfx-x64-dll-uc.lib")
				#else
					#pragma comment(lib, "clanSDL-dll-uc.lib")
					#pragma comment(lib, "SDL-dll-uc.lib")
					#pragma comment(lib, "SDLmain-dll-uc.lib")
					#pragma comment(lib, "SDLgfx-dll-uc.lib")
				#endif
			#else
				#if defined(_M_X64)
					#pragma comment(lib, "clanSDL-x64-dll-uc-debug.lib")
					#pragma comment(lib, "SDL-x64-dll-uc-debug.lib")
					#pragma comment(lib, "SDLmain-x64-dll-uc-debug.lib")
					#pragma comment(lib, "SDLgfx-x64-dll-uc-debug.lib")
				#else
					#pragma comment(lib, "clanSDL-dll-uc-debug.lib")
					#pragma comment(lib, "SDL-dll-uc-debug.lib")
					#pragma comment(lib, "SDLmain-dll-uc-debug.lib")
					#pragma comment(lib, "SDLgfx-dll-uc-debug.lib")
				#endif
			#endif
		#elif defined (_DLL)
			#if !defined (_DEBUG)
				#if defined(_M_X64)
					#pragma comment(lib, "clanSDL-x64-static-mtdll-uc.lib")
					#pragma comment(lib, "SDL-x64-static-mtdll-uc.lib")
					#pragma comment(lib, "SDLmain-x64-static-mtdll-uc.lib")
					#pragma comment(lib, "SDLgfx-x64-static-mtdll-uc.lib")
				#else
					#pragma comment(lib, "clanSDL-static-mtdll-uc.lib")
					#pragma comment(lib, "SDL-static-mtdll-uc.lib")
					#pragma comment(lib, "SDLmain-static-mtdll-uc.lib")
					#pragma comment(lib, "SDLgfx-static-mtdll-uc.lib")
				#endif
			#else
				#if defined(_M_X64)
					#pragma comment(lib, "clanSDL-x64-static-mtdll-uc-debug.lib")
					#pragma comment(lib, "SDL-x64-static-mtdll-uc-debug.lib")
					#pragma comment(lib, "SDLmain-x64-static-mtdll-uc-debug.lib")
					#pragma comment(lib, "SDLgfx-x64-static-mtdll-uc-debug.lib")
				#else
					#pragma comment(lib, "clanSDL-static-mtdll-uc-debug.lib")
					#pragma comment(lib, "SDL-static-mtdll-uc-debug.lib")
					#pragma comment(lib, "SDLmain-static-mtdll-uc-debug.lib")
					#pragma comment(lib, "SDLgfx-static-mtdll-uc-debug.lib")
				#endif
			#endif
			#pragma comment(lib, "winmm.lib")
		#else
			#if !defined (_DEBUG)
				#if defined(_M_X64)
					#pragma comment(lib, "clanSDL-x64-static-mt-uc.lib")
					#pragma comment(lib, "SDL-x64-static-mt-uc.lib")
					#pragma comment(lib, "SDLmain-x64-static-mt-uc.lib")
					#pragma comment(lib, "SDLgfx-x64-static-mt-uc.lib")
				#else
					#pragma comment(lib, "clanSDL-static-mt-uc.lib")
					#pragma comment(lib, "SDL-static-mt-uc.lib")
					#pragma comment(lib, "SDLmain-static-mt-uc.lib")
					#pragma comment(lib, "SDLgfx-static-mt-uc.lib")
				#endif
			#else
				#if defined(_M_X64)
					#pragma comment(lib, "clanSDL-x64-static-mt-uc-debug.lib")
					#pragma comment(lib, "SDL-x64-static-mt-uc-debug.lib")
					#pragma comment(lib, "SDLmain-x64-static-mt-uc-debug.lib")
					#pragma comment(lib, "SDLgfx-x64-static-mt-uc-debug.lib")
				#else
					#pragma comment(lib, "clanSDL-static-mt-uc-debug.lib")
					#pragma comment(lib, "SDL-static-mt-uc-debug.lib")
					#pragma comment(lib, "SDLmain-static-mt-uc-debug.lib")
					#pragma comment(lib, "SDLgfx-static-mt-uc-debug.lib")
				#endif
			#endif
			#pragma comment(lib, "winmm.lib")
		#endif
	#endif
#endif

