#include <ClanLib/core.h>
#include <ClanLib/application.h>

// for std::boolalpha
#include <iomanip>

class XMLTest : public CL_ClanApplication
{
public:
	virtual int main(int argc, char **argv)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();
	
		try
		{
			// Initialize ClanLib base components
			CL_SetupCore setup_core;

			save_xml();
			std::cout << "look at test.xml in this folder" << std::endl;

			load_xml();

			console.wait_for_key();
		}
		catch(CL_Error error)
		{
			std::cout << "Exception caught : " << error.message.c_str() << std::endl;

			// Display console close message and wait for a key
			console.display_close_message();

			return -1;
		}

		return 0;
	}
	
	void save_xml()
	{
		// Create xml document
		CL_DomDocument document;

		// Create xml element (can be container)
		CL_DomElement element = document.create_element("sample");

		// Add some atributes to it
		element.set_attribute("test_value", CL_String::from_int(14));
		element.set_attribute("another_value", "string value");
		element.set_attribute("another_value2", "value2");

		CL_DomText text = document.create_text_node("Very long text, hehe");
		element.append_child(text);
			
		std::string str = element.get_text();
		std::cout << "Test get_text: " << str.c_str() << std::endl;

		// Create another xml element (it will be child of first element)
		CL_DomElement element2 = document.create_element("item");

		// Add some atributes to it
		element2.set_attribute("number", CL_String::from_int(10));
		element2.set_attribute("string", "str data");

		std::cout << "has 'number' attribute: " << std::boolalpha <<
			element2.has_attribute("number") << std::endl;
		std::cout << "has 'unknown' attribute: " << std::boolalpha <<
			element2.has_attribute("unknown") << std::endl;

		std::cout << "'number' value: " << element2.get_attribute("number", "100").c_str() << std::endl;
		std::cout << "'unknown' value: (default = \"dumb\"): " << element2.get_attribute("unknown", "dumb").c_str() << std::endl;

		// Connect it with parent
		element.append_child(element2);

		// Connect first element with dom document
		document.append_child(element);

		// Finaly, save xml document to file
		document.save(new CL_OutputSource_File("test.xml"), true, true);
	}
	
	void load_xml()
	{
		CL_DomDocument document;
		document.load(new CL_InputSource_File("test.xml"), true, true);
		
		CL_DomElement element = document.get_document_element();
		std::cout << "document element: " << element.get_tag_name().c_str() << std::endl;
		std::cout << "test_value: " << element.get_attribute("test_value").c_str() << std::endl;
		std::cout << "text: " << element.get_text().c_str() << std::endl;
		
		CL_DomElement element2 = element.get_first_child().get_next_sibling().to_element();
		std::cout << "element2: " << element2.get_tag_name().c_str() << std::endl;
		std::cout << "number: " << element2.get_attribute("number").c_str() << std::endl;
	}
} my_app;
