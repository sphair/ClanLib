/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

namespace clan
{
/// \addtogroup clanGameIDE_System clanGameIDE System
/// \{

class DocumentEditor;
class DocumentEditorType;
class DockableComponent;
class SolutionModel;
class UIControllerListener;

class UIController
{
public:
	UIController(Ribbon *ribbon, SolutionModel &solution_model);
	~UIController();

	void add_listener(std::shared_ptr<UIControllerListener> listener);

	Ribbon *get_ribbon() const { return ribbon; }

	void open(std::string filename, DocumentEditorType *editor_type);

	void set_active(DocumentEditor *editor);
	void editor_destroyed(DocumentEditor *editor);

	void add_dockable(DockableComponent *dockable) { dockables.push_back(dockable); }
	const std::vector<DockableComponent *> &get_dockables() const { return dockables; }

	Signal<void(DocumentEditor *)> sig_editor_activated;
	Signal<void(DocumentEditor *)> sig_editor_destroyed;

	SolutionModel &get_solution_model() { return solution_model; }

private:
	RibbonSection *get_ribbon_section(const std::string &page_name, const std::string &section_name);

	void show_ribbon_section(const std::string &page_name, const std::string &section_name);
	void hide_ribbon_section(const std::string &page_name, const std::string &section_name);

	struct OpenDocument
	{
		OpenDocument(DocumentEditor *editor, DocumentEditorType *type) : editor(editor), type(type) { }

		DocumentEditor *editor;
		DocumentEditorType *type;
	};

	struct RibbonSectionInfo
	{
		RibbonSectionInfo() : section(0), visible_count(0) { }
		RibbonSection *section;
		int visible_count;
	};

	struct RibbonPageInfo
	{
		RibbonPageInfo() : page(0), visible_count(0) { }
		RibbonPage *page;
		int visible_count;
		std::map<std::string, RibbonSectionInfo> sections;
	};

	Ribbon *ribbon;
	std::map<std::string, RibbonPageInfo> ribbon_pages;
	SolutionModel &solution_model;
	std::vector<OpenDocument> documents;
	std::vector<DockableComponent *> dockables;
	std::vector<std::shared_ptr<UIControllerListener> > listeners;

	friend class UIControllerListener;
	friend class UIRibbonSection;
};

}

/// \}
