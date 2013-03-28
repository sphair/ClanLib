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
#pragma once

#include "../FileItemType/file_item_type.h"
#include "../FileItemType/file_item_type_factory.h"
#include "../UIController/document_editor_type.h"

namespace clan
{

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
	Sprite get_icon(GraphicContext &gc, ResourceManager *resources);
	std::string get_description();
	std::string get_name();
	std::string get_extension();
	bool create(GUIComponent *owner, std::string filename);
	std::vector<DocumentEditorType *> get_editors() { std::vector<DocumentEditorType *> v; v.push_back(&editor_type); return v; }

private:
	TextDocumentEditorType editor_type;
};

}

