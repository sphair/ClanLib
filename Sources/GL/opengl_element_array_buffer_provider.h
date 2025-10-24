/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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


#include "API/Display/TargetProviders/element_array_buffer_provider.h"
#include "API/GL/opengl.h"
#include "API/Core/System/disposable_object.h"
#include "opengl_buffer_object_provider.h"

class CL_OpenGLGraphicContextProvider;

class CL_OpenGLElementArrayBufferProvider : public CL_ElementArrayBufferProvider
{
/// \name Construction
/// \{

public:
	CL_OpenGLElementArrayBufferProvider(CL_OpenGLGraphicContextProvider *gc_provider);

	~CL_OpenGLElementArrayBufferProvider();

	void create(int size, CL_BufferUsage usage);

	void create(void *data, int size, CL_BufferUsage usage);

	void destroy() { delete this; }


/// \}
/// \name Attributes
/// \{

public:
	void *get_data() { return buffer.get_data(); }

	CLuint get_handle() const { return buffer.get_handle(); }


/// \}
/// \name Operations
/// \{

public:
	void lock(CL_BufferAccess access) { buffer.lock(access); }

	void unlock() { buffer.unlock(); }

	void upload_data(int offset, void *data, int size) { buffer.upload_data(offset,data, size); }


/// \}
/// \name Implementation
/// \{

private:
	CL_OpenGLBufferObjectProvider buffer;

/// \}
};

