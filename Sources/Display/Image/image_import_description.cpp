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
**    Mark Page
*/
	
#include "Display/precomp.h"
#include "API/Display/Image/image_import_description.h"
#include "API/Display/Image/pixel_buffer.h"
#include "image_import_description_impl.h"

namespace clan
{
	ImageImportDescription::ImageImportDescription()
		: impl(std::make_shared<ImageImportDescription_Impl>())
	{
	}

	ImageImportDescription::~ImageImportDescription()
	{
	}

	bool ImageImportDescription::get_premultiply_alpha() const
	{
		return impl->premultiply_alpha;
	}

	bool ImageImportDescription::flip_vertical() const
	{
		return impl->flip_vertical;
	}

	bool ImageImportDescription::is_srgb() const
	{
		return impl->srgb;
	}

	bool ImageImportDescription::is_cached() const
	{
		return impl->cached;
	}

	void ImageImportDescription::set_premultiply_alpha(bool enable)
	{
		impl->premultiply_alpha = enable;
	}

	void ImageImportDescription::set_flip_vertical(bool enable)
	{
		impl->flip_vertical = enable;
	}

	void ImageImportDescription::set_srgb(bool enable)
	{
		impl->srgb = enable;
	}

	void ImageImportDescription::set_cached(bool enable)
	{
		impl->cached = enable;
	}

	PixelBuffer ImageImportDescription::process(PixelBuffer &image) const
	{
		if (impl->premultiply_alpha)
			image.premultiply_alpha();

		if (impl->flip_vertical)
			image.flip_vertical();

		if (impl->func_process)
			image = impl->func_process(image);

		return image;
	}

	std::function<PixelBuffer(PixelBuffer &)> &ImageImportDescription::func_process()
	{
		return impl->func_process;
	}
}
