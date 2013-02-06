
#pragma once

class FileItemType;
class UIController;

class FileItemTypeFactory
{
public:
	void register_type(FileItemType *type);
	void unregister_type(FileItemType *type);
	const std::vector<FileItemType*> &types();
	FileItemType *find_from_extension(std::string extension);
	FileItemType *find_from_filename(std::string filename);

	void register_ui(UIController *ui_controller);

private:
	std::vector<FileItemType*> _types;
};
