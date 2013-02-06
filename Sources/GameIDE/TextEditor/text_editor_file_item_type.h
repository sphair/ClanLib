
#pragma once

#include "../FileItemType/file_item_type.h"
#include "../FileItemType/file_item_type_factory.h"
#include "../UIController/document_editor_type.h"

class TextDocumentEditorType : public DocumentEditorType
{
public:
	std::string get_name() { return "Text Editor"; }
	void register_ui(UIController *controller);
	DocumentEditor *open(UIController *controller, std::string filename);
};

class TextEditorFileItemType : public FileItemType
{
public:
	clan::Sprite get_icon(clan::GraphicContext &gc, clan::ResourceManager *resources);
	std::string get_description();
	std::string get_name();
	std::string get_extension();
	bool create(clan::GUIComponent *owner, std::string filename);
	std::vector<DocumentEditorType *> get_editors() { std::vector<DocumentEditorType *> v; v.push_back(&editor_type); return v; }

private:
	TextDocumentEditorType editor_type;
};
