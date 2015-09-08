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

#pragma once

#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/Render/graphic_context.h"
#include "Display/Platform/Win32/win32_window.h"
#include <mutex>

namespace clan
{
	class D3DDisplayWindowProvider : public DisplayWindowProvider
	{
	public:
		D3DDisplayWindowProvider();
		~D3DDisplayWindowProvider();

		Rect get_geometry() const override;
		Rect get_viewport() const override;
		float get_pixel_ratio() const override;

		bool has_focus() const;
		bool is_minimized() const;
		bool is_maximized() const;
		bool is_visible() const;
		bool is_fullscreen() const { return false; } // FIXME: real implementation
		std::string get_title() const;
		Size get_minimum_size(bool client_area = false) const;
		Size get_maximum_size(bool client_area = false) const;

		GraphicContext& get_gc();
		InputContext get_ic();

		DisplayWindowHandle get_handle() const override;

		bool is_clipboard_text_available() const;
		bool is_clipboard_image_available() const;
		std::string get_clipboard_text() const;
		PixelBuffer get_clipboard_image() const;

		const ComPtr<ID3D11Device> &get_device() const { return device; }
		const ComPtr<ID3D11DeviceContext> &get_device_context() const { return device_context; }
		D3D_FEATURE_LEVEL get_feature_level() const { return feature_level; }
		const ComPtr<IDXGISwapChain> &get_swap_chain() const { return swap_chain; }
		const ComPtr<ID3D11Texture2D> &get_back_buffer() const { return back_buffer; }
		const ComPtr<ID3D11RenderTargetView> &get_back_buffer_rtv() const { return back_buffer_rtv; }
		const ComPtr<ID3D11Debug> &get_debug() const { return debug; }
		const ComPtr<ID3D11InfoQueue> &get_info_queue() const { return info_queue; }

		Point client_to_screen(const Point &client);
		Point screen_to_client(const Point &screen);

		void capture_mouse(bool capture);

		void create(DisplayWindowSite *site, const DisplayWindowDescription &description);

		void show_system_cursor();
		CursorProvider *create_cursor(const CursorDescription &cursor_description);
		void set_cursor(CursorProvider *cursor);
		void set_cursor(StandardCursor type);
		void set_cursor_handle(HCURSOR cursor);
		void hide_system_cursor();

		void set_title(const std::string &new_title);
		void set_position(const Rect &pos, bool client_area);
		void set_size(int width, int height, bool client_area);
		void set_minimum_size(int width, int height, bool client_area);
		void set_maximum_size(int width, int height, bool client_area);
		void set_enabled(bool enable);

		void minimize();
		void restore();
		void maximize();
		void toggle_fullscreen();

		void show(bool activate);
		void hide();

		void bring_to_front();

		void flip(int interval);

		void update(const Rect &rect);

		void set_clipboard_text(const std::string &text);
		void set_clipboard_image(const PixelBuffer &buf);

		void request_repaint(const Rect &rect);

		void set_large_icon(const PixelBuffer &image);
		void set_small_icon(const PixelBuffer &image);

		void enable_alpha_channel(const Rect &blur_rect);
		void extend_frame_into_client_area(int left, int top, int right, int bottom);

		void validate_context();

		void set_pixel_ratio(float ratio) override;

	private:
		void create_swap_chain_buffers();
		void release_swap_chain_buffers();
		void on_window_resized();
		void log_debug_messages();

		Win32Window window;

		DisplayWindowSite *site;
		GraphicContext gc;

		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> device_context;
		D3D_FEATURE_LEVEL feature_level;
		ComPtr<IDXGISwapChain> swap_chain;
		ComPtr<ID3D11Debug> debug;
		ComPtr<ID3D11InfoQueue> info_queue;
		ComPtr<ID3D11Texture2D> back_buffer;
		ComPtr<ID3D11RenderTargetView> back_buffer_rtv;

		bool use_fake_front_buffer;
		ComPtr<ID3D11Texture2D> fake_front_buffer;

		int current_interval_setting;

		typedef HRESULT(WINAPI *FuncD3D11CreateDeviceAndSwapChain)(
			__in_opt IDXGIAdapter* pAdapter,
			D3D_DRIVER_TYPE DriverType,
			HMODULE Software,
			UINT Flags,
			__in_ecount_opt(FeatureLevels) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
			UINT FeatureLevels,
			UINT SDKVersion,
			__in_opt CONST DXGI_SWAP_CHAIN_DESC* pSwapChainDesc,
			__out_opt IDXGISwapChain** ppSwapChain,
			__out_opt ID3D11Device** ppDevice,
			__out_opt D3D_FEATURE_LEVEL* pFeatureLevel,
			__out_opt ID3D11DeviceContext** ppImmediateContext);

		static std::recursive_mutex d3d11_mutex;
		static HMODULE d3d11_dll;
		static FuncD3D11CreateDeviceAndSwapChain d3d11_createdeviceandswapchain;
	};
}
