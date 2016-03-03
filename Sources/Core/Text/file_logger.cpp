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
*/

#include "Core/precomp.h"
#include "API/Core/Text/file_logger.h"
#include "API/Core/IOData/file.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"

namespace clan
{
	FileLogger::FileLogger(const std::string &filename) : file(nullptr)
	{
		file = new File(filename, File::open_always, File::access_read_write);
	}

	FileLogger::~FileLogger()
	{
		delete file;
	}

	void FileLogger::log(const std::string &type, const std::string &text)
	{
		StringFormat format = get_log_string(type, text);
		std::string log_line = format.get_result();

		file->seek(0, File::seek_end);
		file->write(log_line.data(), (int)log_line.length());
	}
}
