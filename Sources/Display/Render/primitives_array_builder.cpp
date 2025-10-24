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
**    Rebecca Palmer
*/

#include "Display/precomp.h"
#include "API/Display/Render/primitives_array_builder.h"
#include "API/Display/Render/primitives_array.h"
#include "API/Core/Math/angle.h"
#include "primitives_array_impl.h"

class CL_PrimitivesArrayBlock
{
public:
	CL_PrimitivesArrayBlock(const CL_PrimitivesArrayData *vertex_declaration)
	{
		array_data.attributes = new CL_PrimitivesArrayData::VertexData[vertex_declaration->num_attributes];
		array_data.attribute_indexes = new int[vertex_declaration->num_attributes];
		array_data.normalize_attributes = new bool[vertex_declaration->num_attributes];
		array_data.num_attributes = vertex_declaration->num_attributes;
		for (int i = 0; i < vertex_declaration->num_attributes; i++)
		{
			array_data.attributes[i] = alloc_vertex_data(vertex_declaration->attributes[i].size, vertex_declaration->attributes[i].type);
			array_data.attribute_indexes[i] = vertex_declaration->attribute_indexes[i];
			array_data.normalize_attributes[i] = vertex_declaration->normalize_attributes[i];
		}
	}

	~CL_PrimitivesArrayBlock()
	{
		for (int i = 0; i < array_data.num_attributes; i++)
			free_vertex_data(array_data.attributes[i]);
		delete[] array_data.attributes;
		delete[] array_data.attribute_indexes;
		delete[] array_data.normalize_attributes;
	}

	void copy_data(int offset, int offset_source, const CL_PrimitivesArrayData *source, int vertex_count)
	{
		if (array_data.num_attributes != source->num_attributes)
			throw CL_Exception("All primitives arrays passed to CL_PrimitivesArrayBuilder must use same vertex declaration");
			
		for (int i = 0; i < source->num_attributes; i++)
			if ((array_data.attribute_indexes[i] != source->attribute_indexes[i]) || (array_data.normalize_attributes[i] != source->normalize_attributes[i]))
				throw CL_Exception("All primitives arrays passed to CL_PrimitivesArrayBuilder must use same vertex declaration");

		for (int i = 0; i < source->num_attributes; i++)
			copy_vertex_data(offset, array_data.attributes[i], offset_source, source->attributes[i], vertex_count);
	}

	CL_PrimitivesArrayData *get_array_data() { return &array_data; }

	enum { total_vertices = 1020 };//Needs to be a multiple of the number of vertices per primitive, so one is not cut in half by the block boundary

private:
	CL_PrimitivesArrayData::VertexData alloc_vertex_data(int size, CL_VertexAttributeDataType type)
	{
		CL_PrimitivesArrayData::VertexData data;
		data.size = size;
		data.type = type;
		data.stride = 0;
		data.array_provider = 0;
		data.single_value = false;
		if (size == 0)
		{
			data.data = 0;
			return data;
		}

		int data_length = 0;
		switch (type)
		{
		case cl_type_unsigned_byte:   data_length = sizeof(unsigned char); break;
		case cl_type_unsigned_short:  data_length = sizeof(unsigned short); break;
		case cl_type_unsigned_int:    data_length = sizeof(unsigned int); break;
		case cl_type_byte:            data_length = sizeof(char); break;
		case cl_type_short:           data_length = sizeof(short); break;
		case cl_type_int:             data_length = sizeof(int); break;
		case cl_type_float:           data_length = sizeof(float); break;
		default:                      break;
		}
		data_length *= total_vertices * size;
		data.data = new char[data_length];
		return data;
	}

	void copy_vertex_data(int offset, CL_PrimitivesArrayData::VertexData &dest, int offset_source,
		const CL_PrimitivesArrayData::VertexData &source, int vertex_count)
	{
		if (source.size == 0)
			return;

		if (source.type != dest.type || source.size != dest.size)
			throw CL_Exception("All primitives arrays passed to CL_PrimitivesArrayBuilder must use same vertex declaration");

		int data_length = 0;
		switch (source.type)
		{
		case cl_type_unsigned_byte:   data_length = sizeof(unsigned char); break;
		case cl_type_unsigned_short:  data_length = sizeof(unsigned short); break;
		case cl_type_unsigned_int:    data_length = sizeof(unsigned int); break;
		case cl_type_byte:            data_length = sizeof(char); break;
		case cl_type_short:           data_length = sizeof(short); break;
		case cl_type_int:             data_length = sizeof(int); break;
		case cl_type_float:           data_length = sizeof(float); break;
		default:                      break;
		}
		data_length *= source.size;

		char *d = (char *) dest.data;

		if (source.single_value)
		{
			for (int i = 0; i < vertex_count; i++)
				memcpy(d + data_length * (offset + i), source.value_ubyte, data_length);
		}
		else
		{
			const char *s = (const char *) source.data;

			int pitch_source = data_length;
			if (source.stride != 0)
				pitch_source = source.stride;

			for (int i = 0; i < vertex_count; i++)
				memcpy(d + data_length * (offset + i), s + pitch_source * (offset_source + i), data_length);
		}
	}

	void free_vertex_data(CL_PrimitivesArrayData::VertexData &vertex_data)
	{
		if (vertex_data.single_value)
			return;
		delete[] static_cast<char *>(vertex_data.data);
	}

	CL_PrimitivesArrayData array_data;
};

class CL_PrimitivesArrayBuilder_Impl
{
public:
	CL_PrimitivesArrayBuilder_Impl()
	: next_vertex_pos(0)
	{
	}

	~CL_PrimitivesArrayBuilder_Impl()
	{
		for (std::vector<CL_PrimitivesArrayBlock *>::size_type i = 0; i < blocks.size(); i++)
			delete blocks[i];
	}

	std::vector<CL_PrimitivesArrayBlock *> blocks;
	int next_vertex_pos;
};

CL_PrimitivesArrayBuilder::CL_PrimitivesArrayBuilder()
: impl(new CL_PrimitivesArrayBuilder_Impl)
{
}

void CL_PrimitivesArrayBuilder::append(int num_vertices, const CL_PrimitivesArray &prim_array)
{
	int pos = 0;
	while (pos < num_vertices)
	{
		if (impl->blocks.empty() || impl->next_vertex_pos == CL_PrimitivesArrayBlock::total_vertices)
		{
			impl->blocks.push_back(new CL_PrimitivesArrayBlock(prim_array.impl));
			impl->next_vertex_pos = 0;
		}

		int available = CL_PrimitivesArrayBlock::total_vertices - impl->next_vertex_pos;
		int copy_amount = cl_min(available, num_vertices - pos);
		CL_PrimitivesArrayBlock *block = impl->blocks.back();
		block->copy_data(impl->next_vertex_pos, pos, prim_array.impl, copy_amount);
		pos += copy_amount;
		impl->next_vertex_pos += copy_amount;
	}
}

void CL_PrimitivesArrayBuilder::draw(CL_GraphicContext &gc, CL_PrimitivesType type)
{
	CL_GraphicContextProvider *gc_provider = gc.get_provider();
	std::vector<CL_PrimitivesArrayBlock *>::size_type index, size;
	size = impl->blocks.size();
	for (index = 0; index < size; index++)
	{
		gc_provider->set_primitives_array(impl->blocks[index]->get_array_data());
		int num_vertices = CL_PrimitivesArrayBlock::total_vertices;
		if (index + 1 == size)
			num_vertices = impl->next_vertex_pos;
		gc_provider->draw_primitives_array(type, 0, num_vertices);
	}

	gc.get_provider()->reset_primitives_array();
}

CL_VertexArrayBuffer CL_PrimitivesArrayBuilder::create_buffer(CL_GraphicContext &gc, CL_BufferUsage usage)
{
	throw CL_Exception("CL_PrimitivesArrayBuilder::create_buffer not implemented");
}
