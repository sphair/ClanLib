
#include <ClanLib/core.h>
#include <ClanLib/network.h>

int main(int argc, char**argv)
{
	CL_SetupCore setup_core;
	CL_SetupNetwork setup_network;
	try
	{
		CL_String name = "clanlib.org";
		if (argc == 2)
			name = CL_StringHelp::local8_to_text(argv[1]);
		CL_DNSResolver resolver;
		CL_Console::write_line("Looking up DNS resource %1", name);
		std::vector<CL_DNSResourceRecord> records = resolver.lookup_resource(name, "A", 30*1000);
		std::vector<CL_DNSResourceRecord>::size_type pos, size;
		size = records.size();
		for (pos = 0; pos < size; pos++)
		{
			CL_Console::write_line("Found answer: %1", records[pos].get_a_address_str());
		}
	}
	catch (CL_Exception e)
	{
		CL_Console::write_line(e.message);
	}
	return 0;
}
