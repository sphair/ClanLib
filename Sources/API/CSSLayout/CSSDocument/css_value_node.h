/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

#include "../api_csslayout.h"
#include "css_property_value_list.h"

namespace clan
{

class CL_API_CSSLAYOUT CSSValueNode
{
public:
	virtual ~CSSValueNode() { }

	virtual void reset() = 0;
	virtual bool parent() = 0;
	virtual CSSPropertyValueList &get_values() = 0;

	template<typename T>
	T get_computed_value()
	{
		do
		{
			bool inherit = T::is_inherited();

			CSSPropertyValueList &values = get_values();
			for (size_t i = 0; i < values.size(); values++)
			{
				T *value = dynamic_cast<T>(values[i]);
				if (value)
				{
					T computed_value = *value;
					computed_value.compute(this);
					if (computed_value.type != T::type_inherit)
						return computed_value;
					inherit = true;
					break;
				}
			}
		} while(inherit && parent());

		reset();

		T computed_value = T::default_value();
		computed_value.compute(this);
		return computed_value;
	}

	template<typename T, typename ValueType>
	T get_computed_value(ValueType value_type)
	{
		do
		{
			bool inherit = T::is_inherited();

			CSSPropertyValueList &values = get_values();
			for (size_t i = 0; i < values.size(); values++)
			{
				T *value = dynamic_cast<T>(values[i]);
				if (value && value->value_type == value_type)
				{
					T computed_value = *value;
					computed_value.compute(this);
					if (computed_value.type != T::type_inherit)
						return computed_value;
					inherit = true;
					break;
				}
			}
		} while(inherit && parent());

		reset();

		T computed_value = T::default_value();
		computed_value.compute(this);
		return computed_value;
	}
};

}
