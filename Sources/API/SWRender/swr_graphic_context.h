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

/// \addtogroup clanSWRender_Display clanSWRender Display
/// \{

#pragma once

#include "api_swrender.h"
#include "../Core/System/sharedptr.h"
#include "../Core/System/uniqueptr.h"
#include "../Display/Render/graphic_context.h"

class CL_PixelCommand;
class CL_PixelPipeline;
class CL_GraphicContext_SWRender_Impl;

/// \brief SWRender Graphic Context
///
/// \xmlonly !group=SWRender/Display! !header=swrender.h! \endxmlonly
class CL_API_SWRender CL_GraphicContext_SWRender : public CL_GraphicContext
{
//!Construction
public:
	// \brief Create a null instance
	CL_GraphicContext_SWRender() {}

	/// \brief Create a SWRender specific graphics context
	CL_GraphicContext_SWRender(CL_GraphicContext &gc);

	~CL_GraphicContext_SWRender();

//!Attributes
public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Returns the pixel pipeline class needed to allocated CL_PixelCommand objects.
	CL_PixelPipeline *get_pipeline() const;

//!Operations
public:
	void draw_pixels_bicubic(int x, int y, int zoom_number, int zoom_denominator, const CL_PixelBuffer &pixels);

	/// \brief Queues a pixel command in the pipeline
	template<typename T>
	void queue_command(T *command) { queue_command(std::unique_ptr<T>(command)); }
	void queue_command(CL_UniquePtr<CL_PixelCommand> &command);

//!Implementation
private:
	CL_SharedPtr<CL_GraphicContext_SWRender_Impl> impl;
};

/// \}
