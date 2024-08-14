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
**    Harry Storbacka
**    Mark Page
*/

#pragma once

#include <map>

#include "gl1_graphic_context_provider.h"
#include "API/Display/Render/texture_2d.h"
#include "API/Display/TargetProviders/frame_buffer_provider.h"
#include "API/Core/System/disposable_object.h"

#include "pbuffer.h"

namespace clan
{
	class GL1FrameBufferProvider : public FrameBufferProvider, DisposableObject
	{
	public:
		GL1FrameBufferProvider(GL1GraphicContextProvider *gc_provider);
		~GL1FrameBufferProvider() override;

		Size get_size() const override;
		FrameBufferBindTarget get_bind_target() const override;

		void attach_color(int attachment_index, const RenderBuffer &render_buffer) override;
		void attach_color(int attachment_index, const Texture1D &texture, int level) override;
		void attach_color(int attachment_index, const Texture1DArray &texture, int array_index, int level) override;
		void attach_color(int attachment_index, const Texture2D &texture, int level) override;
		void attach_color(int attachment_index, const Texture2DArray &texture, int array_index, int level) override;
		void attach_color(int attachment_index, const Texture3D &texture, int depth, int level) override;
		void attach_color(int attachment_index, const TextureCube &texture, TextureSubtype subtype, int level) override;
		void detach_color(int attachment_index) override;

		void attach_stencil(const RenderBuffer &render_buffer) override;
		void attach_stencil(const Texture2D &texture, int level) override;
		void attach_stencil(const TextureCube &texture, TextureSubtype subtype, int level) override;
		void detach_stencil() override;

		void attach_depth(const RenderBuffer &render_buffer) override;
		void attach_depth(const Texture2D &texture, int level) override;
		void attach_depth(const TextureCube &texture, TextureSubtype subtype, int level) override;
		void detach_depth() override;

		void attach_depth_stencil(const RenderBuffer &render_buffer) override;
		void attach_depth_stencil(const Texture2D &texture, int level) override;
		void attach_depth_stencil(const TextureCube &texture, TextureSubtype subtype, int level) override;
		void detach_depth_stencil() override;

		void set_bind_target(FrameBufferBindTarget target) override;

		void set_active();
		void stop();
		void start();

		void set_state(OpenGLBlendState &new_blend);
		void set_state(OpenGLRasterizerState &new_rasterizer);
		void set_state(OpenGLDepthStencilState &new_depth_stencil);
		void set_state(GL1State &new_selected_state);

	private:
		void on_dispose() override;
		void sync_pbuffer();
		void sync_texture();
		void detach_all();

		mutable bool pbuffer_changed;
		struct WeakPtrCompare
		{
			bool operator()(const std::weak_ptr<Texture_Impl> &p1, const std::weak_ptr<Texture_Impl> &p2) const
			{
				return p1.lock() < p2.lock();
			}
		};
		std::map< std::weak_ptr<Texture_Impl>, PBuffer_GL1, WeakPtrCompare> texture_pbuffer_map;

		PBuffer_GL1 pbuffer;
		Texture2D selected_surface;
		GL1TextureProvider *selected_texture_provider;

		GL1State selected_state;

		GL1GraphicContextProvider *gc_provider;
	};
}
