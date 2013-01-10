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
**    Mark Page
*/

/// \addtogroup clanGL1_Display clanGL1 Display
/// \{

#pragma once

#include "api_gl1.h"
#include <memory>
#include "../Display/Render/graphic_context.h"

namespace clan
{

class RenderWindowProvider;
class ShaderObjectProvider;
class FrameBufferProvider;
class RenderBufferProvider;
class GL1FrameBufferProvider;
class GraphicContext_GL1_Impl;

/// \brief GL1 Graphic Context
///
/// \xmlonly !group=GL1/Display! !header=gl1.h! \endxmlonly
class CL_API_GL1 GraphicContext_GL1 : public GraphicContext
{
//!Construction
public:
	// \brief Create a null instance
	GraphicContext_GL1() {}

	/// \brief Create a GL1 specific graphics context
	GraphicContext_GL1(GraphicContext &gc);

	~GraphicContext_GL1();

//!Attributes
public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Get Max texture coords
	///
	/// \return max_texture_coords
	int get_max_texture_coords();

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
	/// If the FrameBuffer is active, then the OpenGL context will be the OpenGL PBuffer
	void set_active();

//!Implementation
private:
	std::shared_ptr<GraphicContext_GL1_Impl> impl;
};

}

/// \}
