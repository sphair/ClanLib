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

#pragma once

#include "API/Display/Render/texture.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Render/blend_state.h"
#include "API/Display/Render/render_batcher.h"
#include "API/Display/Render/texture_2d.h"
#include "render_batch_buffer.h"
#include "API/Display/2D/pen.h"
#include "API/Display/2D/path.h"
#include "API/Display/2D/brush.h"
#include "path_fill_renderer.h"
#include "path_stroke_renderer.h"

namespace clan
{
	class RenderBatchBuffer;

	class RenderBatchPath : public RenderBatcher
	{
	public:
		RenderBatchPath(GraphicContext &gc, RenderBatchBuffer *batch_buffer);

		void fill(Canvas &canvas, const Path &path, const Brush &brush);
		void stroke(Canvas &canvas, const Path &path, const Pen &pen);

	private:
		void render(const Path &path, PathRenderer *renderer);

		int set_batcher_active(Canvas &canvas);
		void flush(GraphicContext &gc) override;
		void matrix_changed(const Mat4f &modelview, const Mat4f &projection, TextureImageYAxis image_yaxis, float pixel_ratio) override;

		inline Pointf to_position(const clan::Pointf &point) const;

		Mat4f modelview_matrix;
		RenderBatchBuffer *batch_buffer;

		PathFillRenderer fill_renderer;
		PathStrokeRenderer stroke_renderer;
	};
}
