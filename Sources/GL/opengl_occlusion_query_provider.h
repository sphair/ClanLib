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
**    Harry Storbacka
*/

#pragma once


#include "API/Display/TargetProviders/occlusion_query_provider.h"
#include "API/Display/Render/compare_function.h"
#include "API/GL/opengl.h"
#include "API/Core/System/disposable_object.h"

class CL_OpenGLGraphicContextProvider;

class CL_OpenGLOcclusionQueryProvider : public CL_OcclusionQueryProvider, CL_DisposableObject
{
/// \name Construction
/// \{

public:
	CL_OpenGLOcclusionQueryProvider(CL_OpenGLGraphicContextProvider *gc_provider);

	~CL_OpenGLOcclusionQueryProvider();


/// \}
/// \name Attributes
/// \{

public:
	CLint get_result() const;
	bool is_result_ready() const;


/// \}
/// \name Operations
/// \{

public:
	void begin();
	void end();
	void create();
	void destroy();


/// \}
/// \name Implementation
/// \{

private:
	void on_dispose();
	/// \brief OpenGL occlusion query handle.
	CLuint handle;

	CL_OpenGLGraphicContextProvider *gc_provider;
/// \}
};


