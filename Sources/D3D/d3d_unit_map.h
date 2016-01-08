/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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

#include "API/Display/Render/texture.h"
#include "API/Display/Render/uniform_buffer.h"
#include "API/Display/Render/storage_buffer.h"
#include "d3d_program_object_provider.h"

namespace clan
{
	class D3DGraphicContextProvider;
	class D3DProgramObjectProvider;

	template<typename Type>
	class D3DUnit
	{
	public:
		D3DUnit() { for (int i = 0; i < shadertype_num_types; i++) shader_index[i] = -1; }
		int shader_index[shadertype_num_types];
		Type object;
	};

	typedef D3DUnit<Texture> D3DSamplerUnit;
	typedef D3DUnit<Texture> D3DTextureUnit;
	typedef D3DUnit<Texture> D3DImageUnit;
	typedef D3DUnit<UniformBuffer> D3DUniformUnit;

	class D3DStorageUnit
	{
	public:
		D3DStorageUnit() { for (int i = 0; i < shadertype_num_types; i++) { shader_srv_index[i] = -1; shader_uav_index[i] = -1; } }
		int shader_srv_index[shadertype_num_types];
		int shader_uav_index[shadertype_num_types];
		StorageBuffer object;
	};

	class D3DUnitMap
	{
	public:
		void bind_program(D3DGraphicContextProvider *gc, D3DProgramObjectProvider *program);
		void unbind_program(D3DGraphicContextProvider *gc, D3DProgramObjectProvider *program);
		void set_sampler(D3DGraphicContextProvider *gc, int index, const Texture &texture);
		void set_texture(D3DGraphicContextProvider *gc, int index, const Texture &texture);
		void set_image(D3DGraphicContextProvider *gc, int index, const Texture &texture);
		void set_uniform_buffer(D3DGraphicContextProvider *gc, int index, const UniformBuffer &buffer);
		void set_storage_buffer(D3DGraphicContextProvider *gc, int index, const StorageBuffer &buffer);

	private:
		void bind_sampler(D3DGraphicContextProvider *gc, int index);
		void bind_texture(D3DGraphicContextProvider *gc, int index);
		void bind_image(D3DGraphicContextProvider *gc, int index);
		void bind_uniform_buffer(D3DGraphicContextProvider *gc, int index);
		void bind_storage_buffer(D3DGraphicContextProvider *gc, int index);
		void unbind_sampler(D3DGraphicContextProvider *gc, int index);
		void unbind_texture(D3DGraphicContextProvider *gc, int index);
		void unbind_image(D3DGraphicContextProvider *gc, int index);
		void unbind_uniform_buffer(D3DGraphicContextProvider *gc, int index);
		void unbind_storage_buffer(D3DGraphicContextProvider *gc, int index);
		/*
			struct ShaderResourceBinding
			{
				enum Type { texture, storage } type;
				int index;
				ShaderResourceBinding(Type type, int index) : type(type), index(index) { }
			};

			struct UnorderedBinding
			{
				enum Type { texture, image } type;
				int index;
				UnorderedBinding(Type type, int index) : type(type), index(index) { }
			};

			typedef int SamplerBinding;
			typedef int BufferBinding;

			std::vector<ShaderResourceBinding> shader_resources[shadertype_num_types];
			std::vector<SamplerBinding> samplers[shadertype_num_types];
			std::vector<BufferBinding> uniforms[shadertype_num_types];
			std::vector<UnorderedBinding> uavs[shadertype_num_types];
		*/
		std::vector<D3DSamplerUnit> sampler_units;
		std::vector<D3DTextureUnit> texture_units;
		std::vector<D3DImageUnit> image_units;
		std::vector<D3DUniformUnit> uniform_units;
		std::vector<D3DStorageUnit> storage_units;
	};

}
