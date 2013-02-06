
#pragma once

class OpenedItem
{
public:
	OpenedItem() { }
	OpenedItem(const std::string &filename, const std::string &editor) : filename(filename), editor(editor) { }

	std::string filename;
	std::string editor;
};
