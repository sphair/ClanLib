
#include "precomp.h"
#include "file_item_type_factory.h"
#include "file_item_type.h"
#include "UIController/document_editor_type.h"
#include <algorithm>

using namespace clan;

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
