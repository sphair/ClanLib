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
*/

#pragma once

#include "API/Core/System/cl_platform.h"
#include "API/Core/System/databuffer.h"
#include "zip_file_header.h"

namespace clan
{
	class ZipFileEntry_Impl
	{
	public:
		/// \brief Zip file entry type enum.
		enum Type
		{
			type_file,
			type_removed,
			type_added_memory,
			type_added_file
		};

		/// \brief Central directory record attributes for entry:
		ZipFileHeader record;

		/// \brief File entry type.
		Type type;

		/// \brief Offset to zip data in zip file (type_file).
		// int64_t offset;

		/// \brief Filename of file, if added from file (type_added_file).
		std::string filename;

		/// \brief Data of zip file entry, if added from memory (type_added_memory).
		DataBuffer data;

		/// \brief True, if this entry is a directory.
		bool is_directory;
	};
}
