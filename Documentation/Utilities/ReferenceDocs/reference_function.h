
#ifndef header_reference_function
#define header_reference_function

#include <ClanLib/core.h>

struct ReferenceFunctionParameter
{
	CL_String name;
	CL_String description;
};

class ReferenceFunction
{
public:
	CL_String name;
	CL_String declaration;
	std::vector<ReferenceFunctionParameter> parameters;
	CL_String return_value;
	CL_String brief_description;
	CL_String detailed_description;
	CL_String see_also;

	void load(CL_String member_name, CL_DomElement class_element);
	void save(const CL_StringRef &filename);

private:
	void parse_sectiondef(CL_DomElement sectiondef);
	void parse_memberdef(CL_DomElement memberdef);
	void get_params_list(const CL_DomElement &element);
};

#endif
