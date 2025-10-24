
#ifndef header_reference_enum
#define header_reference_enum

#include <ClanLib/core.h>

class ReferenceEnum
{
public:
	CL_String name;
	std::vector<std::pair<CL_String, CL_String> > values;

	void load(CL_DomDocument document);
	void save(const CL_StringRef &filename);
};

#endif
