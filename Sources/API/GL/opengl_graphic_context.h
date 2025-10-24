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
**    Mark Page
*/

/// \addtogroup clanGL_Display clanGL Display
/// \{

#pragma once

#include "api_gl.h"
#include "../Core/System/sharedptr.h"
#include "../Display/Render/graphic_context.h"

class CL_GraphicContext_GL_Impl;

/// \brief GL Graphic Context
///
/// \xmlonly !group=GL/Display! !header=gl.h! \endxmlonly
class CL_API_GL CL_GraphicContext_GL : public CL_GraphicContext
{
//!Construction
public:
	/// \brief 
	CL_GraphicContext_GL(CL_GraphicContext &gc);

	~CL_GraphicContext_GL();

//!Attributes
public:

	/// \brief Get the opengl version
	///
	/// \param version_major = On Return: Major
	/// \param version_minor = On Return: Minor
	/// \param version_release = On Return: Release
	void get_opengl_version(int &version_major, int &version_minor, int &version_release);

//!Operations
public:
	/// \brief Sets the thread's OpenGL context to the one used by the graphic context
	///
	/// This is the same as calling CL_OpenGL::set_active(gc);
	void set_active();

//!Implementation
private:
	CL_SharedPtr<CL_GraphicContext_GL_Impl> impl;
};

/// \}
