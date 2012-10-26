
#ifndef header_reference_docs
#define header_reference_docs

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <map>
#include <vector>
#include <list>
#include "reference_class.h"

class ReferenceDocs
{
public:
	static CL_String get_formatted(const CL_DomElement &element);
	static CL_String escape_code(const CL_String &code);
	static CL_String to_filename(const CL_StringRef &name);

	static std::map<CL_String, CL_String> refid_to_href;
	static std::list<ReferenceClass> class_list;

	static void find_references();
	static void find_compounddef_references(CL_DomElement compound_element, std::vector<CL_String> owners = std::vector<CL_String>());
	static void generate_classes();
	static void generate_grouped_classes();
	static void generate_all_classes();
	static void generate_index();

	static bool groupsort(ReferenceClass &u1, ReferenceClass &u2);
	static bool indexsort(ReferenceClass &u1, ReferenceClass &u2);
};










#ifdef use_old_version

class RefClass
{
public:
	CL_String className;
	CL_String groupName;
	CL_String subGroupName;
	CL_String groupString; // the String groupName/subGroupName

	std::vector<CL_String> sections;

	void setGroupName(const CL_String &group_string);

	RefClass(const CL_String &class_name,const CL_String &group_string);

	~RefClass();
};

bool groupsort(RefClass &u1,RefClass &u2);
bool indexsort(RefClass &u1,RefClass &u2);

class ReferenceDocs : public CL_ClanApplication
{
public:
	virtual int main(int argc, char **argv);
	
private:
	void find_references();

	void find_compounddef_references(CL_DomElement compound_element, std::vector<CL_String> owners = std::vector<CL_String>());

	static CL_String to_filename(const CL_StringRef &name);
	
	void generate_classes();

	void parse_compounddef(CL_DomElement compounddef);

	CL_String parse_sectiondef(const CL_String &class_name, CL_DomElement sectiondef);

	CL_String parse_memberdef(const CL_String &class_name, const CL_String &section_name, std::map<CL_String, int> &member_map, CL_DomElement memberdef);

	CL_String parse_memberdef_func(const CL_String &class_name, const CL_String &section_name, std::map<CL_String, int> &member_map, CL_DomElement memberdef);

	CL_String parse_memberdef_enum(const CL_String &class_name, const CL_String &section_name, std::map<CL_String, int> &member_map, CL_DomElement memberdef);

	void generate_groupedclasses();

	void generate_allclasses();

	void generate_indexedclasses();

	CL_String get_params_list(const CL_DomElement &element);

	CL_String get_formatted(const CL_DomElement &element);
	
	CL_String escape_code(const CL_String &code);
	
	std::list<RefClass> rlist;

	std::map<CL_String, CL_String> refid_to_href;
};
#endif

#endif
