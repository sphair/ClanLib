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
**    Harry Storbacka
**    Kenneth Gangstoe
*/

#include "Display/precomp.h"
#include "API/Display/Render/primitives_array.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Render/vertex_array_buffer.h"
#include "graphic_context_impl.h"
#include "primitives_array_impl.h"

namespace clan
{
	PrimitivesArray::PrimitivesArray()
	{
	}

	PrimitivesArray::PrimitivesArray(GraphicContext &gc)
		: impl(std::make_shared<PrimitivesArray_Impl>())
	{
		impl->provider = gc.get_provider()->alloc_primitives_array();
	}

	PrimitivesArray::~PrimitivesArray()
	{
	}

	void PrimitivesArray::throw_if_null() const
	{
		if (!impl)
			throw Exception("is null");
	}

	PrimitivesArrayProvider *PrimitivesArray::get_provider() const
	{
		return impl->provider;
	}

	void PrimitivesArray::set_attributes(int index, VertexArrayBuffer &buffer, int size, VertexAttributeDataType type, size_t offset, int stride, bool normalize)
	{
		PrimitivesArrayProvider::VertexData data(buffer.get_provider(), type, offset, size, stride);
		impl->provider->set_attribute(index, data, normalize);
	}
}
