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
**    Mark Page
*/

#include "Core/precomp.h"
#include "API/Core/Resources/file_resource_document.h"
#include "API/Core/IOData/file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/IOData/file.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "file_resource_document_impl.h"
#include <map>

namespace clan
{
	FileResourceDocument::FileResourceDocument()
		: impl(std::make_shared<FileResourceDocument_Impl>())
	{
	}

	FileResourceDocument::FileResourceDocument(const FileSystem &fs)
		: impl(std::make_shared<FileResourceDocument_Impl>())
	{
		impl->fs = fs;
	}

	FileResourceDocument::~FileResourceDocument()
	{
	}

	FileSystem FileResourceDocument::get_file_system() const
	{
		return impl->fs;
	}
}
