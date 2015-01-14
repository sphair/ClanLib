/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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


#pragma once

#include "../../Core/Math/mat4.h"
#include "graphic_context.h"

namespace clan
{
/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

class GraphicContext;
class Canvas;
enum TextureImageYAxis;

/// \brief Render batching abstraction
class RenderBatcher
{
/// \name Operations
/// \{
public:
	virtual ~RenderBatcher() { }

	/// \brief Flush
	///
	/// \param gc = Graphic Context
	virtual void flush(GraphicContext &gc) = 0;

	/// \brief Matrix changed
	///
	/// \param modelview = Mat4f
	/// \param projection = Mat4f
	/// \param image_yaxis = The image Y axis, to use where "projection" is not used
	virtual void matrix_changed(const Mat4f &modelview, const Mat4f &projection, TextureImageYAxis image_yaxis, float dpi) = 0;
/// \}
};

}

/// \}
