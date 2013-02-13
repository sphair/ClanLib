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

#include "Scene/LevelData/level_data.h"
namespace clan
{

class FileSearchPath;

class WMOLevelLoader
{
public:
	static std::shared_ptr<LevelData> load(const std::string &filename, const FileSearchPath &search_path);

private:
	WMOLevelLoader(LevelData *data, const std::string &filename, const FileSearchPath &search_path);
	LevelData *data;

	struct BaseMaterial
	{
		unsigned int flags;
		unsigned int specular_mode;
		unsigned int blend_mode;
		unsigned int offset_texture0;
		unsigned int color_texture0;
		unsigned int flags_texture0;
		unsigned int offset_texture1;
		unsigned int color_texture1;
		unsigned int flags_texture1;
		unsigned int unknown0;
		unsigned int unknown1;
		unsigned int unknown2;
		unsigned int unknown3;
		unsigned int unknown4;
		unsigned int unknown5;
		unsigned int unknown6;
		std::string texture0;
		std::string texture1;
	};

	struct IFFHeader
	{
		IFFHeader() : size(0), next_position(0) { }
		IFFHeader(std::string name, unsigned int size, unsigned int next_position)
		: name(name), size(size), next_position(next_position) { }

		std::string name;
		unsigned int size;
		unsigned int next_position;
	};
	static IFFHeader read_iff_header(clan::File &file);
};

}

