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
**    Mark Page
*/

#pragma once

#include "../pbuffer.h"
#include "API/Core/System/disposable_object.h"
#include "API/GL1/opengl1_wrap.h"
#include "API/Core/System/disposable_object.h"
class CL_GL1GraphicContextProvider;

class CL_PBuffer_GL1_Impl : public CL_DisposableObject
{
/// \name Construction
/// \{

public:
	CL_PBuffer_GL1_Impl(CL_GL1GraphicContextProvider *gc_provider);

	~CL_PBuffer_GL1_Impl();


/// \}
/// \name Attributes
/// \{

public:


/// \}
/// \name Operations
/// \{

public:
	void create(CL_GL1WindowProvider_WGL &window_provider, CL_Size &size);
	void set_active() const;

/// \}
/// \name Implementation
/// \{

private:
	void on_dispose();
	void reset();

	CL_GL1GraphicContextProvider *gc_provider;
	CL_GL1Functions::HPBUFFERARB pbuffer;
	HGLRC pbuffer_context;
	HDC pbuffer_dc;

	CL_GL1GraphicContextProvider *pbuffer_gc_provider;
	CL_GraphicContext pbuffer_gc;

/// \}
};
