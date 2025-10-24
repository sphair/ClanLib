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

//! clanCore="I/O Data"
//! header=core.h

#ifndef header_zip_file_entry
#define header_zip_file_entry

#ifdef CL_API_DLL
#ifdef CL_CORE_EXPORT
#define CL_API_CORE __declspec(dllexport)
#else
#define CL_API_CORE __declspec(dllimport)
#endif
#else
#define CL_API_CORE
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>
#include "datatypes.h"

class CL_Zip_FileEntry_Generic;

//: File entry in zip file.
//- !group=Core/IO Data!
//- !header=core.h!
class CL_API_CORE CL_Zip_FileEntry
{
//! Construction:
public:
	CL_Zip_FileEntry();
	
	CL_Zip_FileEntry(const CL_Zip_FileEntry &copy);
	
	~CL_Zip_FileEntry();

//! Attributes:
public:
	//: Returns the filename of file entry.
	const std::string & get_filename() const;
	
	//: Returns the local filename of file entry. Applicable only when making zips.
	const std::string & get_local_filename() const;

	//: Returns the uncompressed size of file entry.
	cl_int64 get_uncompressed_size();
	
	//: Returns the compressed size of file entry.
	cl_int64 get_compressed_size();

//! Operations:
public:
	//: Copy assignment operator.
	CL_Zip_FileEntry &operator =(const CL_Zip_FileEntry &copy);
	
	//: Sets the filename of file entry.
	void set_filename(const std::string &filename);

	//: Sets the physical location of the file on our HD
	void set_local_filename(const std::string &forced_path);

//! Implementation:
private:
	CL_Zip_FileEntry_Generic *impl;
	friend class CL_Zip_Archive;
	friend class CL_InputSource_Zip_FileEntry;
};

#endif
