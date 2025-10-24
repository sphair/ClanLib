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
**    Kevin J Bluck
*/

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

#pragma once
#define  header_render_window_provider

#include "../api_display.h"

/// \brief Provides generic render window services for clanDisplay.
/** <p>This type encapsulates a platform-specific rendering window. It is designed to encapsulate
    and hide platform dependencies from code that interacts with a rendering context or drawable.
    It is particularly useful for integrating third-party windowing libraries such as Fltk or
    wxWidgets with Clanlib drawing services.</p>
    !group=Display/Display Target Interface! !header=display.h!*/
class CL_API_DISPLAY CL_RenderWindowProvider
{
/// \name Construction
/// \{

public:
	virtual ~CL_RenderWindowProvider()  { return; }

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Get viewport width of rendering window.
	virtual int get_viewport_width() const = 0;

	/// \brief Get viewport height of rendering window.
	virtual int get_viewport_height() const = 0;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Creates a new worker context for the render window.
	/** <p>This creates a new render window provider which refers to the same render window as the
	    spawning provider, but with a new rendering context. The returned pointer refers to a newly
	    allocated instance, and it is the caller's responsibility to destroy it.</p>*/
	virtual const CL_RenderWindowProvider * new_worker_context() const = 0;

	/// \brief Flip the render window's frame buffers.
	virtual void flip_buffers(int interval=-1) const = 0;

	/// \brief Sets this to the active render context.
	/** <p>More precisely, makes current the render context associated with the render window represented by
	    this object to be the active context. Different render windows might actually share contexts.</p> */
	virtual void make_current() const = 0;

/// \}
/// \name Implementation
/// \{

private:

/// \}
};

/// \}

