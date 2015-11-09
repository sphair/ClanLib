
#include <ClanLib/core.h>
using namespace clan;

void TestXMLFile(const std::string &filename)
{
	try
	{
		Console::write_line(filename);

		File file(filename, File::open_existing, File::access_read);
		DomDocument document;
		document.load(file);

		DomElement element = document.get_first_child().to_element();
		const std::string &letters = element.get_attribute("letters");

		Console::write_line(letters);
	}
	catch(Exception error)
	{
		Console::write_line("Exception caught:");
		Console::write_line(error.message);
	}
	Console::write_line("");
}

int main(int, char**)
{

	TestXMLFile("test-emeditor-utf8-iso-8859-1.xml");
	TestXMLFile("test-emeditor-utf8-withoutsignature.xml");
	TestXMLFile("test-emeditor-utf8-withsignature.xml");
	TestXMLFile("test-notepad-utf8.xml");
	TestXMLFile("test-notepad-unicode.xml");
	TestXMLFile("test-notepad-ansi.xml");

	return 0;
}
