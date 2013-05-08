/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
**    Kenneth Gangstoe
**    Mark Page
*/

#include "CSSLayout/precomp.h"
#include "clan_image_stretch.h"

namespace clan
{

void ClanImageStretch::draw_image(Canvas &canvas, const Rect &rect, Image &image, int sizing_left, int sizing_top, int sizing_right, int sizing_bottom)
{
	std::vector<ImagePart> image_parts = calc_stretch_image_parts(rect, image, sizing_left, sizing_top, sizing_right, sizing_bottom);
	size_t size = image_parts.size();
	for (size_t index = 0; index < size; index++)
		image.draw(canvas, image_parts[index].source_rect, image_parts[index].dest_rect);
}

std::vector<ClanImageStretch::ImagePart> ClanImageStretch::calc_stretch_image_parts(const Rect &dest, Image &sprite, int sizing_left, int sizing_top, int sizing_right, int sizing_bottom)
{
	int src_width = sprite.get_width();
	int src_height = sprite.get_height();

	int src_grid_x[4];
	int src_grid_y[4];
	int dest_grid_x[4];
	int dest_grid_y[4];

	int rows = 0;
	int cols = 0;

	src_grid_x[cols++] = 0;
	src_grid_x[cols++] = sizing_left;
	src_grid_x[cols++] = src_width - sizing_right;
	src_grid_x[cols++] = src_width;

	src_grid_y[rows++] = 0;
	src_grid_y[rows++] = sizing_top;
	src_grid_y[rows++] = src_height - sizing_bottom;
	src_grid_y[rows++] = src_height;

	rows = 0;
	cols = 0;

	dest_grid_x[cols++] = dest.left;
	dest_grid_x[cols++] = dest.left + sizing_left;
	dest_grid_x[cols++] = dest.right - sizing_right;
	dest_grid_x[cols++] = dest.right;

	dest_grid_y[rows++] = dest.top;
	dest_grid_y[rows++] = dest.top + sizing_top;
	dest_grid_y[rows++] = dest.bottom - sizing_bottom;
	dest_grid_y[rows++] = dest.bottom;

	std::vector<ImagePart> image_parts;
	for (int y = 0; y < rows-1; y++)
	{
		for (int x = 0; x < cols-1; x++)
		{
			ImagePart part;
			part.source_rect = Rect(src_grid_x[x], src_grid_y[y], src_grid_x[x+1], src_grid_y[y+1]);
			part.dest_rect = Rect(dest_grid_x[x], dest_grid_y[y], dest_grid_x[x+1], dest_grid_y[y+1]);
			if (part.source_rect.get_width() > 0 && part.source_rect.get_height() > 0 &&
				part.dest_rect.get_width() > 0 && part.dest_rect.get_height() > 0)
			{
				image_parts.push_back(part);
			}
		}
	}

	return image_parts;
}

}
