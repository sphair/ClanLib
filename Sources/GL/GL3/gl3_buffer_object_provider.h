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
**    Mark Page
*/

#pragma once

#include "API/Display/TargetProviders/vertex_array_buffer_provider.h"
#include "API/Display/Render/graphic_context.h"
#include "API/GL/opengl.h"
#include "API/Core/System/disposable_object.h"

namespace clan
{
	class GraphicContext;
	class TransferBuffer;
	class GL3GraphicContextProvider;

	class GL3BufferObjectProvider : public DisposableObject
	{
	public:
		GL3BufferObjectProvider();
		~GL3BufferObjectProvider();
		void create(const void *data, int size, BufferUsage usage, GLenum new_binding, GLenum new_target);

		void *get_data();

		GLuint get_handle() const { return handle; }
		GLenum get_binding() const { return binding; }
		GLenum get_target() const { return target; }

		void lock(GraphicContext &gc, BufferAccess access);
		void unlock();
		void upload_data(GraphicContext &gc, int offset, const void *data, int size);

		void upload_data(GraphicContext &gc, const void *data, int size);
		void copy_from(GraphicContext &gc, TransferBuffer &buffer, int dest_pos, int src_pos, int size);
		void copy_to(GraphicContext &gc, TransferBuffer &buffer, int dest_pos, int src_pos, int size);

	private:
		void on_dispose() override;

		GLuint handle;
		GLenum binding;
		GLenum target;

		void *data_ptr;
		GraphicContext lock_gc;
	};
}
