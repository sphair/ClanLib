
#pragma once

class DocumentEditor;
class DocumentEditorType;
class DockableComponent;
class SolutionModel;
class UIControllerListener;

class UIController
{
public:
	UIController(clan::Ribbon *ribbon, SolutionModel &solution_model);
	~UIController();

	void add_listener(std::shared_ptr<UIControllerListener> listener);

	clan::Ribbon *get_ribbon() const { return ribbon; }
	clan::RibbonSection *get_ribbon_section(const std::string &page_name, const std::string &section_name);

	void show_ribbon_section(const std::string &page_name, const std::string &section_name);
	void hide_ribbon_section(const std::string &page_name, const std::string &section_name);

	void open(std::string filename, DocumentEditorType *editor_type);

	void set_active(DocumentEditor *editor);
	void editor_destroyed(DocumentEditor *editor);

	void add_dockable(DockableComponent *dockable) { dockables.push_back(dockable); }
	const std::vector<DockableComponent *> &get_dockables() const { return dockables; }

	clan::Signal_v1<DocumentEditor *> sig_editor_activated;
	clan::Signal_v1<DocumentEditor *> sig_editor_destroyed;

	SolutionModel &get_solution_model() { return solution_model; }

private:
	struct OpenDocument
	{
		OpenDocument(DocumentEditor *editor, DocumentEditorType *type) : editor(editor), type(type) { }

		DocumentEditor *editor;
		DocumentEditorType *type;
	};

	struct RibbonSectionInfo
	{
		RibbonSectionInfo() : section(0), visible_count(0) { }
		clan::RibbonSection *section;
		int visible_count;
	};

	struct RibbonPageInfo
	{
		RibbonPageInfo() : page(0), visible_count(0) { }
		clan::RibbonPage *page;
		int visible_count;
		std::map<std::string, RibbonSectionInfo> sections;
	};

	clan::Ribbon *ribbon;
	std::map<std::string, RibbonPageInfo> ribbon_pages;
	SolutionModel &solution_model;
	std::vector<OpenDocument> documents;
	std::vector<DockableComponent *> dockables;
	std::vector<std::shared_ptr<UIControllerListener> > listeners;

	friend class UIControllerListener;
};
