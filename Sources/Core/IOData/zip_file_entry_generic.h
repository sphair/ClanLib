/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#ifndef header_zip_file_entry_generic
#define header_zip_file_entry_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Core/IOData/datatypes.h"
#include "zip_file_header.h"

class CL_Zip_FileEntry_Generic
{
//! Attributes:
public:
	//: Zip file entry type enum.
	enum Type
	{
		type_file,
		type_removed,
		type_added_memory,
		type_added_file
	};
	
	//: Central directory record attributes for entry:
	CL_Zip_FileHeader record;

	//: File entry type.
	Type type;
	
	//: Offset to zip data in zip file (type_file).
	// cl_int64 offset;

	//: Filename of file, if added from file (type_added_file).
	std::string filename;
	
	//: Data of zip file entry, if added from memory (type_added_memory).
	std::string data;

};

#endif
