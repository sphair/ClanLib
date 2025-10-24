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

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

#ifndef __header_primitives_array_builder__
#define __header_primitives_array_builder__

#include "vertex_array_buffer.h"

class CL_PrimitivesArray;
class CL_PrimitivesArrayBuilder_Impl;

/// \brief Primitives array builder class.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_PrimitivesArrayBuilder
{
public:
	CL_PrimitivesArrayBuilder();

	/// \brief Append
	///
	/// \param num_vertices = value
	/// \param prim_array = Primitives Array
	void append(int num_vertices, const CL_PrimitivesArray &prim_array);

	/// \brief Draw
	///
	/// \param gc = Graphic Context
	/// \param type = Primitives Type
	void draw(CL_GraphicContext &gc, CL_PrimitivesType type);

	/// \brief Create buffer
	///
	/// \param gc = Graphic Context
	/// \param usage = Buffer Usage
	///
	/// \return Vertex Array Buffer
	CL_VertexArrayBuffer create_buffer(CL_GraphicContext &gc, CL_BufferUsage usage = cl_usage_static_draw);

private:
	CL_SharedPtr<CL_PrimitivesArrayBuilder_Impl> impl;
};

#endif
/// \}
