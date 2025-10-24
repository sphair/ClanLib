/*
	GUI2CPP,
	Written in the unholy year 2003 by Magnus Norddahl.

	This is a little commandline tool that sync's a c++ source file and
	header file with a ClanLib GUI definition file.
*/

#include "gui2cpp.h"
#include <iostream>

/////////////////////////////////////////////////////////////////////////////
// Application startup:

int main(int argc, char **argv)
{
	if (argc != 4)
	{
		std::cout << "Syntax: gui2cpp <gui file> <source file> <header file>" << std::endl << std::endl;
		return 255;
	}

#ifdef WIN32
	CL_SetupCore::set_instance(GetModuleHandle(0));
#endif
	CL_SetupCore::init();
	CL_SetupDisplay::init();
	CL_SetupGUI::init();

	try
	{
		GUI2CPP gui2cpp(argv[1], argv[2], argv[3]);
		if (gui2cpp.load() == false) return 1;
		if (gui2cpp.convert() == false) return 2;
		if (gui2cpp.save() == false) return 3;
	}
	catch (CL_Error err)
	{
		std::cout << err.message.c_str() << std::endl;
		return 254;
	}

	CL_SetupGUI::deinit();
	CL_SetupDisplay::deinit();
	CL_SetupCore::deinit();

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// GUI2CPP construction:

GUI2CPP::GUI2CPP(
	const std::string &gui_file,
	const std::string &source_file,
	const std::string &header_file)
	: gui_file(gui_file), source_file(source_file), header_file(header_file)
{
}

GUI2CPP::~GUI2CPP()
{
}

/////////////////////////////////////////////////////////////////////////////
// GUI2CPP attributes:

/////////////////////////////////////////////////////////////////////////////
// GUI2CPP operations:

bool GUI2CPP::load()
{
	gui_document.load(new CL_InputSource_File(gui_file), true);
	
	bool result;
	result = load_file(source_data, source_file);
	if (!result) return false;

	result = load_file(header_data, header_file);
	if (!result) return false;

	return true;
}

bool GUI2CPP::convert()
{
	bool result;

	old_header_variables = find_block(header_data, "CL_HeaderVariables");
	old_header_functions = find_block(header_data, "CL_HeaderFunctions");
	old_source_connections = find_block(source_data, "CL_SourceConnections");

	result = parse_components(gui_document.get_document_element());
	if (!result) return false;

	result = inject_block(header_data, "CL_HeaderVariables", new_header_variables);
	if (!result) return false;

	result = inject_block(header_data, "CL_HeaderFunctions", new_header_functions);
	if (!result) return false;

	result = inject_block(source_data, "CL_SourceConnections", new_source_connections);
	if (!result) return false;

	return true;
}

bool GUI2CPP::save()
{
	bool result;
	result = save_file(source_data, source_file);
	if (!result) return false;

	result = save_file(header_data, header_file);
	if (!result) return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// GUI2CPP implementation:

bool GUI2CPP::parse_components(const CL_DomElement &componentsElement)
{
	for (CL_DomNode node = componentsElement.get_first_child(); !node.is_null(); node = node.get_next_sibling())
	{
		if (!node.is_element()) continue;

		CL_DomElement element = node.to_element();
		std::string component_type = element.get_tag_name();
		std::string component_name = element.get_attribute("name");

		std::cout << "name: " << component_name.c_str() << ", type: " << component_type.c_str() << std::endl;

		// Setup header variables:
		if (!component_name.empty())
		{
			std::string class_name = CL_ComponentType::get_class_name(component_type);
			new_header_variables.append(class_name + " *" + component_name + ";\r\n");
			new_source_connections.append("component_manager.get_component(\""+component_name+"\", &"+component_name+");\r\n");
		}

		// Setup connections:
		CL_DomElement connections = element.named_item("connections").to_element();
		if (connections.is_element())
		{
			for (CL_DomNode connectionNode = connections.get_first_child(); !connectionNode.is_null(); connectionNode = connectionNode.get_next_sibling())
			{
				if (!connectionNode.is_element()) continue;

				CL_DomElement connection = connectionNode.to_element();
				if (connection.get_tag_name() != "connection") continue;

				// todo: Update new_header_functions
				// todo: Update new_source_connections
				// todo: Search old_header_functions for this slot.
				//  If its not there; append function body template to cpp file.
			}
		}

		// Parse any child components, if any.
		if (element.named_item("components").is_element())
		{
			bool result = parse_components(element.named_item("components").to_element());
			if (result == false) return false;
		}
	}
	return true;
}

bool GUI2CPP::load_file(std::string &file_data, const std::string &filename)
{
	CL_InputSource_File input(filename);
	int size = input.size();
	char *data = new char[size];
	size = input.read(data, size);
	file_data = std::string(data, size);
	delete[] data;
	return true;
}

bool GUI2CPP::save_file(const std::string &file_data, const std::string &filename)
{
	CL_OutputSource_File output(filename);
	output.write(file_data.data(), (int) file_data.size());
	return true;
}

std::string GUI2CPP::find_block(const std::string &file_data, const std::string &block_name)
{
	std::string begin_marker = "// {{" + block_name;
	std::string end_marker = "// }}" + block_name;

	std::string::size_type start_pos = file_data.find(begin_marker);
	if (start_pos == std::string::npos) return std::string();

	std::string::size_type end_pos = file_data.find(end_marker, start_pos);
	if (end_pos == std::string::npos) return std::string();

	return file_data.substr(start_pos, end_pos-start_pos);
}

bool GUI2CPP::inject_block(
	std::string &file_data,
	const std::string &block_name,
	const std::string &block_data)
{
	// todo: Inject with tab/whitespace indentation derived from the file data.

	std::string begin_marker = "// {{" + block_name;
	std::string end_marker = "// }}" + block_name;

	std::string::size_type start_pos = file_data.find(begin_marker);
	if (start_pos != std::string::npos)
	{
		std::string::size_type end_pos = file_data.find(end_marker, start_pos);
		if (end_pos == std::string::npos)
		{
			std::cout << "Found begin marker, but unable to find end marker: " << end_marker.c_str() << std::endl;
			return false;
		}

		file_data =
			file_data.substr(0, start_pos) +
			begin_marker + "\r\n" + block_data + end_marker +
			file_data.substr(end_pos+end_marker.length());
	}
	else
	{
		file_data.append(begin_marker + "\r\n");
		file_data.append(block_data);
		file_data.append(end_marker + "\r\n");
	}

	return true;
}
