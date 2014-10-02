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
*/

#pragma once

#include "../View/view.h"

namespace clan
{
	class Image;

	class ImageView : public View
	{
	public:
		Image image();
		void set_image(Image image);

		Image highlighted_image();
		void set_highlighted_image(Image image);

		std::vector<Image> animation_images();
		void set_animation_images(const std::vector<Image> &images);

		std::vector<Image> highlighted_animation_images();
		void set_highlighted_animation_images(const std::vector<Image> &images);

		float animation_duration() const;
		void set_animation_duration(float value);

		float animation_repeat_count() const;
		void set_animation_repeat_count(float value);

		void start_animating();
		void stop_animating();
		bool is_animating() const;
	};
}
