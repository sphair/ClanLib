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
*/

#include "D3D/precomp.h"
#include "d3d_graphic_context_provider.h"
#include "d3d_display_window_provider.h"
#include "d3d_texture_provider.h"
#include "d3d_element_array_buffer_provider.h"
#include "d3d_pixel_buffer_provider.h"
#include "d3d_frame_buffer_provider.h"
#include "d3d_occlusion_query_provider.h"
#include "d3d_program_object_provider.h"
#include "d3d_render_buffer_provider.h"
#include "d3d_shader_object_provider.h"
#include "d3d_vertex_array_buffer_provider.h"
#include "d3d_primitives_array_provider.h"
#include "d3d_uniform_buffer_provider.h"
#include "d3d_storage_buffer_provider.h"
#include "d3d_element_array_buffer_provider.h"
#include "d3d_transfer_buffer_provider.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Render/transfer_texture.h"
#include "API/Display/Render/shared_gc_data.h"
#include "API/Display/Window/display_window_description.h"
#include "API/D3D/d3d_target.h"

namespace clan
{
	D3DGraphicContextProvider::D3DGraphicContextProvider(D3DDisplayWindowProvider *window, const DisplayWindowDescription &display_desc)
		: window(window),
		current_prim_array_provider(0),
		current_program_provider(0),
		input_layout_set(false)
	{
		//	set_blend_color(Colorf::black);
		//	set_blend_function(blend_one, blend_zero, blend_one, blend_zero);
		//	enable_blending(false);

		default_depth = display_desc.get_depth_size();

		Size viewport_size = get_display_window_size();
		for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		{
			viewports[i].Width = viewport_size.width;
			viewports[i].Height = viewport_size.height;
			viewports[i].TopLeftX = 0;
			viewports[i].TopLeftY = 0;
			viewports[i].MinDepth = 0.0f;
			viewports[i].MaxDepth = 1.0f;
		}
		window->get_device_context()->RSSetViewports(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, viewports);

		for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		{
			scissor_rects[i].left = 0;
			scissor_rects[i].top = 0;
			scissor_rects[i].right = 0;
			scissor_rects[i].bottom = 0;
		}

		for (int i = 0; i < shadertype_num_types; i++)
			shader_bound[i] = false;

		set_default_dsv();

		SharedGCData::add_provider(this);
	}

	D3DGraphicContextProvider::~D3DGraphicContextProvider()
	{
		SharedGCData::remove_provider(this);
	}

	void D3DGraphicContextProvider::begin_resize_swap_chain()
	{
		window->get_device_context()->OMSetRenderTargets(0, 0, 0);
		window->get_device_context()->Flush();

		default_dsv.clear();
		default_depth_render_buffer.reset();
	}

	void D3DGraphicContextProvider::end_resize_swap_chain()
	{
		Size viewport_size = get_display_window_size();
		viewports[0].Width = viewport_size.width;
		viewports[0].Height = viewport_size.height;
		viewports[0].TopLeftX = 0;
		viewports[0].TopLeftY = 0;
		window->get_device_context()->RSSetViewports(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, viewports);

		set_default_dsv();
	}

	void D3DGraphicContextProvider::flush()
	{
		window->get_device_context()->Flush();
	}

	int D3DGraphicContextProvider::get_max_attributes()
	{
		return 16; // To do: this is the D3D10 limit - is it still the same for D3D11?
	}

	Size D3DGraphicContextProvider::get_max_texture_size() const
	{
		switch (window->get_feature_level())
		{
		case D3D_FEATURE_LEVEL_9_1:
		case D3D_FEATURE_LEVEL_9_2: return Size(2048, 2048);
		case D3D_FEATURE_LEVEL_9_3: return Size(4096, 4096);
		case D3D_FEATURE_LEVEL_10_0:
		case D3D_FEATURE_LEVEL_10_1: return Size(8192, 8192);
		default:
		case D3D_FEATURE_LEVEL_11_0: return Size(16384, 16384);
		}
	}

	Size D3DGraphicContextProvider::get_display_window_size() const
	{
		return window->get_viewport().get_size();
	}

	float D3DGraphicContextProvider::get_pixel_ratio() const
	{
		return window->get_pixel_ratio();
	}

	HDC D3DGraphicContextProvider::get_drawable() const
	{
		return 0; //window->get_device_context();
	}

	ProgramObject D3DGraphicContextProvider::get_program_object(StandardProgram standard_program) const
	{
		return standard_programs.get_program_object(standard_program);
	}

	PixelBuffer D3DGraphicContextProvider::get_pixeldata(const Rect& rect, TextureFormat texture_format, bool clamp) const
	{
		// To do: fetch format from window->get_back_buffer()->GetDesc(&desc)
		// To do: window->get_back_buffer() is only correct when no frame buffer is bound
		TransferTexture pixels(const_cast<D3DDisplayWindowProvider*>(window)->get_gc(), rect.get_width(), rect.get_height(), data_from_gpu, texture_format);
		D3DPixelBufferProvider *pb_provider = static_cast<D3DPixelBufferProvider *>(pixels.get_provider());
		D3D11_BOX box;
		box.left = rect.left;
		box.top = rect.top;
		box.right = rect.right;
		box.bottom = rect.bottom;
		box.front = 0;
		box.back = 1;
		window->get_device_context()->CopySubresourceRegion(pb_provider->get_texture_2d(window->get_device()), 0, 0, 0, 0, window->get_back_buffer(), 0, &box);
		return pixels;
	}

	TextureProvider *D3DGraphicContextProvider::alloc_texture(TextureDimensions texture_dimensions)
	{
		return new D3DTextureProvider(window->get_device(), window->get_feature_level(), texture_dimensions);
	}

	OcclusionQueryProvider *D3DGraphicContextProvider::alloc_occlusion_query()
	{
		return new D3DOcclusionQueryProvider;
	}

	ProgramObjectProvider *D3DGraphicContextProvider::alloc_program_object()
	{
		return new D3DProgramObjectProvider(window->get_device(), window->get_device_context());
	}

	ShaderObjectProvider *D3DGraphicContextProvider::alloc_shader_object()
	{
		return new D3DShaderObjectProvider(window->get_device(), window->get_feature_level());
	}

	FrameBufferProvider *D3DGraphicContextProvider::alloc_frame_buffer()
	{
		return new D3DFrameBufferProvider(window->get_device());
	}

	RenderBufferProvider *D3DGraphicContextProvider::alloc_render_buffer()
	{
		return new D3DRenderBufferProvider(window->get_device());
	}

	VertexArrayBufferProvider *D3DGraphicContextProvider::alloc_vertex_array_buffer()
	{
		return new D3DVertexArrayBufferProvider(window->get_device());
	}

	UniformBufferProvider *D3DGraphicContextProvider::alloc_uniform_buffer()
	{
		return new D3DUniformBufferProvider(window->get_device());
	}

	StorageBufferProvider *D3DGraphicContextProvider::alloc_storage_buffer()
	{
		return new D3DStorageBufferProvider(window->get_device());
	}

	ElementArrayBufferProvider *D3DGraphicContextProvider::alloc_element_array_buffer()
	{
		return new D3DElementArrayBufferProvider(window->get_device());
	}

	TransferBufferProvider *D3DGraphicContextProvider::alloc_transfer_buffer()
	{
		return new D3DTransferBufferProvider(window->get_device());
	}

	PixelBufferProvider *D3DGraphicContextProvider::alloc_pixel_buffer()
	{
		return new D3DPixelBufferProvider(window->get_device());
	}

	PrimitivesArrayProvider *D3DGraphicContextProvider::alloc_primitives_array()
	{
		return new D3DPrimitivesArrayProvider(window->get_device());
	}

	std::shared_ptr<RasterizerStateProvider> D3DGraphicContextProvider::create_rasterizer_state(const RasterizerStateDescription &desc)
	{
		std::map<RasterizerStateDescription, std::shared_ptr<RasterizerStateProvider> >::iterator it = rasterizer_states.find(desc);
		if (it != rasterizer_states.end())
		{
			return it->second;
		}
		else
		{
			std::shared_ptr<RasterizerStateProvider> state(new D3DRasterizerStateProvider(window->get_device(), desc));
			rasterizer_states[desc.clone()] = state;
			return state;
		}
	}

	std::shared_ptr<BlendStateProvider> D3DGraphicContextProvider::create_blend_state(const BlendStateDescription &desc)
	{
		std::map<BlendStateDescription, std::shared_ptr<BlendStateProvider> >::iterator it = blend_states.find(desc);
		if (it != blend_states.end())
		{
			return it->second;
		}
		else
		{
			std::shared_ptr<BlendStateProvider> state(new D3DBlendStateProvider(window->get_device(), desc));
			blend_states[desc.clone()] = state;
			return state;
		}
	}

	std::shared_ptr<DepthStencilStateProvider> D3DGraphicContextProvider::create_depth_stencil_state(const DepthStencilStateDescription &desc)
	{
		std::map<DepthStencilStateDescription, std::shared_ptr<DepthStencilStateProvider> >::iterator it = depth_stencil_states.find(desc);
		if (it != depth_stencil_states.end())
		{
			return it->second;
		}
		else
		{
			std::shared_ptr<DepthStencilStateProvider> state(new D3DDepthStencilStateProvider(window->get_device(), desc));
			depth_stencil_states[desc.clone()] = state;
			return state;
		}
	}

	void D3DGraphicContextProvider::set_rasterizer_state(RasterizerStateProvider *state)
	{
		if (state)
		{
			D3DRasterizerStateProvider *d3d_state = static_cast<D3DRasterizerStateProvider*>(state);
			window->get_device_context()->RSSetState(d3d_state->state);
		}
	}

	void D3DGraphicContextProvider::set_blend_state(BlendStateProvider *state, const Colorf &blend_color, unsigned int sample_mask)
	{
		if (state)
		{
			D3DBlendStateProvider *d3d_state = static_cast<D3DBlendStateProvider*>(state);
			FLOAT blend_factor[4] = { blend_color.r, blend_color.g, blend_color.b, blend_color.a };
			window->get_device_context()->OMSetBlendState(d3d_state->state, blend_factor, sample_mask);
		}
	}

	void D3DGraphicContextProvider::set_depth_stencil_state(DepthStencilStateProvider *state, int stencil_ref)
	{
		if (state)
		{
			D3DDepthStencilStateProvider *d3d_state = static_cast<D3DDepthStencilStateProvider*>(state);
			window->get_device_context()->OMSetDepthStencilState(d3d_state->state, stencil_ref);
		}
	}

	void D3DGraphicContextProvider::set_program_object(StandardProgram standard_program)
	{
		ProgramObject program = get_program_object(standard_program);
		set_program_object(program);
	}

	void D3DGraphicContextProvider::set_program_object(const ProgramObject &program)
	{
		D3DProgramObjectProvider *new_program_provider = static_cast<D3DProgramObjectProvider *>(program.get_provider());
		if (new_program_provider == current_program_provider)
			return;

		if (current_program_provider)
			unit_map.unbind_program(this, current_program_provider);

		current_program_provider = new_program_provider;

		clear_input_layout();

		for (int j = 0; j < shadertype_num_types; j++)
		{
			D3DShaderObjectProvider *shader_provider = current_program_provider->get_shader_provider((ShaderType)j);
			if (shader_provider)
			{
				switch (j)
				{
				case shadertype_vertex: window->get_device_context()->VSSetShader(shader_provider->get_vertex(), 0, 0); break;
				case shadertype_tess_control: window->get_device_context()->HSSetShader(shader_provider->get_hull(), 0, 0); break;
				case shadertype_tess_evaluation: window->get_device_context()->DSSetShader(shader_provider->get_domain(), 0, 0); break;
				case shadertype_geometry: window->get_device_context()->GSSetShader(shader_provider->get_geometry(), 0, 0); break;
				case shadertype_fragment: window->get_device_context()->PSSetShader(shader_provider->get_pixel(), 0, 0); break;
				case shadertype_compute: window->get_device_context()->CSSetShader(shader_provider->get_compute(), 0, 0); break;
				}
				shader_bound[j] = true;
			}
			else if (shader_bound[j])
			{
				switch (j)
				{
				case shadertype_vertex: window->get_device_context()->VSSetShader(0, 0, 0); break;
				case shadertype_tess_control: window->get_device_context()->HSSetShader(0, 0, 0); break;
				case shadertype_tess_evaluation: window->get_device_context()->DSSetShader(0, 0, 0); break;
				case shadertype_geometry: window->get_device_context()->GSSetShader(0, 0, 0); break;
				case shadertype_fragment: window->get_device_context()->PSSetShader(0, 0, 0); break;
				case shadertype_compute: window->get_device_context()->CSSetShader(0, 0, 0); break;
				}
				shader_bound[j] = false;
			}
		}

		unit_map.bind_program(this, current_program_provider);
	}

	void D3DGraphicContextProvider::reset_program_object()
	{
		if (current_program_provider)
		{
			unit_map.unbind_program(this, current_program_provider);
			current_program_provider = 0;
			for (int j = 0; j < shadertype_num_types; j++)
			{
				if (shader_bound[j])
				{
					switch (j)
					{
					case shadertype_vertex: window->get_device_context()->VSSetShader(0, 0, 0); break;
					case shadertype_tess_control: window->get_device_context()->HSSetShader(0, 0, 0); break;
					case shadertype_tess_evaluation: window->get_device_context()->DSSetShader(0, 0, 0); break;
					case shadertype_geometry: window->get_device_context()->GSSetShader(0, 0, 0); break;
					case shadertype_fragment: window->get_device_context()->PSSetShader(0, 0, 0); break;
					case shadertype_compute: window->get_device_context()->CSSetShader(0, 0, 0); break;
					}
					shader_bound[j] = false;
				}
			}
			clear_input_layout();
		}
	}

	void D3DGraphicContextProvider::set_uniform_buffer(int index, const UniformBuffer &buffer)
	{
		unit_map.set_uniform_buffer(this, index, buffer);
	}

	void D3DGraphicContextProvider::reset_uniform_buffer(int index)
	{
		unit_map.set_uniform_buffer(this, index, UniformBuffer());
	}

	void D3DGraphicContextProvider::set_storage_buffer(int index, const StorageBuffer &buffer)
	{
		unit_map.set_storage_buffer(this, index, buffer);
	}

	void D3DGraphicContextProvider::reset_storage_buffer(int index)
	{
		unit_map.set_storage_buffer(this, index, StorageBuffer());
	}

	void D3DGraphicContextProvider::set_texture(int unit_index, const Texture &texture)
	{
		unit_map.set_texture(this, unit_index, texture);
	}

	void D3DGraphicContextProvider::reset_texture(int unit_index)
	{
		unit_map.set_texture(this, unit_index, Texture());
	}

	void D3DGraphicContextProvider::set_image_texture(int unit_index, const Texture &texture)
	{
		unit_map.set_image(this, unit_index, texture);
	}

	void D3DGraphicContextProvider::reset_image_texture(int unit_index)
	{
		unit_map.set_image(this, unit_index, Texture());
	}

	bool D3DGraphicContextProvider::is_frame_buffer_owner(const FrameBuffer &fb)
	{
		D3DFrameBufferProvider *fb_provider = static_cast<D3DFrameBufferProvider *>(fb.get_provider());
		if (fb_provider)
			return fb_provider->get_device() == window->get_device();
		else
			return false;
	}

	void D3DGraphicContextProvider::set_frame_buffer(const FrameBuffer &write_buffer, const FrameBuffer &read_buffer)
	{
		D3DFrameBufferProvider *fb_provider = static_cast<D3DFrameBufferProvider *>(write_buffer.get_provider());
		ID3D11DepthStencilView *dsv = 0;
		std::vector<ID3D11RenderTargetView *> rtvs = fb_provider->get_views(dsv);
		window->get_device_context()->OMSetRenderTargets(rtvs.size(), (!rtvs.empty()) ? &rtvs[0] : 0, dsv);
	}

	void D3DGraphicContextProvider::reset_frame_buffer()
	{
		set_default_dsv();
	}

	void D3DGraphicContextProvider::set_draw_buffer(DrawBuffer buffer)
	{
		// To do: what does this map to in D3D?
	}

	bool D3DGraphicContextProvider::is_primitives_array_owner(const PrimitivesArray &primitives_array)
	{
		D3DPrimitivesArrayProvider *array_provider = static_cast<D3DPrimitivesArrayProvider *>(primitives_array.get_provider());
		if (array_provider)
			return array_provider->get_device() == window->get_device();
		else
			return false;
	}

	void D3DGraphicContextProvider::apply_input_layout()
	{
		if (!input_layout_set)
		{
			window->get_device_context()->IASetInputLayout(current_prim_array_provider->get_input_layout(current_program_provider));
			input_layout_set = true;
		}
	}

	void D3DGraphicContextProvider::clear_input_layout()
	{
		if (input_layout_set)
		{
			input_layout_set = false;
			window->get_device_context()->IASetInputLayout(0);
		}
	}

	void D3DGraphicContextProvider::draw_primitives(PrimitivesType type, int num_vertices, const PrimitivesArray &primitives_array)
	{
		set_primitives_array(primitives_array);
		draw_primitives_array(type, 0, num_vertices);
		reset_primitives_array();
	}

	void D3DGraphicContextProvider::set_primitives_array(const PrimitivesArray &primitives_array)
	{
		reset_primitives_array();
		current_prim_array_provider = static_cast<D3DPrimitivesArrayProvider *>(primitives_array.get_provider());
		std::vector<ID3D11Buffer*> buffers;
		std::vector<UINT> strides, offsets;
		current_prim_array_provider->get_vertex_buffers(buffers, strides, offsets);
		if (!buffers.empty())
			window->get_device_context()->IASetVertexBuffers(0, buffers.size(), &buffers[0], &strides[0], &offsets[0]);
	}

	void D3DGraphicContextProvider::reset_primitives_array()
	{
		clear_input_layout();
		if (current_prim_array_provider)
		{
			std::vector<ID3D11Buffer*> buffers;
			std::vector<UINT> strides, offsets;
			size_t range = current_prim_array_provider->get_vertex_buffers_range();
			buffers.resize(range);
			strides.resize(range);
			offsets.resize(range);
			if (range != 0)
				window->get_device_context()->IASetVertexBuffers(0, buffers.size(), &buffers[0], &strides[0], &offsets[0]);
		}

		current_prim_array_provider = 0;
	}

	void D3DGraphicContextProvider::draw_primitives_array(PrimitivesType type, int offset, int num_vertices)
	{
		apply_input_layout();
		window->get_device_context()->IASetPrimitiveTopology(to_d3d_primitive_topology(type));
		window->validate_context();
		window->get_device_context()->Draw(num_vertices, offset);
	}

	void D3DGraphicContextProvider::draw_primitives_array_instanced(PrimitivesType type, int offset, int num_vertices, int instance_count)
	{
		apply_input_layout();
		window->get_device_context()->IASetPrimitiveTopology(to_d3d_primitive_topology(type));
		window->validate_context();
		window->get_device_context()->DrawInstanced(num_vertices, instance_count, offset, 0);
	}

	void D3DGraphicContextProvider::set_primitives_elements(ElementArrayBufferProvider *array_provider)
	{
		current_element_array_provider = static_cast<D3DElementArrayBufferProvider*>(array_provider);
	}

	void D3DGraphicContextProvider::reset_primitives_elements()
	{
		window->get_device_context()->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
		current_element_array_provider = 0;
	}

	void D3DGraphicContextProvider::draw_primitives_elements(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset)
	{
		apply_input_layout();
		window->get_device_context()->IASetPrimitiveTopology(to_d3d_primitive_topology(type));
		window->get_device_context()->IASetIndexBuffer(current_element_array_provider->get_buffer(window->get_device()), to_d3d_format(indices_type), 0);
		window->validate_context();
		window->get_device_context()->DrawIndexed(count, to_d3d_index_location(indices_type, offset), 0);
	}

	void D3DGraphicContextProvider::draw_primitives_elements_instanced(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset, int instance_count)
	{
		apply_input_layout();
		window->get_device_context()->IASetPrimitiveTopology(to_d3d_primitive_topology(type));
		window->get_device_context()->IASetIndexBuffer(current_element_array_provider->get_buffer(window->get_device()), to_d3d_format(indices_type), 0);
		window->validate_context();
		window->get_device_context()->DrawIndexedInstanced(count, instance_count, to_d3d_index_location(indices_type, offset), 0, 0);
	}

	void D3DGraphicContextProvider::draw_primitives_elements(PrimitivesType type, int count, ElementArrayBufferProvider *array_provider, VertexAttributeDataType indices_type, void *offset)
	{
		set_primitives_elements(array_provider);
		apply_input_layout();
		window->get_device_context()->IASetPrimitiveTopology(to_d3d_primitive_topology(type));
		window->get_device_context()->IASetIndexBuffer(current_element_array_provider->get_buffer(window->get_device()), to_d3d_format(indices_type), 0);
		window->validate_context();
		window->get_device_context()->DrawIndexed(count, to_d3d_index_location(indices_type, reinterpret_cast<UINT>(offset)), 0);
		reset_primitives_elements();
	}

	void D3DGraphicContextProvider::draw_primitives_elements_instanced(PrimitivesType type, int count, ElementArrayBufferProvider *array_provider, VertexAttributeDataType indices_type, void *offset, int instance_count)
	{
		set_primitives_elements(array_provider);
		apply_input_layout();
		window->get_device_context()->IASetPrimitiveTopology(to_d3d_primitive_topology(type));
		window->get_device_context()->IASetIndexBuffer(current_element_array_provider->get_buffer(window->get_device()), to_d3d_format(indices_type), 0);
		window->validate_context();
		window->get_device_context()->DrawIndexedInstanced(count, instance_count, to_d3d_index_location(indices_type, reinterpret_cast<UINT>(offset)), 0, 0);
		reset_primitives_elements();
	}

	void D3DGraphicContextProvider::set_scissor(const Rect &rect)
	{
		for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		{
			scissor_rects[i].left = rect.left;
			scissor_rects[i].top = rect.top;
			scissor_rects[i].right = rect.right;
			scissor_rects[i].bottom = rect.bottom;
		}
		window->get_device_context()->RSSetScissorRects(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, scissor_rects);
	}

	void D3DGraphicContextProvider::reset_scissor()
	{
		set_scissor(Rect(0, 0, 0x7FFFFFFF, 0x7FFFFFFF));
		// The below does not work, why?
		//window->get_device_context()->RSSetScissorRects(0, 0);
	}

	void D3DGraphicContextProvider::dispatch(int x, int y, int z)
	{
		window->get_device_context()->Dispatch(x, y, z);
	}

	void D3DGraphicContextProvider::clear(const Colorf &color)
	{
		ID3D11RenderTargetView *views[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
		window->get_device_context()->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, views, 0);

		FLOAT c[4] = { color.r, color.g, color.b, color.a };
		for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		{
			if (views[i])
			{
				window->get_device_context()->ClearRenderTargetView(views[i], c);
				views[i]->Release();
			}
		}
	}

	void D3DGraphicContextProvider::clear_depth(float value)
	{
		ID3D11DepthStencilView *dsv = 0;
		window->get_device_context()->OMGetRenderTargets(0, 0, &dsv);
		if (dsv)
		{
			window->get_device_context()->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, value, 0);
			dsv->Release();
		}
	}

	void D3DGraphicContextProvider::clear_stencil(int value)
	{
		ID3D11DepthStencilView *dsv = 0;
		window->get_device_context()->OMGetRenderTargets(0, 0, &dsv);
		if (dsv)
		{
			window->get_device_context()->ClearDepthStencilView(dsv, D3D11_CLEAR_STENCIL, 0, value);
			dsv->Release();
		}
	}

	void D3DGraphicContextProvider::set_viewport(const Rectf &viewport)
	{
		for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		{
			viewports[i].TopLeftX = viewport.left;
			viewports[i].TopLeftY = viewport.top;
			viewports[i].Width = viewport.get_width();
			viewports[i].Height = viewport.get_height();
		}
		window->get_device_context()->RSSetViewports(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, viewports);
	}

	void D3DGraphicContextProvider::set_viewport(int index, const Rectf &viewport)
	{
		if (index >= 0 && index < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)
		{
			viewports[index].TopLeftX = viewport.left;
			viewports[index].TopLeftY = viewport.top;
			viewports[index].Width = viewport.get_width();
			viewports[index].Height = viewport.get_height();
			window->get_device_context()->RSSetViewports(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, viewports);
		}
	}

	void D3DGraphicContextProvider::set_depth_range(float n, float f)
	{
		for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		{
			viewports[i].MinDepth = n;
			viewports[i].MaxDepth = f;
		}
		window->get_device_context()->RSSetViewports(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, viewports);
	}

	void D3DGraphicContextProvider::set_depth_range(int index, float n, float f)
	{
		if (index >= 0 && index < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)
		{
			viewports[index].MinDepth = n;
			viewports[index].MaxDepth = f;
			window->get_device_context()->RSSetViewports(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, viewports);
		}
	}

	void D3DGraphicContextProvider::on_window_resized()
	{
		Size new_size = window->get_viewport().get_size();
		window_resized_signal(new_size);
	}

	int D3DGraphicContextProvider::get_major_version() const
	{
		switch (window->get_feature_level())
		{
		case D3D_FEATURE_LEVEL_9_1:
		case D3D_FEATURE_LEVEL_9_2:
		case D3D_FEATURE_LEVEL_9_3: return 9;
		case D3D_FEATURE_LEVEL_10_0:
		case D3D_FEATURE_LEVEL_10_1: return 10;
		case D3D_FEATURE_LEVEL_11_0:
		default:
		case D3D_FEATURE_LEVEL_11_1: return 11;
		}
	}

	int D3DGraphicContextProvider::get_minor_version() const
	{
		switch (window->get_feature_level())
		{
		case D3D_FEATURE_LEVEL_9_1: return 1;
		case D3D_FEATURE_LEVEL_9_2: return 2;
		case D3D_FEATURE_LEVEL_9_3: return 3;
		case D3D_FEATURE_LEVEL_10_0: return 0;
		case D3D_FEATURE_LEVEL_10_1: return 1;
		case D3D_FEATURE_LEVEL_11_0: return 0;
		default:
		case D3D_FEATURE_LEVEL_11_1: return 1;
		}
	}

	bool D3DGraphicContextProvider::has_compute_shader_support() const
	{
		switch (window->get_feature_level())
		{
		case D3D_FEATURE_LEVEL_9_1:
		case D3D_FEATURE_LEVEL_9_2:
		case D3D_FEATURE_LEVEL_9_3: return false;
		case D3D_FEATURE_LEVEL_10_0:
		case D3D_FEATURE_LEVEL_10_1: break;
		case D3D_FEATURE_LEVEL_11_0:
		default:
		case D3D_FEATURE_LEVEL_11_1: return true;
		}

		D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS options = { 0 };

		HRESULT result = window->get_device()->CheckFeatureSupport(D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS, &options, sizeof(D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS));
		D3DTarget::throw_if_failed("D3D11Device.CheckFeatureSupport(D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS) failed", result);

		return options.ComputeShaders_Plus_RawAndStructuredBuffers_Via_Shader_4_x != FALSE;
	}

	D3D11_PRIMITIVE_TOPOLOGY D3DGraphicContextProvider::to_d3d_primitive_topology(PrimitivesType type)
	{
		switch (type)
		{
		case type_points: return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		case type_line_strip: return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
		case type_line_loop: break;
		case type_lines: return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		case type_triangle_strip: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		case type_triangle_fan: break;
		case type_triangles: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		}
		throw Exception("Unsupported primitives type");
	}

	DXGI_FORMAT D3DGraphicContextProvider::to_d3d_format(VertexAttributeDataType indices_type)
	{
		switch (indices_type)
		{
		case type_unsigned_byte: return DXGI_FORMAT_R8_UINT;
		case type_unsigned_short: return DXGI_FORMAT_R16_UINT;
		case type_unsigned_int: return DXGI_FORMAT_R32_UINT;
		case type_byte: return DXGI_FORMAT_R8_SINT;
		case type_short: return DXGI_FORMAT_R16_SINT;
		case type_int: return DXGI_FORMAT_R32_SINT;
		case type_float: return DXGI_FORMAT_R32_FLOAT;
		}
		throw Exception("Unsupported index type");
	}

	UINT D3DGraphicContextProvider::to_d3d_index_location(VertexAttributeDataType indices_type, size_t offset)
	{
		int index_data_bytesize = 4;
		switch (indices_type)
		{
		case type_unsigned_byte:
		case type_byte:
			index_data_bytesize = 1;
			break;

		case type_unsigned_short:
		case type_short:
			index_data_bytesize = 2;
			break;

		case type_unsigned_int:
		case type_int:
		case type_float:
			index_data_bytesize = 4;
			break;
		}
		if (offset % index_data_bytesize != 0)
			throw Exception("Unaligned element offsets not supported by D3D target");
		return offset / index_data_bytesize;
	}

	void D3DGraphicContextProvider::set_default_dsv()
	{
		if (default_depth != 0 && !default_depth_render_buffer)
		{
			TextureFormat texture_format = tf_depth_component32;
			if (default_depth <= 16)
				texture_format = tf_depth_component32;
			else  if (default_depth <= 24)
				texture_format = tf_depth_component24;

			Size viewport_size = get_display_window_size();

			default_depth_render_buffer = std::shared_ptr<D3DRenderBufferProvider>(new D3DRenderBufferProvider(window->get_device()));
			default_depth_render_buffer->create(viewport_size.width, viewport_size.height, texture_format, 1);
			default_dsv = default_depth_render_buffer->create_dsv(window->get_device());
		}

		ID3D11RenderTargetView *targets[] = { window->get_back_buffer_rtv() };
		ID3D11DepthStencilView *default_depth_stencil_rtv = (default_depth != 0) ? default_dsv.get() : nullptr;
		window->get_device_context()->OMSetRenderTargets(1, targets, default_depth_stencil_rtv);
	}
}
