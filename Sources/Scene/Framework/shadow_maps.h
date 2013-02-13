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

#include "Scene/Framework/inout_data.h"
#include <list>
namespace clan
{

class ShadowMaps;
class ShadowMapEntry;
class ShadowMapEntry_Impl;

class ShadowMaps
{
public:
	ShadowMaps(clan::GraphicContext &gc, int shadow_map_size, int max_active_maps, clan::TextureFormat format);
	~ShadowMaps();

	void start_frame();
	void assign_indexes();

	OutData<clan::Texture2DArray> shadow_maps;

private:
	void use_entry(ShadowMapEntry_Impl *entry);
	void entry_destroyed(ShadowMapEntry_Impl *entry);

	void add_used(ShadowMapEntry_Impl *entry);
	void add_unused(ShadowMapEntry_Impl *entry);
	void unlink(ShadowMapEntry_Impl *entry);

	std::vector<clan::FrameBuffer> framebuffers;
	std::vector<clan::Texture2D> views;

	ShadowMapEntry_Impl *used_entries;
	ShadowMapEntry_Impl *unused_entries;
	std::vector<int> free_indexes;

	friend class ShadowMapEntry;
	friend class ShadowMapEntry_Impl;
};

class ShadowMapEntry
{
public:
	ShadowMapEntry();
	ShadowMapEntry(ShadowMaps *shadow_maps);
	bool is_null() const { return !impl; }

	void use_in_frame();

	int get_index() const;
	clan::FrameBuffer get_framebuffer() const;
	clan::Texture2D get_view() const;

private:
	std::shared_ptr<ShadowMapEntry_Impl> impl;
};

class ShadowMapEntry_Impl
{
public:
	ShadowMapEntry_Impl(ShadowMaps *maps) : shadow_maps(maps), index(-1), prev(0), next(0) { }
	~ShadowMapEntry_Impl() { shadow_maps->entry_destroyed(this); }

	ShadowMaps *shadow_maps;
	int index;

	ShadowMapEntry_Impl *prev;
	ShadowMapEntry_Impl *next;
};

}

