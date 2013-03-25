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

#include "API/Display/Render/graphic_context.h"

namespace clan
{

class PBuffer_GL1_Impl;

#ifdef WIN32
class GL1WindowProvider_WGL;
#else
class GL1WindowProvider_GLX;
#endif

class GL1GraphicContextProvider;

class PBuffer_GL1
{
/// \name Construction
/// \{

public:
	// Construct a null instance
	PBuffer_GL1();

	PBuffer_GL1(GL1GraphicContextProvider *gc_provider);
	~PBuffer_GL1();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;


/// \}
/// \name Operations
/// \{

public:
#ifdef WIN32
	void create(GL1WindowProvider_WGL &window_provider, Size &size);
#else
	void create(GL1WindowProvider_GLX &window_provider, Size &size);
#endif
	void set_active() const;

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<PBuffer_GL1_Impl> impl;

/// \}
};


}
