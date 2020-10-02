/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

#include "D3D/precomp.h"
#include "d3d_primitives_array_provider.h"
#include "d3d_program_object_provider.h"
#include "d3d_vertex_array_buffer_provider.h"
#include "API/D3D/d3d_target.h"

namespace clan
{
	D3DPrimitivesArrayProvider::D3DPrimitivesArrayProvider(const ComPtr<ID3D11Device> &device)
		: device(device)
	{
	}

	D3DPrimitivesArrayProvider::~D3DPrimitivesArrayProvider()
	{
	}

	void D3DPrimitivesArrayProvider::get_vertex_buffers(std::vector<ID3D11Buffer *> &out_buffers, std::vector<UINT> &out_strides, std::vector<UINT> &out_offsets)
	{
		out_buffers.clear();
		out_strides.clear();
		out_offsets.clear();

		for (size_t i = 0; i < attributes_data.size(); i++)
		{
			if (attributes_set[i])
			{
				D3DVertexArrayBufferProvider *array_provider = static_cast<D3DVertexArrayBufferProvider*>(attributes_data[i].array_provider);
				out_buffers.push_back(array_provider->get_buffer(device));
				int stride = attributes_data[i].stride;
				// Stride is not optional in D3D
				if (!stride)
				{
					switch (attributes_data[i].type)
					{
					case VertexAttributeDataType::type_unsigned_byte:
					case VertexAttributeDataType::type_byte:
						stride = 1 * attributes_data[i].size;
						break;

					case VertexAttributeDataType::type_unsigned_short:
					case VertexAttributeDataType::type_short:
						stride = 2 * attributes_data[i].size;
						break;

					case VertexAttributeDataType::type_unsigned_int:
					case VertexAttributeDataType::type_int:
					case VertexAttributeDataType::type_float:
						stride = 4 * attributes_data[i].size;
						break;
					}
				}

				out_strides.push_back(stride);
				out_offsets.push_back(0);
			}
			else
			{
				out_buffers.push_back(0);
				out_strides.push_back(0);
				out_offsets.push_back(0);
			}
		}
	}

	size_t D3DPrimitivesArrayProvider::get_vertex_buffers_range() const
	{
		size_t count = 0;
		for (size_t i = 0; i < attributes_data.size(); i++)
			if (attributes_set[i])
				count++;
		return count;
	}

	void D3DPrimitivesArrayProvider::set_attribute(int index, const VertexData &data, bool normalize)
	{
		// To do: create some kind of reference count for data.array_provider so the object stays valid until primitives array is destroyed.

		input_layouts.clear();

		if (attributes_data.size() <= index)
		{
			attributes_data.resize(index + 1);
			attributes_normalize.resize(index + 1);
			attributes_set.resize(index + 1);
		}
		attributes_data[index] = data;
		attributes_normalize[index] = normalize;
		attributes_set[index] = true;
	}

	ID3D11InputLayout *D3DPrimitivesArrayProvider::get_input_layout(D3DProgramObjectProvider *program)
	{
		std::map<void *, ComPtr<ID3D11InputLayout> >::iterator it = input_layouts.find(program);
		if (it != input_layouts.end())
		{
			return it->second;
		}
		else
		{
			ComPtr<ID3D11InputLayout> layout = create_input_layout(program);
			input_layouts[program] = layout;
			return layout;
		}
	}

	ComPtr<ID3D11InputLayout> D3DPrimitivesArrayProvider::create_input_layout(D3DProgramObjectProvider *program)
	{
		DataBuffer shader_bytecode = program->get_shader_bytecode(ShaderType::vertex);

		std::vector<D3D11_INPUT_ELEMENT_DESC> elements;
		for (std::map<int, D3DProgramObjectProvider::AttributeBinding>::iterator it = program->attribute_bindings.begin(); it != program->attribute_bindings.end(); ++it)
		{
			int attrib_location = it->first;
			if (attrib_location < (int)attributes_data.size() && attributes_set[attrib_location])
			{
				D3D11_INPUT_ELEMENT_DESC desc;
				desc.SemanticName = it->second.semantic_name.c_str();
				desc.SemanticIndex = it->second.semantic_index;
				desc.Format = to_d3d_format(attributes_data[attrib_location], attributes_normalize[attrib_location]);
				desc.InputSlot = attrib_location;
				desc.AlignedByteOffset = attributes_data[attrib_location].offset;
				desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				desc.InstanceDataStepRate = 0;
				elements.push_back(desc);
			}
		}

		ComPtr<ID3D11InputLayout> input_layout;
		HRESULT result = device->CreateInputLayout(&elements[0], elements.size(), shader_bytecode.get_data(), shader_bytecode.get_size(), input_layout.output_variable());
		D3DTarget::throw_if_failed("CreateInputLayout failed", result);
		return input_layout;
	}

	DXGI_FORMAT D3DPrimitivesArrayProvider::to_d3d_format(const VertexData &data, bool normalize)
	{
		switch (data.size)
		{
		case 1:
			switch (data.type)
			{
			case VertexAttributeDataType::type_unsigned_byte:
				return normalize ? DXGI_FORMAT_R8_UNORM : DXGI_FORMAT_R8_UINT;
			case VertexAttributeDataType::type_unsigned_short:
				return normalize ? DXGI_FORMAT_R16_UNORM : DXGI_FORMAT_R16_UINT;
			case VertexAttributeDataType::type_unsigned_int:
				return /*normalize ? DXGI_FORMAT_R32_UNORM :*/ DXGI_FORMAT_R32_UINT;
			case VertexAttributeDataType::type_byte:
				return normalize ? DXGI_FORMAT_R8_SNORM : DXGI_FORMAT_R8_SINT;
			case VertexAttributeDataType::type_short:
				return normalize ? DXGI_FORMAT_R16_SNORM : DXGI_FORMAT_R16_SINT;
			case VertexAttributeDataType::type_int:
				return /*normalize ? DXGI_FORMAT_R32_SNORM :*/ DXGI_FORMAT_R32_SINT;
			case VertexAttributeDataType::type_float:
				return DXGI_FORMAT_R32_FLOAT;
			default:
				return DXGI_FORMAT_R32_TYPELESS;	// DXGI_FORMAT_R16_TYPELESS, DXGI_FORMAT_R8_TYPELESS
			}

			break;
		case 2:
			switch (data.type)
			{
			case VertexAttributeDataType::type_unsigned_byte:
				return normalize ? DXGI_FORMAT_R8G8_UNORM : DXGI_FORMAT_R8G8_UINT;
			case VertexAttributeDataType::type_unsigned_short:
				return normalize ? DXGI_FORMAT_R16G16_UNORM : DXGI_FORMAT_R16G16_UINT;
			case VertexAttributeDataType::type_unsigned_int:
				return /*normalize ? DXGI_FORMAT_R32G32_UNORM :*/ DXGI_FORMAT_R32G32_UINT;
			case VertexAttributeDataType::type_byte:
				return normalize ? DXGI_FORMAT_R8G8_SNORM : DXGI_FORMAT_R8G8_SINT;
			case VertexAttributeDataType::type_short:
				return normalize ? DXGI_FORMAT_R16G16_SNORM : DXGI_FORMAT_R16G16_SINT;
			case VertexAttributeDataType::type_int:
				return DXGI_FORMAT_R32G32_SINT;
			case VertexAttributeDataType::type_float:
				return DXGI_FORMAT_R32G32_FLOAT;
			default:
				return DXGI_FORMAT_R32G32_TYPELESS;	// DXGI_FORMAT_R16G16_TYPELESS, DXGI_FORMAT_R8G8_TYPELESS
			}
			break;
		case 3:
			switch (data.type)
			{
			case VertexAttributeDataType::type_unsigned_byte:
				break;
			case VertexAttributeDataType::type_unsigned_short:
				break;
			case VertexAttributeDataType::type_unsigned_int:
				return /*normalize ? DXGI_FORMAT_R32G32B32_UNORM :*/ DXGI_FORMAT_R32G32B32_UINT;
			case VertexAttributeDataType::type_byte:
				break;
			case VertexAttributeDataType::type_short:
				break;
			case VertexAttributeDataType::type_int:
				return /*normalize ? DXGI_FORMAT_R32G32B32_SNORM :*/ DXGI_FORMAT_R32G32B32_SINT;
			case VertexAttributeDataType::type_float:
				return DXGI_FORMAT_R32G32B32_FLOAT;
			default:
				return DXGI_FORMAT_R32G32B32_TYPELESS;	// DXGI_FORMAT_R8G8B8A8_TYPELESS
			}
			break;
		case 4:
			switch (data.type)
			{
			case VertexAttributeDataType::type_unsigned_byte:
				return normalize ? DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT_R8G8B8A8_UINT;
			case VertexAttributeDataType::type_unsigned_short:
				return normalize ? DXGI_FORMAT_R16G16B16A16_UNORM : DXGI_FORMAT_R16G16B16A16_UINT;
			case VertexAttributeDataType::type_unsigned_int:
				return /*normalize ? DXGI_FORMAT_R32G32B32A32_UNORM :*/ DXGI_FORMAT_R32G32B32A32_UINT;
			case VertexAttributeDataType::type_byte:
				return normalize ? DXGI_FORMAT_R8G8B8A8_SNORM : DXGI_FORMAT_R8G8B8A8_SINT;
			case VertexAttributeDataType::type_short:
				return normalize ? DXGI_FORMAT_R16G16B16A16_SNORM : DXGI_FORMAT_R16G16B16A16_SINT;
			case VertexAttributeDataType::type_int:
				return /*normalize ? DXGI_FORMAT_R32G32B32A32_SNORM :*/ DXGI_FORMAT_R32G32B32A32_SINT;
			case VertexAttributeDataType::type_float:
				return DXGI_FORMAT_R32G32B32A32_FLOAT;
			default:
				return DXGI_FORMAT_R32G32B32A32_TYPELESS;	// DXGI_FORMAT_R16G16B16A16_TYPELESS
			}

			break;
		}
		throw Exception("Cannot decode the datatype");
	}
}
