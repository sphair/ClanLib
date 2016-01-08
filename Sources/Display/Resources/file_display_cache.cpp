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
**    Magnus Norddahl
**    Mark Page
*/

#include "Display/precomp.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/2D/sprite.h"
#include "API/Display/2D/image.h"
#include "API/Display/Render/texture_2d.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_description.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Render/texture.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/IOData/path_help.h"
#include "file_display_cache.h"
#include "../Font/font_impl.h"

namespace clan
{
	FileDisplayCache::FileDisplayCache(const FileResourceDocument &doc)
		: doc(doc)
	{
	}

	FileDisplayCache::~FileDisplayCache()
	{
	}

	Resource<Sprite> FileDisplayCache::get_sprite(Canvas &canvas, const std::string &id)
	{
		auto it = sprites.find(id);
		if (it != sprites.end())
		{
			Resource<Sprite> sprite = it->second;
			sprite.get() = sprite.get().clone();
			return sprite;
		}

		Resource<Sprite> sprite = Sprite(canvas, id, doc.get_file_system());
		sprites[id] = sprite;
		sprite.get() = sprite.get().clone();
		return sprite;
	}

	Resource<Image> FileDisplayCache::get_image(Canvas &canvas, const std::string &id)
	{
		auto it = images.find(id);
		if (it != images.end())
		{
			Resource<Image> image = it->second;
			image.get() = image.get().clone();
			return image;
		}

		Resource<Image> image = Image(canvas, id, doc.get_file_system());
		images[id] = image;
		image.get() = image.get().clone();
		return image;
	}

	Resource<Texture> FileDisplayCache::get_texture(GraphicContext &gc, const std::string &id)
	{
		auto it = textures.find(id);
		if (it != textures.end())
			return it->second;

		Resource<Texture> texture = Texture2D(gc, id, doc.get_file_system());
		textures[id] = texture;
		return texture;
	}

	Resource<Font> FileDisplayCache::get_font(Canvas &canvas, const std::string &family_name, const FontDescription &desc)
	{
		auto it = fonts.find(family_name);
		if (it != fonts.end())
			return Font(it->second, desc);

		//TODO: Maybe allow loading of fonts from the doc filesystem?

		FontFamily font_family(family_name);
		fonts[family_name] = font_family;

		Font font = Font(font_family, desc);

		return font;
	}
}
