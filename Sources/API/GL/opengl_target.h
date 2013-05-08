/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

/// \addtogroup clanGL_Display clanGL Display
/// \{

#pragma once

#include "api_gl.h"
#include "../Display/display_target.h"
#include <memory>

namespace clan
{

class OpenGLTargetProvider;
class OpenGLTarget_Impl;
class OpenGLWindowDescription;

/// \brief Display target for clanDisplay.
class CL_API_GL OpenGLTarget : public DisplayTarget
{
/// \name Construction
/// \{

public:
	/// \brief Constructs an OpenGL target.
	OpenGLTarget();

	~OpenGLTarget();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns true if this display target is the current target
	///
	/// This may change after a display window has been created
	static bool is_current();

	static OpenGLWindowDescription get_description();

/// \}
/// \name Operations
/// \{

public:
	/// \brief Set this display target to be the current target
	static void set_current();

	static void set_description(OpenGLWindowDescription &desc);

/// \}
/// \name Implementation
/// \{

private:
/// \}
};

}

/// \}
