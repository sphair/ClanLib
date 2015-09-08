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
**    Mark Page
*/

#include "D3D/precomp.h"
#include "API/D3D/d3d_target.h"
#include "API/Display/display_target.h"
#include "API/Display/Render/render_buffer.h"
#include "API/Display/Render/uniform_buffer.h"
#include "API/Core/Text/string_format.h"
#include "d3d_target_provider.h"
#include "d3d_texture_provider.h"
#include "d3d_pixel_buffer_provider.h"
#include "d3d_render_buffer_provider.h"
#include "d3d_vertex_array_buffer_provider.h"
#include "d3d_element_array_buffer_provider.h"
#include "d3d_uniform_buffer_provider.h"
#include "d3d_graphic_context_provider.h"
#include "d3d_display_window_provider.h"
#include "setup_d3d_impl.h"
#include "setup_d3d.h"

namespace clan
{
	bool D3DTarget::is_current()
	{
		return std::dynamic_pointer_cast<D3DTargetProvider>(DisplayTarget::get_current_target()) ? true : false;
	}

	void D3DTarget::set_current()
	{
		SetupD3D::start();
		if (!SetupD3D_Impl::instance)
			throw Exception("clanD3D instance invalid");

		static std::shared_ptr<D3DTargetProvider> target = std::make_shared<D3DTargetProvider>();
		DisplayTarget::set_current_target(target);
	}

	ID3D11Texture2D *D3DTarget::get_texture2d_handle(const GraphicContext &gc, const Texture &texture)
	{
		D3DTextureProvider *provider = static_cast<D3DTextureProvider *>(texture.get_provider());
		return provider->get_texture_2d(static_cast<const D3DGraphicContextProvider *>(gc.get_provider())->get_window()->get_device());
	}

	ID3D11Texture2D *D3DTarget::get_texture2d_handle(const GraphicContext &gc, const PixelBuffer &pixel_buffer)
	{
		//D3DPixelBufferProvider *provider = static_cast<D3DPixelBufferProvider *>(pixel_buffer.get_provider());
		//return provider->get_texture_2d();
		return 0;
	}

	ID3D11Texture2D *D3DTarget::get_texture2d_handle(const GraphicContext &gc, const RenderBuffer &render_buffer)
	{
		D3DRenderBufferProvider *provider = static_cast<D3DRenderBufferProvider*>(render_buffer.get_provider());
		return provider->get_texture(static_cast<const D3DGraphicContextProvider *>(gc.get_provider())->get_window()->get_device());
	}

	ID3D11Buffer *D3DTarget::get_buffer_handle(const GraphicContext &gc, const VertexArrayBuffer &buffer)
	{
		D3DVertexArrayBufferProvider *provider = static_cast<D3DVertexArrayBufferProvider *>(buffer.get_provider());
		return provider->get_buffer(static_cast<const D3DGraphicContextProvider *>(gc.get_provider())->get_window()->get_device());
	}

	ID3D11Buffer *D3DTarget::get_buffer_handle(const GraphicContext &gc, const ElementArrayBuffer &buffer)
	{
		D3DElementArrayBufferProvider *provider = static_cast<D3DElementArrayBufferProvider *>(buffer.get_provider());
		return provider->get_buffer(static_cast<const D3DGraphicContextProvider *>(gc.get_provider())->get_window()->get_device());
	}

	ID3D11Buffer *D3DTarget::get_buffer_handle(const GraphicContext &gc, const UniformBuffer &buffer)
	{
		D3DUniformBufferProvider *provider = static_cast<D3DUniformBufferProvider *>(buffer.get_provider());
		return provider->get_buffer(static_cast<const D3DGraphicContextProvider *>(gc.get_provider())->get_window()->get_device());
	}

	ID3D11ShaderResourceView *D3DTarget::get_srv_handle(const GraphicContext &gc, const Texture &texture)
	{
		D3DTextureProvider *provider = static_cast<D3DTextureProvider *>(texture.get_provider());
		return provider->get_srv(static_cast<const D3DGraphicContextProvider *>(gc.get_provider())->get_window()->get_device());
	}

	ID3D11Device *D3DTarget::get_device_handle(const GraphicContext &gc)
	{
		const D3DGraphicContextProvider *provider = static_cast<const D3DGraphicContextProvider *>(gc.get_provider());
		return provider->get_window()->get_device();
	}

	ID3D11DeviceContext *D3DTarget::get_device_context_handle(const GraphicContext &gc)
	{
		const D3DGraphicContextProvider *provider = static_cast<const D3DGraphicContextProvider *>(gc.get_provider());
		return provider->get_window()->get_device_context();
	}

	IDXGISwapChain *D3DTarget::get_swap_chain_handle(const GraphicContext &gc)
	{
		const D3DGraphicContextProvider *provider = static_cast<const D3DGraphicContextProvider *>(gc.get_provider());
		return provider->get_window()->get_swap_chain();
	}

	ID3D11RenderTargetView *D3DTarget::get_back_buffer_rtv_handle(const GraphicContext &gc)
	{
		const D3DGraphicContextProvider *provider = static_cast<const D3DGraphicContextProvider *>(gc.get_provider());
		return provider->get_window()->get_back_buffer_rtv();
	}

	void D3DTarget::throw_if_failed(const char *text, HRESULT result)
	{
		if (FAILED(result))
		{
			switch (result)
			{
			case D3D11_ERROR_FILE_NOT_FOUND:
				throw Exception(string_format("%1: D3D11_ERROR_FILE_NOT_FOUND", text));
			case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS:
				throw Exception(string_format("%1: D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS", text));
			case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS:
				throw Exception(string_format("%1: D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS", text));
			case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD:
				throw Exception(string_format("%1: D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD", text));
				//case D3DERR_INVALIDCALL:
				//	throw Exception(string_format("%1: D3DERR_INVALIDCALL", text));
				//case D3DERR_WASSTILLDRAWING:
				//	throw Exception(string_format("%1: D3DERR_WASSTILLDRAWING", text));
			case E_FAIL:
				throw Exception(string_format("%1: E_FAIL", text));
			case E_INVALIDARG:
				throw Exception(string_format("%1: E_INVALIDARG", text));
			case E_OUTOFMEMORY:
				throw Exception(string_format("%1: E_OUTOFMEMORY", text));
			default:
				throw Exception(text);
			}
		}
	}
}
