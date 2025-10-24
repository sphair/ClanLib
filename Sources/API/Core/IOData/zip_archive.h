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

#ifndef header_zip_archive
#define header_zip_archive

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

#include "../System/sharedptr.h"
#include "zip_file_entry.h"
#include "inputsource_provider.h"

class CL_InputSource;
class CL_OutputSource;
class CL_Zip_Archive_Generic;

//: Zip archive.
//- !group=Core/IO Data!
//- !header=core.h!
class CL_API_CORE CL_Zip_Archive : public CL_InputSourceProvider
{
//! Construction:
public:
	//: Constructs or loads a ZIP archive.
	//param filename: .zip archive to load.
	CL_Zip_Archive();
	
	CL_Zip_Archive(const std::string &filename);
	
	CL_Zip_Archive(const CL_Zip_Archive &copy);
	
	~CL_Zip_Archive();

//! Attributes:
public:
	//: List of file entries in archive.
	std::vector<CL_Zip_FileEntry> &get_file_list();

//! Operations:
public:
	//: Opens a file in the archive.
	CL_InputSource *open_source(const std::string &filename);
	
	//: Clones this input source provider.
	CL_InputSourceProvider *clone();

	//: Get full path to source:
	std::string get_pathname(const std::string &filename);

	//: Returns a new inputsource provider object that uses a new path relative to current one.
	CL_InputSourceProvider *create_relative(const std::string &path);

	//: Creates a new file entry
	CL_OutputSource *create_file(const std::string &filename, bool compress = true);

	//: Adds a file to zip archive.
	//- <p>File is not added to zip file until it save() is called.</p>
	//param filename: Filename of file.
	void add_file(const std::string &filename, bool compress = true);

	//: Adds a file to zip archive with extra parameters
	//- <p> This versions allows you to set filename to any path you wish, to flexibily construct file tree layouts in the
	//- zip.  input_filename should be the true path and filename of the source file.</p>
	
	void add_file(const std::string &input_filename, const std::string &filename_in_archive, bool compress = true);

	//: Saves zip archive.
	//param filename: Filename of zip archive. Must not be used to save to the same as loaded from.
	//- <p>If no filename parameter was passed, it will modify the zip
	//- archive loaded at construction time. It does this by creating a
	//- temporary file, saving the new archive, deletes the old one and
	//- renames the temp file to the original archive filename.</p>
	//-
	//- <p>If the archive was created instead of loaded, a filename must
	//- be specify a filename. Likewise, if saving to same archive as
	//- loaded from, a filename must not be specified. Doing so will
	//- cause the save operation to fail.</p>


	void save();
	
	void save(const std::string &filename);
//! Implementation:
private:

	void write_zip_file(CL_Zip_FileEntry *pEntry, CL_OutputSource *output);

	CL_SharedPtr<CL_Zip_Archive_Generic> impl;
};
#endif
