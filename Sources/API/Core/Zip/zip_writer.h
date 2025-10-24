/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

/// \addtogroup clanCore_I_O_Data clanCore I/O Data
/// \{

#pragma once

#include "../api_core.h"
#include "../Text/string_types.h"
#include "../System/sharedptr.h"
#include "../System/cl_platform.h"
#include <vector>

class CL_IODevice;
class CL_ZipWriter_Impl;

/// \brief Zip file writer.
///
/// \xmlonly !group=Core/Zip! !header=core.h! \endxmlonly
class CL_API_CORE CL_ZipWriter
{
/// \name Construction
/// \{

public:

	/// \brief Constructs a ZipWriter
	///
	/// \param output = IODevice
	/// \param storeFilenamesAsUTF8 = bool
	CL_ZipWriter(CL_IODevice &output, bool storeFilenamesAsUTF8 = false);

/// \}
/// \name Operations
/// \{

public:
	/// \brief Begins file entry in the zip file.
	void begin_file(const CL_StringRef &filename, bool compress);

	/// \brief Writes some file data to the zip file.
	void write_file_data(const void *data, cl_byte64 size);

	/// \brief Ends the file entry.
	void end_file();

	/// \brief Writes the table of contents part of the zip file.
	void write_toc();

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_ZipWriter_Impl> impl;
/// \}
};

/// \}
