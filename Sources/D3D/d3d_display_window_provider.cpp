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
**	  Harry Storbacka
*/

#include "D3D/precomp.h"
#include "d3d_display_window_provider.h"
#include "d3d_target_provider.h"
#include "d3d_graphic_context_provider.h"
#include "API/Core/Math/rect.h"
#include "API/Core/Text/logger.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/Window/display_window_description.h"
#include "API/D3D/d3d_target.h"
#include "Display/Platform/Win32/cursor_provider_win32.h"
#include "d3d_share_list.h"

namespace clan
{
std::recursive_mutex D3DDisplayWindowProvider::d3d11_mutex;
HMODULE D3DDisplayWindowProvider::d3d11_dll = 0;
D3DDisplayWindowProvider::FuncD3D11CreateDeviceAndSwapChain D3DDisplayWindowProvider::d3d11_createdeviceandswapchain = 0;

/////////////////////////////////////////////////////////////////////////////
// D3DDisplayWindowProvider Construction:

D3DDisplayWindowProvider::D3DDisplayWindowProvider()
: site(0), use_fake_front_buffer(false), current_interval_setting(1)
{
	window.set_allow_drop_shadow(true);
	window.func_on_resized() = bind_member(this, &D3DDisplayWindowProvider::on_window_resized);
}

D3DDisplayWindowProvider::~D3DDisplayWindowProvider()
{
	if (device)
		D3DShareList::device_destroyed(device);
}

/////////////////////////////////////////////////////////////////////////////
// D3DDisplayWindowProvider Attributes:

Rect D3DDisplayWindowProvider::get_geometry() const
{
	return window.get_geometry();
}

Rect D3DDisplayWindowProvider::get_viewport() const
{
	return window.get_viewport();
}

float D3DDisplayWindowProvider::get_pixel_ratio() const
{
	return window.get_pixel_ratio();
}

bool D3DDisplayWindowProvider::has_focus() const
{
	return window.has_focus();
}

bool D3DDisplayWindowProvider::is_minimized() const
{
	return window.is_minimized();
}

bool D3DDisplayWindowProvider::is_maximized() const
{
	return window.is_maximized();
}

bool D3DDisplayWindowProvider::is_visible() const
{
	return window.is_visible();
}

GraphicContext& D3DDisplayWindowProvider::get_gc()
{
	return gc;
}

InputContext D3DDisplayWindowProvider::get_ic()
{
	return window.get_ic();
}

DisplayWindowHandle D3DDisplayWindowProvider::get_handle() const
{
	DisplayWindowHandle handle;
	handle.hwnd = window.get_hwnd();
	return handle;
}

bool D3DDisplayWindowProvider::is_clipboard_text_available() const
{
	return window.is_clipboard_text_available();
}

bool D3DDisplayWindowProvider::is_clipboard_image_available() const
{
	return window.is_clipboard_image_available();
}

Size D3DDisplayWindowProvider::get_minimum_size(bool client_area) const
{
	return window.get_minimum_size(client_area);
}

Size D3DDisplayWindowProvider::get_maximum_size( bool client_area/*=false*/ ) const
{
	return window.get_maximum_size(client_area);
}

std::string D3DDisplayWindowProvider::get_title() const
{
	return window.get_title();
}

/////////////////////////////////////////////////////////////////////////////
// D3DDisplayWindowProvider Operations:

Point D3DDisplayWindowProvider::client_to_screen(const Point &client)
{
	return window.client_to_screen(client);
}

Point D3DDisplayWindowProvider::screen_to_client(const Point &screen)
{
	return window.screen_to_client(screen);
}

void D3DDisplayWindowProvider::capture_mouse(bool capture)
{
	window.capture_mouse(capture);
}

void D3DDisplayWindowProvider::create(DisplayWindowSite *new_site, const DisplayWindowDescription &description)
{
	site = new_site;

	if (device)
		D3DShareList::device_destroyed(device);
	info_queue.clear();
	debug.clear();
	back_buffer_rtv.clear();
	fake_front_buffer.clear();
	back_buffer.clear();
	swap_chain.clear();
	device_context.clear();
	device.clear();

	window.create(site, description);

	use_fake_front_buffer = description.is_update_supported();

	D3D_FEATURE_LEVEL request_levels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };
	DXGI_SWAP_CHAIN_DESC swap_chain_description;
	swap_chain_description.BufferCount = description.get_flipping_buffers();
	swap_chain_description.BufferDesc.Width = 0;
	swap_chain_description.BufferDesc.Height = 0;
	swap_chain_description.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_description.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_description.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	swap_chain_description.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_description.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swap_chain_description.SampleDesc.Count = 1;
	swap_chain_description.SampleDesc.Quality = 0;
	swap_chain_description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_description.OutputWindow = window.get_hwnd();
	swap_chain_description.Windowed = TRUE; // Seems the documentation wants us to call IDXGISwapChain::SetFullscreenState afterwards
	swap_chain_description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_description.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	bool debug_mode = false; // To do: fetch this from DisplayWindowDescription using same method as clanGL (or maybe promote debug flag to clanDisplay?)

	UINT device_flags = 0;
	if (debug_mode)
		device_flags |= D3D11_CREATE_DEVICE_DEBUG;

	std::unique_lock<std::recursive_mutex> mutex_lock(d3d11_mutex);
	if (d3d11_dll == 0)
	{
		d3d11_dll = LoadLibrary(L"d3d11.dll");
		if (d3d11_dll == 0)
			throw Exception("Unable to load d3d11.dll");

		try
		{
			d3d11_createdeviceandswapchain = reinterpret_cast<FuncD3D11CreateDeviceAndSwapChain>(GetProcAddress(d3d11_dll, "D3D11CreateDeviceAndSwapChain"));
			if (d3d11_createdeviceandswapchain == 0)
				throw Exception("D3D11CreateDeviceAndSwapChain function not found!");

		}
		catch (...)
		{
			CloseHandle(d3d11_dll);
			d3d11_dll = 0;
			d3d11_createdeviceandswapchain = 0;
			throw;
		}
	}
	HRESULT result = d3d11_createdeviceandswapchain(
		0,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		device_flags,
		request_levels, 3,
		D3D11_SDK_VERSION,
		&swap_chain_description,
		swap_chain.output_variable(),
		device.output_variable(),
		&feature_level,
		device_context.output_variable());
	D3DTarget::throw_if_failed("D3D11CreateDeviceAndSwapChain failed", result);

	if (debug_mode)
	{
		result = device->QueryInterface(__uuidof(ID3D11Debug), (void**)debug.output_variable());
		if (FAILED(result))
			debug.clear(); // No debug info available.  Should this throw an exception instead?
		result = device->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)info_queue.output_variable());
		if (FAILED(result))
			info_queue.clear(); // No debug messages available.
	}

	// Disable mouse lag (no, 3 frames rendered ahead is NOT a good default Microsoft):
	ComPtr<IDXGIDevice1> dxgi_device;
	result = swap_chain->GetDevice(__uuidof(IDXGIDevice1), (void**)dxgi_device.output_variable());
	D3DTarget::throw_if_failed("Unable to retrieve IDXGIDevice1 from swap chain", result);
	dxgi_device->SetMaximumFrameLatency(1);

	create_swap_chain_buffers();

	// Prevent DXGI from responding to an alt-enter sequence.
	ComPtr<IDXGIAdapter> dxgi_adapter;
	result = dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void **)dxgi_adapter.output_variable());
	if (SUCCEEDED(result))
	{
		ComPtr<IDXGIFactory> dxgi_factory;
		result = dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void **)dxgi_factory.output_variable());
		if (SUCCEEDED(result))
		{
			dxgi_factory->MakeWindowAssociation(window.get_hwnd(), DXGI_MWA_NO_ALT_ENTER);
		}
	}

	gc = GraphicContext(new D3DGraphicContextProvider(this, description));

	if (description.is_fullscreen())
		swap_chain->SetFullscreenState(TRUE, 0);

	D3DGraphicContextProvider *d3d_gc = static_cast<D3DGraphicContextProvider*>(gc.get_provider());
	d3d_gc->standard_programs = StandardPrograms(gc);
}

void D3DDisplayWindowProvider::show_system_cursor()
{
	window.show_system_cursor();
}

CursorProvider *D3DDisplayWindowProvider::create_cursor(const CursorDescription &cursor_description)
{
	return new CursorProvider_Win32(cursor_description);
}

void D3DDisplayWindowProvider::set_cursor(CursorProvider *cursor)
{
	window.set_cursor(static_cast<CursorProvider_Win32 *>(cursor));
}

void D3DDisplayWindowProvider::set_cursor(StandardCursor type)
{
	window.set_cursor(type);
}

void D3DDisplayWindowProvider::set_cursor_handle(HCURSOR cursor)
{
	window.set_cursor_handle(cursor);
}

void D3DDisplayWindowProvider::hide_system_cursor()
{
	window.hide_system_cursor();
}

void D3DDisplayWindowProvider::set_title(const std::string &new_title)
{
	window.set_title(new_title);
}

void D3DDisplayWindowProvider::set_position(const Rect &pos, bool client_area)
{
	window.set_position(pos, client_area);
}

void D3DDisplayWindowProvider::set_size(int width, int height, bool client_area)
{
	window.set_size(width, height, client_area);
}

void D3DDisplayWindowProvider::set_minimum_size( int width, int height, bool client_area )
{
	window.set_minimum_size(width,height,client_area);
}

void D3DDisplayWindowProvider::set_maximum_size( int width, int height, bool client_area )
{
	window.set_maximum_size(width,height,client_area);
}

void D3DDisplayWindowProvider::set_enabled(bool enable)
{
	window.set_enabled(enable);
}

void D3DDisplayWindowProvider::minimize()
{
	window.minimize();
}

void D3DDisplayWindowProvider::restore()
{
	window.restore();
}

void D3DDisplayWindowProvider::maximize()
{
	window.maximize();
}

void D3DDisplayWindowProvider::toggle_fullscreen()
{
	window.toggle_fullscreen();
}


void D3DDisplayWindowProvider::show(bool activate)
{
	window.show(activate);
}

void D3DDisplayWindowProvider::hide()
{
	window.hide();
}

void D3DDisplayWindowProvider::bring_to_front()
{
	window.bring_to_front();
}

void D3DDisplayWindowProvider::flip(int interval)
{
	if (use_fake_front_buffer)
		device_context->CopyResource(fake_front_buffer, back_buffer);

	//D3DGraphicContextProvider *gc_provider = static_cast<D3DGraphicContextProvider*>(gc.get_provider());
	if (interval != -1)
		current_interval_setting = interval;
	swap_chain->Present(current_interval_setting, 0);
	log_debug_messages();
}

void D3DDisplayWindowProvider::update(const Rect &rect)
{
	if (use_fake_front_buffer)
	{
		D3D11_BOX src_box;
		src_box.left = rect.left;
		src_box.right = rect.right;
		src_box.top = rect.top;
		src_box.bottom = rect.bottom;
		src_box.front = 0;
		src_box.back = 1;
		device_context->CopySubresourceRegion(fake_front_buffer, 0, rect.left, rect.top, 0, back_buffer, 0, &src_box);
		device_context->CopyResource(back_buffer, fake_front_buffer);
		swap_chain->Present(current_interval_setting, 0);
		log_debug_messages();
	}
}

void D3DDisplayWindowProvider::set_clipboard_text(const std::string &text)
{
	window.set_clipboard_text(text);
}

void D3DDisplayWindowProvider::set_clipboard_image(const PixelBuffer &buf)
{
	window.set_clipboard_image(buf);
}

std::string D3DDisplayWindowProvider::get_clipboard_text() const
{
	return window.get_clipboard_text();
}

PixelBuffer D3DDisplayWindowProvider::get_clipboard_image() const
{
	return window.get_clipboard_image();
}

void D3DDisplayWindowProvider::request_repaint(const Rect &rect)
{
	window.request_repaint(rect);
}

void D3DDisplayWindowProvider::set_large_icon(const PixelBuffer &image)
{
	window.set_large_icon(image);
}

void D3DDisplayWindowProvider::set_small_icon(const PixelBuffer &image)
{
	window.set_small_icon(image);
}

void D3DDisplayWindowProvider::enable_alpha_channel(const Rect &blur_rect)
{
	window.enable_alpha_channel(blur_rect);
}

void D3DDisplayWindowProvider::extend_frame_into_client_area(int left, int top, int right, int bottom)
{
	window.extend_frame_into_client_area(left, top, right, bottom);
}

void D3DDisplayWindowProvider::validate_context()
{
	if (debug)
	{
		HRESULT result = debug->ValidateContext(device_context);
		if (FAILED(result) || result == S_FALSE)
		{
			log_event("d3d", "Direct3D context not valid before draw call!");
		}
	}
}

void D3DDisplayWindowProvider::set_pixel_ratio(float ratio)
{
	window.set_pixel_ratio(ratio);
}

/////////////////////////////////////////////////////////////////////////////
// D3DDisplayWindowProvider Implementation:

void D3DDisplayWindowProvider::create_swap_chain_buffers()
{
	release_swap_chain_buffers();

	HRESULT result = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)back_buffer.output_variable());
	D3DTarget::throw_if_failed("Unable to get back buffer", result);

	if (use_fake_front_buffer)
	{
		D3D11_TEXTURE2D_DESC texture_desc;
		back_buffer->GetDesc(&texture_desc);
		texture_desc.Usage = D3D11_USAGE_DEFAULT;
		texture_desc.CPUAccessFlags = 0;
		texture_desc.MiscFlags = 0;
		texture_desc.BindFlags = 0;
		result = device->CreateTexture2D(&texture_desc, 0, fake_front_buffer.output_variable());
		D3DTarget::throw_if_failed("Unable to create front buffer", result);
	}

	result = device->CreateRenderTargetView(back_buffer, 0, back_buffer_rtv.output_variable());
	D3DTarget::throw_if_failed("Unable to create back buffer render target view", result);
}

void D3DDisplayWindowProvider::release_swap_chain_buffers()
{
	back_buffer_rtv.clear();
	fake_front_buffer.clear();
	back_buffer.clear();
}

void D3DDisplayWindowProvider::on_window_resized()
{
	D3DGraphicContextProvider *gc_provider = static_cast<D3DGraphicContextProvider*>(gc.get_provider());

	if (gc_provider)
	{
		gc_provider->begin_resize_swap_chain();
		release_swap_chain_buffers();

		HRESULT result = swap_chain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
		if (gc.get_provider())
			((D3DGraphicContextProvider *) gc.get_provider())->on_window_resized();

		create_swap_chain_buffers();

		gc_provider->end_resize_swap_chain();

	}
}

void D3DDisplayWindowProvider::log_debug_messages()
{
	if (info_queue)
	{
		UINT64 message_count = info_queue->GetNumStoredMessagesAllowedByRetrievalFilter();
		std::vector<char> buffer;
		for (UINT64 i = 0; i < message_count; i++)
		{
			SIZE_T length = 0;
			HRESULT result = info_queue->GetMessage(i, 0, &length);
			if (FAILED(result))
				continue;

			buffer.resize(max(buffer.size(), length));
			D3D11_MESSAGE *message = reinterpret_cast<D3D11_MESSAGE*>(buffer.empty() == false ? &buffer[0] : 0);

			result = info_queue->GetMessage(i, message, &length);
			if (FAILED(result))
				continue;

			std::string text(message->pDescription, message->DescriptionByteLength);
			log_event("d3d", "%4 (id=%1, category=%2, severity=%3)", message->ID, message->Category, message->Severity, text);
		}
		info_queue->ClearStoredMessages();
	}
}

}
