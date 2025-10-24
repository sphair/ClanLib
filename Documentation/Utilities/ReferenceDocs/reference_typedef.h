
#ifndef header_reference_typedef
#define header_reference_typedef

#include <ClanLib/core.h>

class ReferenceTypedef
{
public:
	CL_String name;
	CL_String value;

	void load(CL_DomDocument document);
	void save(const CL_StringRef &filename);
};

#endif
