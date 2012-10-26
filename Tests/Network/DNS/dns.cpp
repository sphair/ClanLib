
#include <ClanLib/core.h>
#include <ClanLib/network.h>
using namespace clan;

int main(int argc, char**argv)
{
	SetupCore setup_core;
	SetupNetwork setup_network;
	try
	{
		std::string name = "clanlib.org";
		if (argc == 2)
			name = StringHelp::local8_to_text(argv[1]);
		DNSResolver resolver;
		Console::write_line("Looking up DNS resource %1", name);
		std::vector<DNSResourceRecord> records = resolver.lookup_resource(name, "A", 30*1000);
		std::vector<DNSResourceRecord>::size_type pos, size;
		size = records.size();
		for (pos = 0; pos < size; pos++)
		{
			Console::write_line("Found answer: %1", records[pos].get_a_address_str());
		}
	}
	catch (Exception e)
	{
		Console::write_line(e.message);
	}
	return 0;
}
