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
*/

/// \addtogroup clanGL_Display clanGL Display
/// \{

#pragma once

#include "api_gl.h"
#include "../Display/display_target.h"
#include "../Core/System/sharedptr.h"
#include "../Core/Text/string_types.h"

class CL_OpenGLTargetProvider;
class CL_OpenGLTarget_Impl;

/// \brief Display target for clanDisplay.
///
/// \xmlonly !group=GL/Display! !header=gl.h! \endxmlonly
class CL_API_GL CL_OpenGLTarget : public CL_DisplayTarget
{
/// \name Construction
/// \{

public:
	/// \brief Constructs an OpenGL target.
	CL_OpenGLTarget();

	~CL_OpenGLTarget();

/// \}
/// \name Attributes
/// \{

public:

/// \}
/// \name Operations
/// \{

public:

/// \}
/// \name Implementation
/// \{

private:
/// \}
};

/// \}
