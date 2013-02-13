/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
namespace clan
{

class CachedTextureImpl
{
public:
	Texture texture;
};

class CachedTexture
{
public:
	CachedTexture() : impl(new CachedTextureImpl()) { }
	Texture &get_texture() { return impl->texture; }
	void set_texture(Texture &t) { impl->texture = t; }

	bool operator<(const CachedTexture &other) const { return impl < other.impl; }
	bool operator==(const CachedTexture &other) const { return impl == other.impl; }
	bool operator!=(const CachedTexture &other) const { return impl != other.impl; }

	//operator Texture &() { return impl->texture; }
	//operator const Texture &() const { return impl->texture; }

private:
	std::shared_ptr<CachedTextureImpl> impl;
};

}

