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
*/



#pragma once

#include "API/Display/TargetProviders/primitives_array_provider.h"

namespace clan
{
/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

class PrimitivesArray_Impl
{
public:
	PrimitivesArray_Impl() : provider(0) { }
	~PrimitivesArray_Impl() { if (provider) delete provider; }

	PrimitivesArrayProvider *provider;
};

/*
class PrimitivesArray_Impl : public PrimitivesArrayData
{
public:
	PrimitivesArray_Impl(int max_attributes, std::weak_ptr<GraphicContext_Impl> &gc)
	: attributes_map(0), max_attributes(max_attributes), gc(gc)
	{
		num_attributes = 0;
		attribute_indexes = new int[max_attributes];
		attributes = new VertexData[max_attributes];
		normalize_attributes = new bool[max_attributes];

		attributes_map = new int[max_attributes];

		clear();
	}

	~PrimitivesArray_Impl()
	{
		delete[] attribute_indexes;
		delete[] attributes;
		delete[] normalize_attributes;
		delete[] attributes_map;
	}

	void clear()
	{
		num_attributes = 0;
		for (int i = 0; i < max_attributes; i++)
			attributes_map[i] = -1;
	}

	void set_attribute(int index, const VertexData &data, bool normalize = false)
	{
		if (index >= max_attributes)
			return;
		if (attributes_map[index] == -1)
		{
			if (num_attributes >= max_attributes)
				return;
			attribute_indexes[num_attributes] = index;
			attributes_map[index] = num_attributes++;
		}
		attributes[attributes_map[index]] = data;
		normalize_attributes[attributes_map[index]] = normalize;
	}

	int *attributes_map;
	int max_attributes;
	std::weak_ptr<GraphicContext_Impl> gc;
};
*/

}

/// \}
