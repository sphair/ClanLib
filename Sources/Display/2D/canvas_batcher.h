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
**    Mark Page
*/

#pragma once

#include "API/Display/Render/graphic_context.h"
#include "Display/2D/render_batch_buffer.h"
#include "Display/2D/render_batch_triangle.h"
#include "Display/2D/render_batch_line.h"
#include "Display/2D/render_batch_line_texture.h"
#include "Display/2D/render_batch_point.h"
#include "Display/2D/render_batch_path.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/Window/display_window.h"

namespace clan
{
	class CanvasBatcher_Impl;

	class CanvasBatcher
	{
	public:
		CanvasBatcher();
		CanvasBatcher(GraphicContext &gc);
		~CanvasBatcher();

		/// \brief Returns true if this object is invalid.
		bool is_null() const { return !impl; }

		void flush();
		bool set_batcher(GraphicContext &gc, RenderBatcher *batcher);
		void stop_batcher(GraphicContext& gc);
		void update_batcher_matrix(GraphicContext &gc, const Mat4f &modelview, const Mat4f &projection, TextureImageYAxis image_yaxis);

		RenderBatchTriangle *get_triangle_batcher();
		RenderBatchLine *get_line_batcher();
		RenderBatchLineTexture *get_line_texture_batcher();
		RenderBatchPoint *get_point_batcher();
		RenderBatchPath *get_path_batcher();

	private:
		std::shared_ptr<CanvasBatcher_Impl> impl;
	};
}
