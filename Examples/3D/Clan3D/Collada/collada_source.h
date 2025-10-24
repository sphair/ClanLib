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

class CL_Collada_Source_Impl;

class CL_Collada_Source
{
public:
	CL_Collada_Source();

	CL_Collada_Source(CL_DomElement &source_element);

	enum ArrayType
	{
		cl_unknown_array,
		cl_vec2f_array,
		cl_vec3f_array,
		cl_vec2i_array,
		cl_vec3i_array,
		cl_float_array,
		cl_int_array
	};

	CL_String &get_name();
	CL_String &get_id();

	bool is_null();

	ArrayType get_array_type();
	std::vector<float> &get_float_array();
	std::vector<int> &get_int_array();
	std::vector<CL_Vec2f> &get_vec2f_array();
	std::vector<CL_Vec3f> &get_vec3f_array();
	std::vector<CL_Vec2i> &get_vec2i_array();
	std::vector<CL_Vec3i> &get_vec3i_array();

	/// \brief Equality operator
	bool operator==(const CL_Collada_Source &other) const
	{
		return impl==other.impl;
	}

	/// \brief Inequality operator
	bool operator!=(const CL_Collada_Source &other) const
	{
		return impl!=other.impl;
	}
private:
	CL_SharedPtr<CL_Collada_Source_Impl> impl;

};


