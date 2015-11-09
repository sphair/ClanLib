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

#include "API/Display/Resources/display_cache.h"
#include "API/Core/Resources/file_resource_document.h"

namespace clan
{
	class FontFamily;

	class FileDisplayCache : public DisplayCache
	{
	public:
		FileDisplayCache(const FileResourceDocument &doc);
		~FileDisplayCache();

		Resource<Sprite> get_sprite(Canvas &canvas, const std::string &id) override;
		Resource<Image> get_image(Canvas &canvas, const std::string &id) override;
		Resource<Texture> get_texture(GraphicContext &gc, const std::string &id) override;
		Resource<Font> get_font(Canvas &canvas, const std::string &family_name, const FontDescription &desc) override;

	private:
		FileResourceDocument doc;

		std::map<std::string, Resource<Sprite> > sprites;
		std::map<std::string, Resource<Image> > images;
		std::map<std::string, Resource<Texture> > textures;
		std::map<std::string, FontFamily > fonts;
	};
}
