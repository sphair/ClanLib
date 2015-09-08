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
**    Harry Storbacka
*/

#pragma once

#include "gl3_graphic_context_provider.h"
#include "API/Display/Render/texture_2d.h"
#include "API/Display/TargetProviders/frame_buffer_provider.h"
#include "API/Core/System/disposable_object.h"
#include "API/Display/Render/render_buffer.h"

namespace clan
{
	class GL3FrameBufferProvider : public FrameBufferProvider, DisposableObject
	{
	public:
		GL3FrameBufferProvider(GL3GraphicContextProvider *gc_provider);
		~GL3FrameBufferProvider();

		GLuint get_handle();
		Size get_size() const override;
		FrameBufferBindTarget get_bind_target() const override;
		GL3GraphicContextProvider *get_gc_provider() { return gc_provider; }

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

		void check_framebuffer_complete();
		void bind_framebuffer(bool write_only);

	private:
		void on_dispose() override;
		static std::string get_error_message(int error_code);

		GLuint decode_texture_subtype(TextureSubtype subtype);
		void detach_object(GLenum opengl_attachment);

		// Returns true if the object was replaced
		bool attach_object(GLenum opengl_attachment, const Texture &texture, int level, int zoffset, GLuint texture_target);
		bool attach_object(GLenum opengl_attachment, const RenderBuffer &render_buffer);

		int decode_internal_attachment_offset(GLenum opengl_attachment);

		static const int depth_attachment_offset = 0;
		static const int stencil_attachment_offset = 1;
		static const int depth_stencil_attachment_offset = 2;
		static const int color_attachment_offset = 3;
		static const int max_color_attachments = 16;
		static const int num_attachment_offsets = color_attachment_offset + max_color_attachments;

		Texture attached_textures[num_attachment_offsets];
		RenderBuffer attached_renderbuffers[num_attachment_offsets];

		int count_color_attachments = 0;
		GLuint handle = 0;
		FrameBufferBindTarget bind_target = framebuffer_draw;

		GL3GraphicContextProvider *gc_provider;
	};

	class FrameBufferStateTracker
	{
	public:
		FrameBufferStateTracker(FrameBufferBindTarget target, GLuint handle, GL3GraphicContextProvider *gc_provider);
		~FrameBufferStateTracker();

	private:
		FrameBufferBindTarget bind_target;
		GLint last_bound;
		bool handle_and_bound_equal;
	};
}
