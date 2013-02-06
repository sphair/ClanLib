
#pragma once

class DocumentEditorType;

class FileItemType
{
public:
	virtual ~FileItemType() { }
	virtual clan::Sprite get_icon(clan::GraphicContext &gc, clan::ResourceManager *resources) { return clan::Sprite(); }
	virtual std::string get_name() = 0;
	virtual std::string get_description() = 0;
	virtual std::string get_extension() = 0;
	virtual bool create(clan::GUIComponent *owner, std::string filename) = 0;
	virtual std::vector<DocumentEditorType *> get_editors() = 0;
};
