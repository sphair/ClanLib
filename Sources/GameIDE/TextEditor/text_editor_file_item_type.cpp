
#include "precomp.h"
#include "text_editor_file_item_type.h"
#include "text_editor.h"

using namespace clan;

Sprite TextEditorFileItemType::get_icon(GraphicContext &gc, ResourceManager *resources)
{
	return Sprite(gc, "IconDocument", resources);
}

std::string TextEditorFileItemType::get_name()
{
	return "XML File (.xml)";
}

std::string TextEditorFileItemType::get_description()
{
	return "Creates a blank XML file.";
}

std::string TextEditorFileItemType::get_extension()
{
	return "xml";
	//return "js";
}

bool TextEditorFileItemType::create(GUIComponent *owner, std::string filename)
{
	File file(filename, File::create_always, File::access_read_write);
	file.close();
	return true;
}

/////////////////////////////////////////////////////////////////////////////

void TextDocumentEditorType::register_ui(UIController *controller)
{
	TextEditor::register_ui(controller);
}

DocumentEditor *TextDocumentEditorType::open(UIController *controller, std::string filename)
{
	TextEditor *editor = new TextEditor(controller, filename);
	editor->load();
	return editor;
}
