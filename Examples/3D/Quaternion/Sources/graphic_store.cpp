/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

#include "precomp.h"
#include "graphic_store.h"

GraphicStore::GraphicStore(GraphicContext &gc) : shader_color(gc)
{
#ifdef _DEBUG
	//struct aiLogStream stream;
	//stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT,NULL);
	//aiAttachLogStream(&stream);
	//stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE,"assimp_log.txt");
	//aiAttachLogStream(&stream);
#endif

#if defined(I_LOVE_ASSIMP_AND_PRECOMPILED_IT)
	store = aiCreatePropertyStore();
	aiSetImportPropertyFloat(store, AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE,89.53f);
#endif

}

GraphicStore::~GraphicStore()
{
#if defined(I_LOVE_ASSIMP_AND_PRECOMPILED_IT)
	if (store)
	{
		aiReleasePropertyStore(store);
		aiDetachAllLogStreams();
	}
#endif
}

void GraphicStore::LoadImages( GraphicContext &gc )
{

}
