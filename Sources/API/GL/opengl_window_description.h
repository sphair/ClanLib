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
#include "../Core/System/sharedptr.h"
#include "../Display/Window/display_window_description.h"

class CL_OpenGLWindowDescription_Impl;

/// \brief OpenGL specific display window description.
///
///   <p>Use this class to setup OpenGL specific settings for a top level
///    window:</p>
///    <pre>
///    CL_OpenGLWindowDescription desc;
///    dest.set_title("OpenGL window with stereo visual");
///    desc.set_stereo(true);
///    desc.set_stencil_size(4);
///    CL_OpenGLWindow window(desc);
///    </pre> 
/// \xmlonly !group=GL/Display! !header=gl.h! \endxmlonly
class CL_API_GL CL_OpenGLWindowDescription : public CL_DisplayWindowDescription
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a window description with default values.
	CL_OpenGLWindowDescription();

	/// \brief Constructs a OpenGLWindowDescription
	///
	/// \param desc = Display Window Description
	CL_OpenGLWindowDescription(const CL_DisplayWindowDescription &desc);
	CL_OpenGLWindowDescription &operator=(CL_DisplayWindowDescription &desc);

	virtual ~CL_OpenGLWindowDescription();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns true if only double-buffered visuals are considered.
	/** <p>If not enabled, only single-buffered visuals are considered.</p>*/
	bool get_doublebuffer() const;

	/// \brief Returns true if only stereo visuals are considered.
	/** <p>If not enabled, only monoscopic visuals are considered.</p>*/
	bool get_stereo() const;

	/// \brief Returns the desired color buffer size.
	/** <p>Ignored in GLX (X11)</p>
	    <p>In WGL (Windows) it specifies the size of the color buffer, excluding the alpha bitplanes.</p>*/
	int get_buffer_size() const;

	/// \brief Returns the minimum required red buffer.
	int get_red_size() const;

	/// \brief Returns the minimum required green buffer.
	int get_green_size() const;

	/// \brief Returns the minimum required blue buffer.
	int get_blue_size() const;

	/// \brief Returns the minimum required alpha buffer.
	int get_alpha_size() const;

	/// \brief Returns the number of samples per pixel.
	int get_multisampling() const;

	/// \brief Returns selected major version number
	int get_version_major() const;

	/// \brief Returns selected minor version number
	int get_version_minor() const;

	/// \brief Returns true if allowed lower versions than the selected version
	bool get_allow_lower_versions() const;

	/// \brief Returns true if the debug flag is set
	bool get_debug() const;

	/// \brief Returns true if the forward compatible flag is set
	bool get_forward_compatible() const;

	/// \brief Returns true if the core profile flag is set
	bool get_core_profile() const;

	/// \brief Returns true if the compatibility profile flag is set
	bool get_compatibility_profile() const;

	/// \brief Returns the layer plane value
	int get_layer_plane() const;

/// \}
/// \name Operations
/// \{
public:
	/// \brief Sets if only double-buffered visuals are considered.
	/** <p>If not enabled, only single-buffered visuals are considered.</p>*/
	void set_doublebuffer(bool value);

	/// \brief Sets if only stereo visuals are to be considered.
	/** <p>If not enabled, only monoscopic visuals are considered.</p>*/
	void set_stereo(bool value);

	/// \brief Sets the desired color buffer size.
	/** <p>Ignored in GLX (X11)</p>
	    <p>In WGL (Windows) it specifies the size of the color buffer, excluding the alpha bitplanes.</p>*/
	void set_buffer_size(int value);

	/// \brief Sets the minimum required red buffer.
	/** <p>If this value is zero, the smallest available red buffer is preferred. Otherwise,
	    the largest available red buffer of at least the minimum size is preferred.</p>*/
	void set_red_size(int value);

	/// \brief Sets the minimum required green buffer.
	/** <p>If this value is zero, the smallest available green buffer is preferred. Otherwise,
	    the largest available green buffer of at least the minimum size is preferred.</p>*/
	void set_green_size(int value);

	/// \brief Sets the minimum required blue buffer.
	/** <p>If this value is zero, the smallest available blue buffer is preferred. Otherwise,
	    the largest available blue buffer of at least the minimum size is preferred.</p>*/
	void set_blue_size(int value);

	/// \brief Sets the minimum required alpha buffer.
	/** <p>If this value is zero, the smallest available alpha buffer is preferred. Otherwise,
	    the largest available alpha buffer of at least the minimum size is preferred.</p>*/
	void set_alpha_size(int value);

	/// \brief Sets the number of samples per pixel.
	void set_multisampling(int value);

	/// \brief Select the OpenGL version number
	///
	/// Defaults to OpenGL 3.0, with allow lower versions
	///
	/// \param major = OpenGL major number (e.g. 4)
	/// \param minor = OpenGL minor number (e.g. 1)
	/// \param allow_lower_versions = Allow lower versions of the specified opengl
	void set_version(int major, int minor, bool allow_lower_versions);

	/// \brief Sets the debug flag
	///
	/// Debug contexts are intended for use during application development, and
	/// provide additional runtime checking, validation, and logging
	/// functionality while possibly incurring performance penalties
	///
	/// \param enable = true - Enable this option (Default is false)
	void set_debug(bool enable);

	/// \brief Sets the forward compatible flag
	///
	/// Forward-compatible contexts must not support functionality marked
	/// as deprecated by that version of the API, while a
	/// non-forward-compatible context must support all functionality in
	/// that version, deprecated or not.
	///
	/// \param enable = true - Enable this option (Default is false)
	void set_forward_compatible(bool enable);

	/// \brief Sets the core profile flag
	///
	/// If true, then a context implementing the core profile of OpenGL is returned
	///
	/// \param enable = true - Enable this option (Default is true)
	void set_core_profile(bool enable);

	/// \brief Sets the compatibility profile flag
	///
	/// If than a context implementing the compatibility profile is returned.
	///
	/// \param enable = true - Enable this option (Default is false)
	void set_compatibility_profile(bool enable);

	/// \brief Sets the layer plane value
	///
	/// Specifies the layer plane that the rendering context is bound to
	///
	/// \param value = The layer plane. (Default is 0 - the main plane)
	void set_layer_plane(int value);

/// \}
/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_OpenGLWindowDescription_Impl> impl_gl;
/// \}
};

/// \}
