/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
**    Mark Page
*/

#pragma once

#include "API/Display/Render/render_batcher.h"
#include "API/Display/Render/texture.h"
#include "API/Display/Render/graphic_context.h"

struct CL_Surface_DrawParams1;

class CL_RenderBatcherSprite : public CL_RenderBatcher
{
public:
	virtual void draw_sprite(CL_GraphicContext &gc, const CL_Surface_DrawParams1 *params, const CL_Texture &texture) = 0;
	virtual void draw_image(CL_GraphicContext &gc, const CL_Rectf &src, const CL_Rectf &dest, const CL_Colorf &color, const CL_Texture &texture) = 0;
	virtual void draw_glyph_subpixel(CL_GraphicContext &gc, const CL_Rectf &src, const CL_Rectf &dest, const CL_Colorf &color, const CL_Texture &texture) = 0;
	virtual void fill(CL_GraphicContext &gc, float x1, float y1, float x2, float y2, const CL_Colorf &color) = 0;
};
