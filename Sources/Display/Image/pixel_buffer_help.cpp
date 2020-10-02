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
**    Mark Page
*/

#include "Display/precomp.h"
#include "API/Display/Image/pixel_buffer_help.h"

namespace clan
{
	PixelBuffer PixelBufferHelp::add_border(const PixelBuffer &pb, int border_size, const Rect &rect)
	{
		if (rect.left < 0 || rect.top < 0 || rect.right > pb.get_width() || rect.bottom > pb.get_height())
			throw Exception("Rectangle passed to PixelBufferHelp::add_border() out of bounds");

		if (border_size < 0)
		{
			border_size = 0;
		}

		int old_width = pb.get_width();
		int old_height = pb.get_height();

		int new_width = rect.get_width() + border_size * 2;
		int new_height = rect.get_height() + border_size * 2;

		// Convert pixel buffer if in an unsupported format
		PixelBuffer work_buffer;
		PixelBuffer work_pb = pb;
		if (work_pb.get_format() != TextureFormat::rgba8)
		{
			work_buffer = pb.to_format(TextureFormat::rgba8);
			work_pb = work_buffer;
		}

		PixelBuffer new_pb(new_width, new_height, work_pb.get_format(), nullptr);

		int old_pitch = work_pb.get_pitch();
		int new_pitch = new_pb.get_pitch();

		int32_t *actual_src_data = (int32_t *)work_pb.get_data();
		actual_src_data += (rect.top * old_pitch) / 4;
		actual_src_data += rect.left;

		int32_t *actual_dest_data = (int32_t *)new_pb.get_data();

		for (int ypos = 0; ypos < new_height; ypos++)
		{
			int real_ypos = ypos - border_size;
			if (real_ypos < 0)
				real_ypos = 0;

			if (real_ypos >= old_height)
				real_ypos = old_height - 1;

			int32_t *src_data = actual_src_data;
			src_data += (old_pitch * real_ypos) / 4;

			int32_t *dest_data = actual_dest_data;
			dest_data += (new_pitch * ypos) / 4;

			for (int xpos = 0; xpos < new_width; xpos++)
			{
				int real_xpos = xpos - border_size;
				if (real_xpos < 0)
					real_xpos = 0;

				if (real_xpos >= old_width)
					real_xpos = old_width - 1;

				dest_data[xpos] = src_data[real_xpos];
			}
		}
		return new_pb;
	}
}
