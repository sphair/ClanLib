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

/// \addtogroup clanDisplay_Resources clanDisplay Resources
/// \{

#pragma once

#include "../api_display.h"
#include "../../Core/Resources/resource.h"
#include <memory>

namespace clan
{

class XMLResourceDocument;
class GraphicContext;
class Canvas;
class Sprite;
class Image;
class Texture;
class Font;
class FontDescription;
class DisplayCacheProvider;
class DisplayCache_Impl;

class CL_API_DISPLAY DisplayCache
{
public:
	DisplayCache();
	DisplayCache(const XMLResourceDocument &doc);
	DisplayCache(DisplayCacheProvider *provider);

	bool is_null() const;

	DisplayCacheProvider *get_provider() const;

	Resource<Sprite> get_sprite(GraphicContext &gc, const std::string &id);
	Resource<Image> get_image(GraphicContext &gc, const std::string &id);
	Resource<Texture> get_texture(GraphicContext &gc, const std::string &id);
	Resource<Font> get_font(Canvas &canvas, const FontDescription &desc);

private:
	std::shared_ptr<DisplayCache_Impl> impl;
};

}

/// \}
