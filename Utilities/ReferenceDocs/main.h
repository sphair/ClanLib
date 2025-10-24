
#ifndef header_main
#define header_main

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <stdlib.h>

class RefClass
{
public:
	std::string className;
	std::string groupName;
	std::string subGroupName;
	std::string groupString; // the String groupName/subGroupName

	std::vector<std::string> sections;

	void setGroupName(const std::string &group_string);

	RefClass(const std::string &class_name,const std::string &group_string);

	~RefClass();
};

bool groupsort(RefClass &u1,RefClass &u2);
bool indexsort(RefClass &u1,RefClass &u2);

class ReferenceDocs : public CL_ClanApplication
{
public:
	virtual int main(int argc, char **argv);

private:
	void parse_compounddef(CL_DomElement compounddef);

	std::string parse_sectiondef(const std::string &class_name, CL_DomElement sectiondef);

	std::string parse_memberdef(const std::string &class_name, const std::string &section_name, std::map<std::string, int> &member_map, CL_DomElement memberdef);

	void parse_groupedclasses();

	void parse_allclasses();

	void parse_indexedclasses();

	std::string ReferenceDocs::get_params_list(const CL_DomElement &element);

	std::string get_formatted(const CL_DomElement &element);
	
	void on_log_message(const std::string &channel, int level, const std::string &text);
	
	CL_SlotContainer slots;

	std::list<RefClass> rlist;
};

#endif
