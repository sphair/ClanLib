/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

/// \brief <p>ClanLib Scene library.</p>
//! Global=Scene

#pragma once

#ifdef __cplusplus_cli
#pragma managed(push, off)
#endif

#include "Scene3D/scene.h"
#include "Scene3D/scene_camera.h"
#include "Scene3D/scene_light.h"
#include "Scene3D/scene_model.h"
#include "Scene3D/scene_object.h"
#include "Scene3D/scene_particle_emitter.h"
#include "Scene3D/scene_cache.h"
#include "Scene3D/scene_cache_provider.h"
#include "Scene3D/scene_pass.h"
#include "Scene3D/scene_inout_data.h"
#include "Scene3D/cached_texture.h"
#include "Scene3D/ModelData/model_data.h"
#include "Scene3D/LevelData/level_data.h"
#include "Scene3D/Performance/gpu_timer.h"
#include "Scene3D/Performance/scope_timer.h"

#ifdef __cplusplus_cli
#pragma managed(pop)
#endif

#if defined(_MSC_VER)
	#if !defined(_MT)
		#error Your application is set to link with the single-threaded version of the run-time library. Go to project settings, in the C++ section, and change it to multi-threaded.
	#endif
	#if !defined(_DEBUG)
		#if defined(DLL)
			#pragma comment(lib, "clanScene3D-dll.lib")
		#elif defined(_DLL)
			#pragma comment(lib, "clanScene3D-static-mtdll.lib")
		#else
			#pragma comment(lib, "clanScene3D-static-mt.lib")
		#endif
	#else
		#if defined(DLL)
			#pragma comment(lib, "clanScene3D-dll-debug.lib")
		#elif defined(_DLL)
			#pragma comment(lib, "clanScene3D-static-mtdll-debug.lib")
		#else
			#pragma comment(lib, "clanScene3D-static-mt-debug.lib")
		#endif
	#endif
#endif
