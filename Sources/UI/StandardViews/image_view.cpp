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
*/

#include "UI/precomp.h"
#include "API/UI/StandardViews/button_view.h"
#include "API/UI/UIThread/ui_thread.h"
#include "API/Display/2D/image.h"
#include "API/Display/2D/canvas.h"
#include <algorithm>

namespace clan
{

	/////////////////////////////////////////////////////////////////////////

	class ImageViewImpl
	{
	public:
		std::shared_ptr<ImageSource> image;
		std::shared_ptr<ImageSource> highlighted_image;
		Image canvas_image;
		Image canvas_highlighted_image;

		void get_images(Canvas &canvas)
		{
			if (!canvas_image && image)
				canvas_image = image->image(canvas);

			if (!canvas_highlighted_image && highlighted_image)
				canvas_highlighted_image = highlighted_image->image(canvas);
		}
	};

	ImageView::ImageView() : impl(std::make_shared<ImageViewImpl>())
	{
	}

	std::shared_ptr<ImageSource> ImageView::image()
	{
		return impl->image;
	}

	void ImageView::set_image(const std::shared_ptr<ImageSource> &image)
	{
		impl->image = image;
		impl->canvas_image = Image();
		set_needs_render();
		set_needs_layout();
	}

	void ImageView::set_image(const Image &image)
	{
		set_image(ImageSource::from_image(image));
	}

	std::shared_ptr<ImageSource> ImageView::highlighted_image()
	{
		return impl->highlighted_image;
	}

	void ImageView::set_highlighted_image(const std::shared_ptr<ImageSource> &image)
	{
		impl->highlighted_image = image;
		impl->canvas_highlighted_image = Image();
		set_needs_render();
		set_needs_layout();
	}

	void ImageView::set_highlighted_image(const Image &image)
	{
		set_highlighted_image(ImageSource::from_image(image));
	}

	void ImageView::render_content(Canvas &canvas)
	{
		impl->get_images(canvas);

		if (impl->canvas_image && impl->canvas_image.get_width() != 0.0f && impl->canvas_image.get_height() != 0.0f)
		{
			float scale_x = geometry().content_width / impl->canvas_image.get_width();
			float scale_y = geometry().content_height / impl->canvas_image.get_height();
			float scale = std::min(scale_x, scale_y);

			float width = impl->canvas_image.get_width() * scale;
			float height = impl->canvas_image.get_width() * scale;

			impl->canvas_image.draw(canvas, Rectf::xywh((geometry().content_width - width) * 0.5f, (geometry().content_height - height) * 0.5f, width, height));
		}
	}

	float ImageView::calculate_preferred_width(Canvas &canvas)
	{
		impl->get_images(canvas);

		if (impl->canvas_image)
			return impl->canvas_image.get_width();
		else
			return 0.0f;
	}

	float ImageView::calculate_preferred_height(Canvas &canvas, float width)
	{
		impl->get_images(canvas);

		if (impl->canvas_image && impl->canvas_image.get_width() != 0)
			return impl->canvas_image.get_height() * width / impl->canvas_image.get_width();
		else
			return 0.0f;
	}
}
