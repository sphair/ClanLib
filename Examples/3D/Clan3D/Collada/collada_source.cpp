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
**    Mark Page
*/

#include "precomp.h"
#include "collada_source.h"

class CL_Collada_Source_Impl
{
public:
	CL_Collada_Source_Impl() {}

	void load_source(CL_DomElement &source_element);

	CL_String id;
	CL_String name;

	CL_Collada_Source::ArrayType array_type;

	std::vector<float> float_array;
	std::vector<int> int_array;
	std::vector<CL_Vec2f> cl_vec2f_array;
	std::vector<CL_Vec3f> cl_vec3f_array;
	std::vector<CL_Vec2i> cl_vec2i_array;
	std::vector<CL_Vec3i> cl_vec3i_array;

private:

	void load_float_array(CL_DomElement &float_array_element, CL_DomElement &source_element);
	void get_float_values(CL_DomElement &float_array_element, std::vector<float> &temp_float_array);
	void enter_floats(const std::vector<float> &temp_float_array);
	void enter_vec2f(const std::vector<float> &temp_float_array);
	void enter_vec3f(const std::vector<float> &temp_float_array);

	void load_int_array(CL_DomElement &int_array_element, CL_DomElement &source_element);
	void get_int_values(CL_DomElement &int_array_element, std::vector<int> &temp_int_array);
	void enter_ints(const std::vector<int> &temp_int_array);
	void enter_vec2i(const std::vector<int> &temp_int_array);
	void enter_vec3i(const std::vector<int> &temp_int_array);
	CL_DomElement find_accessor(CL_DomElement &source_element, CL_DomString &id_text);

};

CL_Collada_Source::CL_Collada_Source()
{
}

CL_Collada_Source::CL_Collada_Source(CL_DomElement &source_element) : impl(new CL_Collada_Source_Impl())
{
	impl->load_source(source_element);
}

CL_DomElement CL_Collada_Source_Impl::find_accessor(CL_DomElement &source_element, CL_DomString &id_text)
{
	CL_DomElement technique_common_element = source_element.named_item("technique_common").to_element();
	if (technique_common_element.is_null())
	{
		return CL_DomElement();
	}

	// Find the correct technique for this element
	CL_DomNode cur_child(technique_common_element.get_first_child());
	while (!cur_child.is_null())
	{
		if(cur_child.get_node_name() == "accessor")
		{
			CL_DomElement accessor_element = cur_child.to_element();
			CL_String source_text = accessor_element.get_attribute("source");
			source_text = source_text.substr(1);	// Remove the initial '#' symbol
			if (source_text == id_text)
			{
				// Found technique
				return accessor_element;
			}
		}
		cur_child = cur_child.get_next_sibling();
	}

	return CL_DomElement();	
}

void CL_Collada_Source_Impl::load_source(CL_DomElement &source_element)
{
	name = source_element.get_attribute("name");
	id = source_element.get_attribute("id");

	array_type = CL_Collada_Source::cl_unknown_array;

	CL_DomElement float_array_element = source_element.named_item("float_array").to_element();
	if (!float_array_element.is_null())
	{
		load_float_array(float_array_element, source_element);
	}

	CL_DomElement int_array_element = source_element.named_item("int_array").to_element();
	if (!int_array_element.is_null())
	{
		load_int_array(int_array_element, source_element);
	}

}

void CL_Collada_Source_Impl::load_float_array(CL_DomElement &float_array_element, CL_DomElement &source_element)
{
	if (array_type != CL_Collada_Source::cl_unknown_array)
		throw CL_Exception("file error, array already set");

	CL_DomString attribute_id =  float_array_element.get_attribute("id");
	CL_DomElement accessor_element = find_accessor(source_element, attribute_id);

	int count = float_array_element.get_attribute_int("count");

	unsigned int stride = 1;
	if (!accessor_element.is_null())
	{
		unsigned int offset = accessor_element.get_attribute_int("offset");
		if (offset)
			throw CL_Exception("source offset is not supported");

		stride = accessor_element.get_attribute_int("stride", 1);
		if ( (stride == 0) || (stride > 3) )
		{
			stride = 1;		// Default to float if stride is unknown
		}

		unsigned int technique_count = accessor_element.get_attribute_int("count");
		if (count != (technique_count * stride))
			throw CL_Exception("unsupported count mismatch");

		// TODO: Check <param>'s - and swizzle if required

	}

	// Setup
	switch (stride)
	{
		case 1:
			array_type = CL_Collada_Source::cl_float_array;
			float_array.resize(count);
			break;
		case 2:
			array_type = CL_Collada_Source::cl_vec2f_array;
			cl_vec2f_array.resize(count/2);
			break;
		case 3:
			array_type = CL_Collada_Source::cl_vec3f_array;
			cl_vec3f_array.resize(count/3);
			break;
	}

	// Get values
	std::vector<float> temp_float_array;
	temp_float_array.resize(count);
	get_float_values(float_array_element, temp_float_array);

	switch (stride)
	{
		case 1:
			enter_floats(temp_float_array);
			break;
		case 2:
			enter_vec2f(temp_float_array);
			break;
		case 3:
			enter_vec3f(temp_float_array);
			break;
	}
}
void CL_Collada_Source_Impl::enter_floats(const std::vector<float> &temp_float_array)
{
	array_type = CL_Collada_Source::cl_float_array;
	int size = temp_float_array.size();
	for (int cnt=0; cnt<size; cnt++)
	{
		float_array[cnt] = temp_float_array[cnt];
	}
}

void CL_Collada_Source_Impl::enter_vec2f(const std::vector<float> &temp_float_array)
{
	array_type = CL_Collada_Source::cl_vec2f_array;
	int size = temp_float_array.size() / 2;
	int src_offset = 0;
	for (int cnt=0; cnt<size; cnt++)
	{
		float xpos = temp_float_array[src_offset++];
		float ypos = temp_float_array[src_offset++];
		cl_vec2f_array[cnt] = CL_Vec2f(xpos, ypos);
	}
}

void CL_Collada_Source_Impl::enter_vec3f(const std::vector<float> &temp_float_array)
{
	array_type = CL_Collada_Source::cl_vec3f_array;
	int size = temp_float_array.size() / 3;
	int src_offset = 0;
	for (int cnt=0; cnt<size; cnt++)
	{
		float xpos = temp_float_array[src_offset++];
		float ypos = temp_float_array[src_offset++];
		float zpos = temp_float_array[src_offset++];
		cl_vec3f_array[cnt] = CL_Vec3f(xpos, ypos, zpos);
	}
}

void CL_Collada_Source_Impl::get_float_values(CL_DomElement &float_array_element, std::vector<float> &temp_float_array)
{
	CL_String points = float_array_element.get_text();
	const CL_String::char_type *point_text = points.c_str();

	int count = temp_float_array.size();
	for (int cnt=0; cnt < count; cnt++)
	{
		if (!(*point_text))
			throw CL_Exception("Data count mismatch");

		float value = (float) atof(point_text);
		temp_float_array[cnt] = value;

		while(*point_text)
		{
			if (*(point_text++) <= ' ')	// Find whitespace
				break;
		}

		while(*point_text)
		{
			if ((*point_text) > ' ')	// Find end of whitespace
				break;
			point_text++;
		}

	}
	if (*point_text)
			throw CL_Exception("Data count mismatch (end)");
}


void CL_Collada_Source_Impl::load_int_array(CL_DomElement &int_array_element, CL_DomElement &source_element)
{
	if (array_type != CL_Collada_Source::cl_unknown_array)
		throw CL_Exception("file error, array already set");

	CL_DomString attribute_id =  int_array_element.get_attribute("id");
	CL_DomElement accessor_element = find_accessor(source_element, attribute_id);

	int count = int_array_element.get_attribute_int("count");

	unsigned int stride = 1;
	if (!accessor_element.is_null())
	{
		unsigned int offset = accessor_element.get_attribute_int("offset");
		if (offset)
			throw CL_Exception("source offset is not supported");

		stride = accessor_element.get_attribute_int("stride", 1);
		if ( (stride == 0) || (stride > 3) )
		{
			stride = 1;		// Default to int if stride is unknown
		}

		unsigned int technique_count = accessor_element.get_attribute_int("count");
		if (count != (technique_count*stride))
			throw CL_Exception("unsupported count mismatch");

		// TODO: Check <param>'s - and swizzle if required

	}

	// Setup
	switch (stride)
	{
		case 1:
			array_type = CL_Collada_Source::cl_int_array;
			int_array.resize(count);
			break;
		case 2:
			array_type = CL_Collada_Source::cl_vec2i_array;
			cl_vec2i_array.resize(count/2);
			break;
		case 3:
			array_type = CL_Collada_Source::cl_vec3i_array;
			cl_vec3i_array.resize(count/3);
			break;
	}

	// Get values
	std::vector<int> temp_int_array;
	temp_int_array.resize(count);
	get_int_values(int_array_element, temp_int_array);

	switch (stride)
	{
		case 1:
			enter_ints(temp_int_array);
			break;
		case 2:
			enter_vec2i(temp_int_array);
			break;
		case 3:
			enter_vec3i(temp_int_array);
			break;
	}
}
void CL_Collada_Source_Impl::enter_ints(const std::vector<int> &temp_int_array)
{
	array_type = CL_Collada_Source::cl_int_array;
	int size = temp_int_array.size();
	for (int cnt=0; cnt<size; cnt++)
	{
		int_array[cnt] = temp_int_array[cnt];
	}
}

void CL_Collada_Source_Impl::enter_vec2i(const std::vector<int> &temp_int_array)
{
	array_type = CL_Collada_Source::cl_vec2i_array;
	int size = temp_int_array.size() / 2;
	int src_offset = 0;
	for (int cnt=0; cnt<size; cnt++)
	{
		int xpos = temp_int_array[src_offset++];
		int ypos = temp_int_array[src_offset++];
		cl_vec2i_array[cnt] = CL_Vec2i(xpos, ypos);
	}
}

void CL_Collada_Source_Impl::enter_vec3i(const std::vector<int> &temp_int_array)
{
	array_type = CL_Collada_Source::cl_vec3i_array;
	int size = temp_int_array.size() / 3;
	int src_offset = 0;
	for (int cnt=0; cnt<size; cnt++)
	{
		int xpos = temp_int_array[src_offset++];
		int ypos = temp_int_array[src_offset++];
		int zpos = temp_int_array[src_offset++];

		cl_vec3i_array[cnt] = CL_Vec3i(xpos, ypos, zpos);
	}
}

void CL_Collada_Source_Impl::get_int_values(CL_DomElement &int_array_element, std::vector<int> &temp_int_array)
{
	CL_String points = int_array_element.get_text();
	const CL_String::char_type *point_text = points.c_str();

	int count = temp_int_array.size();
	for (int cnt=0; cnt < count; cnt++)
	{
		if (!(*point_text))
			throw CL_Exception("Data count mismatch");

		int value = (int) atof(point_text);
		temp_int_array[cnt] = value;

		while(*point_text)
		{
			if (*(point_text++) <= ' ')	// Find whitespace
				break;
		}

		while(*point_text)
		{
			if ((*point_text) > ' ')	// Find end of whitespace
				break;
			point_text++;
		}

	}
	if (*point_text)
			throw CL_Exception("Data count mismatch (end)");
}


CL_String &CL_Collada_Source::get_name()
{
	return impl->name;
}

CL_String &CL_Collada_Source::get_id()
{
	return impl->id;
}

CL_Collada_Source::ArrayType CL_Collada_Source::get_array_type()
{
	return impl->array_type;
}

std::vector<float> &CL_Collada_Source::get_float_array()
{
	if (impl->array_type != cl_float_array)
		throw CL_Exception("Array type is not float");
	return impl->float_array;
}

std::vector<int> &CL_Collada_Source::get_int_array()
{
	if (impl->array_type != cl_int_array)
		throw CL_Exception("Array type is not integer");
	return impl->int_array;
}

std::vector<CL_Vec2f> &CL_Collada_Source::get_vec2f_array()
{
	if (impl->array_type != cl_vec2f_array)
		throw CL_Exception("Array type is not CL_Vec2f");
	return impl->cl_vec2f_array;
}

std::vector<CL_Vec3f> &CL_Collada_Source::get_vec3f_array()
{
	if (impl->array_type != cl_vec3f_array)
		throw CL_Exception("Array type is not CL_Vec3f");
	return impl->cl_vec3f_array;
}

std::vector<CL_Vec2i> &CL_Collada_Source::get_vec2i_array()
{
	if (impl->array_type != cl_vec2i_array)
		throw CL_Exception("Array type is not CL_Vec2i");
	return impl->cl_vec2i_array;
}

std::vector<CL_Vec3i> &CL_Collada_Source::get_vec3i_array()
{
	if (impl->array_type != cl_vec3i_array)
		throw CL_Exception("Array type is not CL_Vec3i");
	return impl->cl_vec3i_array;
}

bool CL_Collada_Source::is_null()
{
	return impl.is_null();
}
