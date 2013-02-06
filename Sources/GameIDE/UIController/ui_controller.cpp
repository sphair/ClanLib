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
#include "ui_controller.h"
#include "document_editor.h"
#include "document_editor_type.h"
#include "document_editor_ribbon.h"
#include "Workspace/workspace.h"
#include "BuildSystem/build_system_ribbon.h"
#include <algorithm>

namespace clan
{

UIController::UIController(Ribbon *ribbon, SolutionModel &solution_model)
: ribbon(ribbon), solution_model(solution_model)
{
	add_listener(std::shared_ptr<UIControllerListener>(new DocumentEditorRibbon(this)));
	add_listener(std::shared_ptr<UIControllerListener>(new BuildSystemRibbon(this)));
}

UIController::~UIController()
{
	listeners.clear();
}

void UIController::add_listener(std::shared_ptr<UIControllerListener> listener)
{
	listeners.push_back(listener);
}

RibbonSection *UIController::get_ribbon_section(const std::string &page_name, const std::string &section_name)
{
	RibbonPageInfo &page_info = ribbon_pages[page_name];
	RibbonSectionInfo &section_info = page_info.sections[section_name];

	if (page_info.page == 0)
	{
		page_info.page = new RibbonPage(ribbon, page_name);
		page_info.page->set_visible(false);
	}

	if (section_info.section == 0)
	{
		section_info.section = new RibbonSection(page_info.page, section_name);
		section_info.section->set_visible(false);
	}

	return section_info.section;
}

void UIController::show_ribbon_section(const std::string &page_name, const std::string &section_name)
{
	RibbonPageInfo &page_info = ribbon_pages[page_name];
	RibbonSectionInfo &section_info = page_info.sections[section_name];

	if (page_info.page)
	{
		page_info.visible_count++;
		if (page_info.visible_count == 1)
		{
			page_info.page->set_visible(true, false);
			ribbon->request_repaint(); // Bug: shouldn't be needed.
		}
	}

	if (section_info.section)
	{
		section_info.visible_count++;
		if (section_info.visible_count == 1)
			section_info.section->set_visible(true, false);
	}
}

void UIController::hide_ribbon_section(const std::string &page_name, const std::string &section_name)
{
	RibbonPageInfo &page_info = ribbon_pages[page_name];
	RibbonSectionInfo &section_info = page_info.sections[section_name];

	if (section_info.section)
	{
		section_info.visible_count--;
		if (section_info.visible_count == 0)
			section_info.section->set_visible(false, false);
	}

	if (page_info.page)
	{
		page_info.visible_count--;
		if (page_info.visible_count == 0)
			page_info.page->set_visible(false);
	}
}

void UIController::open(std::string filename, DocumentEditorType *editor_type)
{
	for (size_t i = 0; i < documents.size(); i++)
	{
		if (documents[i].type == editor_type && documents[i].editor->get_filename() == filename)
		{
			documents[i].editor->show();
			return;
		}
	}
	documents.push_back(OpenDocument(editor_type->open(this, filename), editor_type));
	documents.back().editor->show_as_docked(Workspace::instance()->get_root_dock());
}

void UIController::set_active(DocumentEditor *editor)
{
	for (size_t i = 0; i < listeners.size(); i++)
	{
		listeners[i]->set_active(editor);
	}

	sig_editor_activated.invoke(editor);
}

void UIController::editor_destroyed(DocumentEditor *editor)
{
	for (size_t i = 0; i < listeners.size(); i++)
	{
		listeners[i]->editor_destroyed(editor);
	}
}
/*
void UIController::register_ribbon_setup(const std::string &resource_name)
{
	Resource resource = ribbon->get_resources().get_resource(resource_name);
	DomElement xml_ribbon_setup = resource.get_element();
	std::vector<DomNode> xml_pages = xml_ribbon_setup.select_nodes("page");
	for (size_t i = 0; i < xml_pages.size(); i++)
	{
		std::string page_name = xml_pages[i].to_element().get_attribute("name");
		int page_importance = xml_pages[i].to_element().get_attribute_int("importance", 1000);
		UIRibbonPage &ribbon_page = page(page_name, page_importance);

		std::vector<DomNode> xml_sections = xml_pages[i].select_nodes("section");
		for (size_t j = 0; j < xml_sections.size(); j++)
		{
			std::string section_name = xml_sections[j].to_element().get_attribute("name");
			int section_importance = xml_sections[j].to_element().get_attribute_int("importance", 1000);
			UIRibbonSection &ribbon_section = ribbon_page.section(section_name, section_importance);
			ribbon_section.width = xml_sections[j].to_element().get_attribute_int("width", ribbon_section.width);

			DomElement xml_component = xml_sections[j].to_element().get_first_child_element();
			while (!xml_component.is_null())
			{
				std::string component_type = xml_component.get_node_name();
				std::string component_name = xml_component.get_attribute("name");
				component_factory.get_type(component_type)->load_ribbon_setup(ribbon_section, component_name, xml_component);
				xml_component = xml_component.get_next_sibling_element();
			}
		}
	}
}
*/

}

