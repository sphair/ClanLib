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

#pragma once

#include "API/Display/TargetProviders/primitives_array_provider.h"
#include <map>

namespace clan
{
	class D3DProgramObjectProvider;

	class D3DPrimitivesArrayProvider : public PrimitivesArrayProvider
	{
	public:
		D3DPrimitivesArrayProvider(const ComPtr<ID3D11Device> &device);
		~D3DPrimitivesArrayProvider() override;

		ComPtr<ID3D11Device> &get_device() { return device; }
		void get_vertex_buffers(std::vector<ID3D11Buffer *> &out_buffers, std::vector<UINT> &out_strides, std::vector<UINT> &out_offsets);
		size_t get_vertex_buffers_range() const;

		void set_attribute(int index, const VertexData &data, bool normalize) override;

		ID3D11InputLayout *get_input_layout(D3DProgramObjectProvider *program);

	private:
		ComPtr<ID3D11InputLayout> create_input_layout(D3DProgramObjectProvider *program);
		static DXGI_FORMAT to_d3d_format(const VertexData &data, bool normalize);

		ComPtr<ID3D11Device> device;
		std::vector<VertexData> attributes_data;
		std::vector<bool> attributes_normalize;
		std::vector<bool> attributes_set;
		std::map<void *, ComPtr<ID3D11InputLayout> > input_layouts;
	};
}
