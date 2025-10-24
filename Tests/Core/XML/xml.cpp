
#include <ClanLib/core.h>

void TestXMLFile(const CL_String &filename)
{
	try
	{
		CL_Console::write_line(filename);

		CL_File file(filename, CL_File::open_existing, CL_File::access_read);
		CL_DomDocument document;
		document.load(file);

		CL_DomElement element = document.get_first_child().to_element();
		const CL_String &letters = element.get_attribute("letters");

		CL_Console::write_line(letters);
	}
	catch(CL_Exception error)
	{
		CL_Console::write_line("Exception caught:");
		CL_Console::write_line(error.message);
	}
	CL_Console::write_line("");
}

int main(int, char**)
{
	CL_SetupCore setup_core;

	TestXMLFile("test-emeditor-utf8-iso-8859-1.xml");
	TestXMLFile("test-emeditor-utf8-withoutsignature.xml");
	TestXMLFile("test-emeditor-utf8-withsignature.xml");
	TestXMLFile("test-notepad-utf8.xml");
	TestXMLFile("test-notepad-unicode.xml");
	TestXMLFile("test-notepad-ansi.xml");

	return 0;
}
