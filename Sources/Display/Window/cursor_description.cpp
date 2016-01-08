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
**    Kenneth Gangstoe
**    Magnus Norddahl
*/

#include "Display/precomp.h"
#include "API/Core/IOData/file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/Display/Window/cursor_description.h"
#include "API/Display/ImageProviders/provider_factory.h"
#include "cursor_description_impl.h"

namespace clan
{
	CursorDescription::CursorDescription()
		: impl(std::make_shared<CursorDescription_Impl>())
	{
	}

	CursorDescription::CursorDescription(const CursorDescription &copy)
		: impl(copy.impl)
	{
	}

	CursorDescription::~CursorDescription()
	{
	}

	const std::vector<CursorDescriptionFrame> &CursorDescription::get_frames() const
	{
		return impl->frames;
	}

	Point CursorDescription::get_hotspot() const
	{
		return impl->hotspot;
	}

	CursorDescription &CursorDescription::operator =(const CursorDescription &copy)
	{
		impl = copy.impl;
		return *this;
	}

	void CursorDescription::add_frame(const PixelBuffer &pixelbuffer)
	{
		impl->frames.push_back(CursorDescriptionFrame(pixelbuffer, Rect(0, 0, pixelbuffer.get_width(), pixelbuffer.get_height())));
	}

	void CursorDescription::add_frame(const std::string &filename, FileSystem &fs, const ImageImportDescription &import_desc)
	{
		PixelBuffer image = ImageProviderFactory::load(filename, fs, "");
		image = import_desc.process(image);
		add_frame(image);
	}

	void CursorDescription::add_frame(const std::string &fullname, const ImageImportDescription &import_desc)
	{
		std::string path = PathHelp::get_fullpath(fullname, PathHelp::path_type_file);
		std::string filename = PathHelp::get_filename(fullname, PathHelp::path_type_file);
		FileSystem vfs(path);
		add_frame(filename, vfs, import_desc);
	}

	void CursorDescription::add_frame(IODevice &file, const std::string &image_type, const ImageImportDescription &import_desc)
	{
		PixelBuffer image = ImageProviderFactory::load(file, image_type);
		image = import_desc.process(image);
		add_frame(image);
	}

	void CursorDescription::add_gridclipped_frames(
		const PixelBuffer &pixelbuffer,
		int xpos, int ypos,
		int width, int height,
		int xarray, int yarray,
		int array_skipframes,
		int xspace, int yspace)
	{
		int ystart = ypos;
		for (int y = 0; y < yarray; y++)
		{
			int xstart = xpos;
			for (int x = 0; x < xarray; x++)
			{
				if (y == yarray - 1 && x >= xarray - array_skipframes)
					break;

				if (xstart + width > pixelbuffer.get_width() || ystart + height > pixelbuffer.get_height())
					throw Exception("add_gridclipped_frames: Outside pixelbuffer bounds");

				impl->frames.push_back(CursorDescriptionFrame(pixelbuffer, Rect(xstart, ystart, xstart + width, ystart + height)));
				xstart += width + xspace;
			}
			ystart += height + yspace;
		}
	}

	void CursorDescription::add_alphaclipped_frames(
		const PixelBuffer &pixelbuffer,
		int xpos, int ypos,
		float trans_limit)
	{
		PixelBuffer alpha_buffer = pixelbuffer.to_format(tf_rgba8);

		int begin = 0;
		bool prev_trans = true;

		int alpha_width = alpha_buffer.get_width();
		int alpha_height = alpha_buffer.get_height();
		bool found_opaque = false;
		bool found_trans = false;

		std::vector<int> opaque_row_vector;
		opaque_row_vector.resize(alpha_width);

		int *opaque_row = &(opaque_row_vector[0]);
		memset(opaque_row, 0, alpha_width*sizeof(int));

		int cut_top = ypos;
		int cut_bottom = alpha_height;

		char *data = (char *)alpha_buffer.get_data();

		for (int y = ypos; y < alpha_height; y++)
		{
			bool opaque_line = false;
			Vec4ub *line = (Vec4ub *)(data + alpha_buffer.get_pitch()*y);
			for (int x = 0; x < alpha_width; x++)
			{
				if (line[x].a > trans_limit * 255)
				{
					opaque_row[x] = 1;
					opaque_line = true;
					found_opaque = true;
				}
			}

			if (opaque_line == false) // cut something of top or bottom
			{
				if (found_opaque)
				{
					cut_bottom--;
					found_trans = true;
				}
				else
					cut_top++;
			}
			else if (found_trans)
			{
				found_trans = false;
				cut_bottom = alpha_height;
			}
		}

		if (cut_top >= cut_bottom)
			throw Exception("add_alphaclipped_frames: Image contained only alpha!");

		for (int x = xpos; x < alpha_width; x++)
		{
			if (opaque_row[x] && prev_trans)
			{
				begin = x;
				prev_trans = false;
			}
			else if (!opaque_row[x] && !prev_trans)
			{
				impl->frames.push_back(
					CursorDescriptionFrame(pixelbuffer, Rect(begin, cut_top, x + 1, cut_bottom)));

				prev_trans = true;
			}
		}

		if (!prev_trans)
		{
			impl->frames.push_back(
				CursorDescriptionFrame(pixelbuffer, Rect(begin, cut_top, alpha_width, cut_bottom)));
		}
	}

	void CursorDescription::add_alphaclipped_frames_free(
		const PixelBuffer &pixelbuffer,
		int xpos, int ypos,
		float trans_limit)
	{
		PixelBuffer alpha_buffer = pixelbuffer.to_format(tf_rgba8);

		int width = alpha_buffer.get_width();
		int height = alpha_buffer.get_height();

		std::vector<int> explored_vector;
		explored_vector.resize(width * height);
		int *explored = &(explored_vector[0]);
		memset(explored, 0, width * height * sizeof(int));

		Vec4ub *data = alpha_buffer.get_data<Vec4ub>();
		int x1, y1, x2, y2;
		bool more;

		for (int y = ypos; y < height; y++)
		{
			for (int x = xpos; x < width; x++)
			{
				if (explored[y*width + x] == 1) continue;
				explored[y*width + x] = 1;
				if (data[y*width + x].a <= trans_limit * 255)
					continue;

				// Initialize the bounding box to the current pixel
				x1 = x2 = x;
				y1 = y2 = y;
				more = true;
				while (more)
				{
					// Assume that there are NO opaque pixels around the current bounding box
					more = false;

					// Scan under the current bounding box and see if there any non-transparent pixels
					for (int i = x1; i <= x2; i++)
					{
						if (y2 + 1 < height)
						{
							explored[(y2 + 1)*width + i] = 1;
							if (data[(y2 + 1)*width + i].a > trans_limit * 255)
							{
								more = true;
								y2 = y2 + 1;
							}
						}
					}

					// Now scan the left and right sides of the current bounding box
					for (int j = y1; j <= y2; j++)
					{
						// Scan the right side
						if (x2 + 1 < width)
						{
							explored[j*width + x2 + 1] = 1;
							if (data[j*width + x2 + 1].a > trans_limit * 255)
							{
								more = true;
								x2 = x2 + 1;
							}
						}
						// Scan the left side
						if (x1 - 1 >= 0)
						{
							explored[j*width + x1 - 1] = 1;
							if (data[j*width + x1 - 1].a > trans_limit * 255)
							{
								more = true;
								x1 = x1 - 1;
							}
						}
					}
				}

				// Mark all pixels in the bounding box as being explored
				for (int i = x1; i <= x2; i++)
				{
					for (int j = y1; j <= y2; j++)
					{
						explored[j*width + i] = 1;
					}
				}

				impl->frames.push_back(CursorDescriptionFrame(pixelbuffer, Rect(x1, y1, x2, y2)));
			}
		}
	}

	void CursorDescription::set_frame_delay(int frame, double delay)
	{
		impl->frames[frame].delay = delay;
	}

	void CursorDescription::set_hotspot(const Point &hotspot)
	{
		impl->hotspot = hotspot;
	}
}
