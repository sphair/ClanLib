/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "resource_tree.h"
#include "resource_manager.h"

#include <ClanLib/core.h>

#include <iostream>
#include <map>
#include <unistd.h>

struct Resource
{
	Resource(std::string n, std::string l, const ResourceOptions &o): name(n), location(l), options(o)
		{type=options.get_option("type").get_values()[0];options.remove("type");}
	std::string name;
	std::string type;
	std::string location;
	ResourceOptions options;
};

struct Section
{
	Section(std::string n, Section *p=0):parent(p), name(n) {}
	std::string name;
	std::vector<Section*> sections;
	std::vector<Resource*> resources;
	Section* parent;

	~Section() {}
};

struct Tree
{
	Tree() {current_section = root = new Section("");};
	~Tree() {delete root;}
	Section *root;
	Section *current_section;
};

ResourceTree::ResourceTree(std::vector<char> buffer):
	tree(new Tree),
	manager(new ResourceManager(buffer, this))
{
}

ResourceTree::~ResourceTree()
{
	delete manager;
	delete tree;
}

void ResourceTree::add_section(std::string name)
{
	Section* sect=new Section(name,tree->current_section);
	tree->current_section->sections.push_back(sect);
	tree->current_section = sect;
}

void ResourceTree::end_section()
{
	tree->current_section = tree->current_section->parent;
}

void ResourceTree::add_resource(std::string name, std::string location, const ResourceOptions &options)
{
	tree->current_section->resources.push_back(new Resource(name, location, options));
}

std::map<std::string, void (*)(Resource*, CL_DomElement&, CL_DomDocument&)> resource_map;

std::string escape_option(const std::string& opt)
{
	std::string str;
	std::string::const_iterator it = opt.begin();
	for(;it!=opt.end();++it)
	{
		if(*it=='"')
			str += "&quot;";
		else
			str += *it;
	}
	return str;
}

std::string option_to_string(const std::vector<std::string> &value)
{
	std::string str;
	std::vector<std::string>::const_iterator it = value.begin();
	str = escape_option(*it++);
	for(;it != value.end();++it)
	{
		str += ",";
		str += escape_option(*it);
	}

	return str;
}

void basic_resource_element(Resource* res, CL_DomElement& res_elem, CL_DomDocument& document)
{
	if(res->location!="")
		res_elem.set_attribute("value", res->location);
}

void file_resource_element(Resource* res, CL_DomElement& res_elem, CL_DomDocument& document)
{
	res_elem.set_attribute("file", res->location);
}

void sprite_resource_element(Resource* res, CL_DomElement& res_elem, CL_DomDocument& document)
{
	for(int i=1;;i++)
	{
		std::string str_image = "image";
		str_image += CL_String::from_int(i);
		if(!res->options.exists(str_image))
			break;

		CL_DomElement elem = document.create_element("image");
		elem.set_attribute("name", str_image);
		elem.set_attribute("file", option_to_string(res->options.get_option(str_image).get_values()));
		res->options.remove(str_image);


		// I'm not really sure what the format should look like

		if(res->options.exists(str_image + "_method"))
		{
			std::string method = res->options.get_option(str_image + "_method").get_values()[0];
			res->options.remove(str_image + "_method");
			CL_DomElement method_elem = document.create_element(method);

			std::string method_options[] = {"pos","size","array","array_skipframes","spacing","tcol","trans_limit"};

			for(int j=0;j<sizeof(method_options)/sizeof(method_options[0]);j++)
			{
				std::string str_option = str_image + "_" + method_options[j];
				if(res->options.exists(str_option))
				{
					
					method_elem.set_attribute(method_options[j], option_to_string(res->options.get_option(str_option).get_values()));
					res->options.remove(str_option);
				}
			}

			elem.append_child(method_elem);
		}
		
		res_elem.append_child(elem);
	}
}

void init_resource_map()
{
	resource_map["integer"]=basic_resource_element;
	resource_map["string"]=basic_resource_element;
	resource_map["font"]=basic_resource_element;

	resource_map["surface"]=file_resource_element;
	resource_map["sample"]=file_resource_element;

	resource_map["sprite"]=sprite_resource_element;
}


void add_resource_element(Resource* res, CL_DomElement& sect_elem, CL_DomDocument& document)
{
	CL_DomElement elem = document.create_element(res->type);
	elem.set_attribute("name", res->name);

	if(resource_map.find(res->type)!=resource_map.end())
		resource_map[res->type](res, elem, document);
	else
		basic_resource_element(res, elem, document);

	std::map<std::string, ResourceOption> options = res->options.get_options();

	std::map<std::string, ResourceOption>::iterator it=options.begin();
	for(;it!=options.end();++it)
		elem.set_attribute(it->first, option_to_string(it->second.get_values()));
	sect_elem.append_child(elem);
}

void add_section_element(Section* section, CL_DomElement& sect_elem, CL_DomDocument& document)
{
	std::vector<Section*>::iterator sec_it = section->sections.begin();
	for(;sec_it!=section->sections.end();++sec_it)
	{
		CL_DomElement elem = document.create_element("section");
		elem.set_attribute("name", (*sec_it)->name);
		add_section_element(*sec_it, elem, document);
		sect_elem.append_child(elem);
	}

	std::vector<Resource*>::iterator res_it = section->resources.begin();
	for(;res_it!=section->resources.end();++res_it)
	{
		add_resource_element(*res_it, sect_elem, document);
	}
}



class OutputSource_Stdout : public CL_OutputSource
{
public:
	int tell() const { return -1; }
	int size() const { return -1; }
	void open() { return; }
	void close() { return; }
	CL_OutputSource *clone() { return new OutputSource_Stdout; }
	int write(const void *data, int size) { return ::write(1, data, size); }
};

void ResourceTree::write()
{
	CL_DomDocument document;
	CL_DomElement root_element = document.create_element("resources");

	add_section_element(tree->root, root_element, document);

	document.append_child(root_element);
	document.save(new OutputSource_Stdout(), true);
}
