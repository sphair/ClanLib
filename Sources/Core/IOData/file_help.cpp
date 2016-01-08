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
**    Kenneth Gangstoe
*/

#include "Core/precomp.h"
#include "API/Core/IOData/file_help.h"
#include "API/Core/IOData/file.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/System/exception.h"

#ifndef WIN32
#include <sys/stat.h>
#endif

namespace clan
{
	void FileHelp::copy_file(const std::string &from, const std::string &to, bool copy_always)
	{
#ifdef WIN32
		BOOL result = CopyFile(StringHelp::utf8_to_ucs2(from).c_str(), StringHelp::utf8_to_ucs2(to).c_str(), copy_always ? FALSE : TRUE);
		if (result == FALSE)
			throw Exception("Unable to copy file");
#else
		while (!copy_always)
		{
			try
			{
				File input_file(to);
			}
			catch (const Exception&)
			{
				break;
			}

			throw Exception("Destination file already exists");
		}

		File input_file(from);
		File output_file(to, File::create_always, File::access_read_write);
		char buffer[16*1024];
		while (true)
		{
			int bytes_read = input_file.read(buffer, 16*1024);
			if (bytes_read <= 0)
				break;

			int bytes_written = output_file.write(buffer, bytes_read);
			if (bytes_written != bytes_read)
			{
				output_file.close();
				FileHelp::delete_file(to);
				throw Exception("Unable to copy file");
			}
		}
#endif
	}

	void FileHelp::delete_file(const std::string &filename)
	{
#ifdef WIN32
		BOOL result = DeleteFile(StringHelp::utf8_to_ucs2(filename).c_str());
		if (result == FALSE)
			throw Exception("Unable to delete file");
#else
		std::string filename_local8 = StringHelp::text_to_local8(filename);
		int result = unlink(filename_local8.c_str());
		if (result == -1)
			throw Exception("Unable to delete file");
#endif
	}

	bool FileHelp::file_exists(const std::string &filename)
	{
#ifdef WIN32
		HANDLE file = CreateFile(
			StringHelp::utf8_to_ucs2(filename).c_str(),	/* lpFileName */
			0,					/* dwDesiredAccess */
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,	/* dwShareMode */
			0,					/* lpSecurityAttributes */
			OPEN_EXISTING,		/* dwCreationDisposition */
			0,					/* dwFlagsAndAttributes */
			0);					/* hTemplateFile */

		if (file != INVALID_HANDLE_VALUE) CloseHandle(file);
		return file != INVALID_HANDLE_VALUE;

		//	return (GetFileAttributes(filename.c_str()) != INVALID_FILE_ATTRIBUTES);
#else
		struct stat stFileInfo;
		return (stat(filename.c_str(), &stFileInfo) == 0);
#endif
	}
}
