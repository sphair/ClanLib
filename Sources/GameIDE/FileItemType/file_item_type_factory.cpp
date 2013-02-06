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
#include "GameIDE/precomp.h"
#include "file_item_type_factory.h"
#include "file_item_type.h"
#include "API/GameIDE/UIController/document_editor_type.h"
#include <algorithm>

namespace clan
{

void FileItemTypeFactory::register_type(FileItemType *type)
{
	_types.push_back(type);
}

void FileItemTypeFactory::unregister_type(FileItemType *type)
{
	_types.erase(std::find(_types.begin(), _types.end(), type));
}

const std::vector<FileItemType*> &FileItemTypeFactory::types()
{
	return _types;
}

FileItemType *FileItemTypeFactory::find_from_extension(std::string extension)
{
	for (size_t i = 0; i < _types.size(); i++)
	{
		if (StringHelp::compare(extension, _types[i]->get_extension(), true) == 0)
		{
			return _types[i];
		}
	}
	return 0;
}

FileItemType *FileItemTypeFactory::find_from_filename(std::string filename)
{
	return find_from_extension(PathHelp::get_extension(filename));
}

void FileItemTypeFactory::register_ui(UIController *ui_controller)
{
	for (size_t i = 0; i < _types.size(); i++)
	{
		std::vector<DocumentEditorType *> editors = _types[i]->get_editors();
		for (size_t j = 0; j < editors.size(); j++)
		{
			editors[j]->register_ui(ui_controller);
		}
	}
}

}

