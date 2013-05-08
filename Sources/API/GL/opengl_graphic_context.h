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
**    Mark Page
*/

/// \addtogroup clanGL_Display clanGL Display
/// \{

#pragma once

#include "api_gl.h"
#include <memory>
#include "../Display/Render/graphic_context.h"

namespace clan
{

class GraphicContext_GL_Impl;

/// \brief GL Graphic Context
class CL_API_GL GraphicContext_GL : public GraphicContext
{
//!Construction
public:
	// \brief Create a null instance
	GraphicContext_GL() {}

	/// \brief Create a GL specific graphics context
	GraphicContext_GL(GraphicContext &gc);

	~GraphicContext_GL();

//!Attributes
public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Get the opengl version
	///
	/// \param version_major = On Return: Major
	/// \param version_minor = On Return: Minor
	void get_opengl_version(int &version_major, int &version_minor);

	/// \brief Get the opengl version
	///
	/// \param version_major = On Return: Major
	/// \param version_minor = On Return: Minor
	/// \param version_release = On Return: Release
	void get_opengl_version(int &version_major, int &version_minor, int &version_release);

	/// \brief Get the opengl shading language version
	///
	/// \param version_major = On Return: Major
	/// \param version_minor = On Return: Minor
	void get_opengl_shading_language_version(int &version_major, int &version_minor);

	/// \brief Get the opengl renderer string
	std::string get_renderer_string();

	/// \brief Get the opengl vendor string
	std::string get_vendor_string();

	/// \brief Get the list of opengl extensions.
	std::vector<std::string> get_extensions();

//!Operations
public:
	/// \brief Sets the thread's OpenGL context to the one used by the graphic context
	void set_active();

//!Implementation
private:
	std::shared_ptr<GraphicContext_GL_Impl> impl;
};

}

/// \}
