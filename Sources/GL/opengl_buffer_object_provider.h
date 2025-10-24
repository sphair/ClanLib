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
**    Mark Page
*/

#pragma once


#include "API/Display/TargetProviders/vertex_array_buffer_provider.h"
#include "API/GL/opengl.h"
#include "API/Core/System/disposable_object.h"

class CL_OpenGLGraphicContextProvider;

class CL_OpenGLBufferObjectProvider : public CL_DisposableObject
{
/// \name Construction
/// \{

public:
	CL_OpenGLBufferObjectProvider(CL_OpenGLGraphicContextProvider *gc_provider);

	~CL_OpenGLBufferObjectProvider();

	void create(const void *data, int size, CL_BufferUsage usage, CLenum new_binding, CLenum new_target);

	void destroy() { delete this; }


/// \}
/// \name Attributes
/// \{

public:
	void *get_data();

	CLuint get_handle() const { return handle; }
	CLenum get_binding() const { return binding; }
	CLenum get_target() const { return target; }

/// \}
/// \name Operations
/// \{

public:
	void lock(CL_BufferAccess access);

	void unlock();

	void upload_data(int offset, const void *data, int size);

/// \}
/// \name Implementation
/// \{

private:
	void on_dispose();
	CLenum to_enum(CL_BufferUsage usage) const;

	CLenum to_enum(CL_BufferAccess access) const;

	CL_OpenGLGraphicContextProvider *gc_provider;

	CLuint handle;
	CLenum binding;
	CLenum target;

	void *data_ptr;
/// \}
};


