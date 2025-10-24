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
**    Magnus Norddahl
**    Harry Storbacka
*/

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

#pragma once

#include "../api_display.h"
#include "../../Core/System/sharedptr.h"

#include "texture.h"

class CL_RenderBuffer;
class CL_GraphicContext;
class CL_RenderBufferProvider;
class CL_RenderBuffer_Impl;

/// \brief Render-buffer object class.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_RenderBuffer
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a null instance.
	CL_RenderBuffer();

	/// \brief Constructs a RenderBuffer
	///
	/// \param context = Graphic Context
	/// \param width = value
	/// \param height = value
	/// \param internal_format = Texture Format
	CL_RenderBuffer(CL_GraphicContext &context, int width, int height, CL_TextureFormat internal_format = cl_rgba, int multisample_samples=0);

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return impl.is_null(); }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Returns the render buffer size.
	const CL_Size &get_size() const;

	/// \brief Get Provider
	///
	/// \return provider
	CL_RenderBufferProvider *get_provider() const;

/// \}
/// \name Operations
/// \{

public:

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_RenderBuffer_Impl> impl;
/// \}
};

/// \}
