
#ifndef header_reference_class
#define header_reference_class

#include <ClanLib/core.h>
#include <map>

struct ReferenceClassMember
{
	CL_String name;
	enum Type
	{
		type_class,
		type_enum,
		type_define,
		type_function,
		type_typedef
	} type;
	CL_String hint;
	CL_String brief_description;
};

struct ReferenceClassGroup
{
	CL_String name;
	std::vector<ReferenceClassMember> members;
};

class ReferenceClass
{
public:
	ReferenceClass();

	CL_String name;
	std::vector<CL_String> base_classes;
	std::vector<CL_String> derived_classes;
	CL_String clan_group, clan_section;
	CL_String api_include;
	CL_String brief_description;
	CL_String detailed_description;
	CL_String see_also;
	std::vector<ReferenceClassGroup> groups;
	bool hidden;

	void load(CL_DomElement element);
	void save(const CL_StringRef &filename);
	void generate_members(CL_DomElement class_element);

private:
	CL_String get_group_html(const ReferenceClassGroup &group, bool first_group);
	void parse_compounddef(CL_DomElement compounddef);
	ReferenceClassGroup parse_sectiondef(CL_DomElement sectiondef);
	ReferenceClassMember parse_memberdef(const CL_String &section_name, std::map<CL_String, int> &member_map, CL_DomElement memberdef);
	void extract_clangroup();
};

#endif
