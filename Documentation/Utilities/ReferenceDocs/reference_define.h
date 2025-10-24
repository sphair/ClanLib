
#ifndef header_reference_define
#define header_reference_define

#include <ClanLib/core.h>

class ReferenceDefine
{
public:
	CL_String name;
	CL_String value;

	void load(CL_DomDocument document);
	void save(const CL_StringRef &filename);
};

#endif
