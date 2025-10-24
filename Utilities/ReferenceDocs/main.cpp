
#include "main.h"
#include <iostream>
#include <fstream>

/////////////////////////////////////////////////////////////////////////////
// RefClass

void RefClass::setGroupName(const std::string &group_string)
{
	int i1 = group_string.find('/');
	if (i1 < 0)
	{
		if (group_string.size() > 0)
		{
			// just no subgroup
			i1 = group_string.size();
		}
		else
		{
			groupString = "Incorrect/no Group";
			subGroupName = "no Group";
			groupName = "Incorrect";
			return;
		}
	}
	groupName = group_string.substr(0,i1);
	printf("dba3 information %d %d",i1, group_string.size());
	if (i1+1 >= group_string.size()) { subGroupName=""; }
	else { subGroupName = group_string.substr(i1+1,group_string.size()-i1); }
	groupString = group_string;
}

RefClass::RefClass(const std::string &class_name,const std::string &group_string)
{
	className = class_name;
	setGroupName(group_string);
}

RefClass::~RefClass()
{
}

bool groupsort(RefClass &u1,RefClass &u2)
{
	return (u1.groupString.compare(u2.groupString) < 0);
}

bool indexsort(RefClass &u1,RefClass &u2)
{
	return (u1.className.compare(u2.className) < 0);
}

/////////////////////////////////////////////////////////////////////////////
// ReferenceDocs startup:

ReferenceDocs app;

int ReferenceDocs::main(int argc, char **argv)
{
	CL_SetupCore::init();
	slots.connect(CL_Log::sig_log(), this, &ReferenceDocs::on_log_message);

	try
	{
		CL_DirectoryScanner scanner;
		bool result = scanner.scan("../../Documentation/Reference/doxyoutput/xml", "*.xml");
		if (!result)
		{
			CL_Log::log("parser", "Unable to find xml doc input!");
			return 255;
		}
		
		while (scanner.next())
		{
			std::string filename = scanner.get_pathname();
			CL_Log::log("spam", "Parsing %1", filename);
		
			CL_DomDocument doc(new CL_InputSource_File(filename), true);
			CL_DomElement doc_element = doc.get_document_element();
			if (doc_element.get_tag_name() != "doxygen")
			{
				CL_Log::log("error", "Not a doxygen doxygen document: %1 (%2)", filename, doc_element.get_tag_name());
				continue;
			}
			
			CL_DomNode cur_node = doc_element.get_first_child();
			while (!cur_node.is_null())
			{
				if (cur_node.is_element())
				{
					CL_DomElement cur_element = cur_node.to_element();
					if (cur_element.get_tag_name() == "compounddef")
					{
						parse_compounddef(cur_element);
					}
				}

				cur_node = cur_node.get_next_sibling();
			}

		}
		parse_groupedclasses();
		parse_allclasses();
		parse_indexedclasses();
	}
	catch (CL_Error err)
	{
		CL_Log::log("ClanLib ReferenceDocs Fatal Error: %1", err.message.c_str());
	}

	CL_SetupCore::deinit();

	return 0;
}

void ReferenceDocs::parse_compounddef(CL_DomElement compounddef)
{
	if (compounddef.get_tag_name() != "compounddef")
	{
		CL_Log::log("debug", "Tag name is wrong. Expected 'compounddef' but got '%1'", compounddef.get_tag_name());
		return;
	}
			
	// Right now, we only deal with classes.
	if (compounddef.get_attribute("kind") != "class")
	{
		CL_Log::log("spam", "Wrong compound kind. Expected 'class' but got '%1", compounddef.get_attribute("kind"));
		return;
	}
			
	std::string class_name = "unknown";
	std::vector<std::string> base_classes, derived_classes, sections;
	std::string brief, detail, clan_group, clan_section, clan_header;
	bool ishide = false;

	CL_DomNode cur_node;
	for (cur_node = compounddef.get_first_child(); !cur_node.is_null(); cur_node = cur_node.get_next_sibling())
	{
		CL_DomElement cur_element = cur_node.to_element();
		if (cur_element.is_null()) continue;
		
		std::string tag_name = cur_element.get_tag_name();

		if (tag_name == "compoundname")
		{
			class_name = cur_element.get_text();
		}
		else if (tag_name == "basecompoundref")
		{
			std::string flags;
			if (cur_element.has_attribute("prot")) flags += cur_element.get_attribute("prot")+" ";
			if (cur_element.get_attribute("virt") == "virtual") flags += "virtual ";
			std::string str = CL_String::format("%1<a href=\"%2.html\" class=\"reflink\">%3</a>", flags, cur_element.get_text(), cur_element.get_text());
			base_classes.push_back(str);
		}
		else if (tag_name == "derivedcompoundref")
		{
			std::string str = CL_String::format("<a href=\"%1.html\" class=\"reflink\">%2</a>", cur_element.get_text(), cur_element.get_text());
			derived_classes.push_back(str);
		}
		else if (tag_name == "sectiondef" && cur_element.get_attribute("kind") == "user-defined")
		{
			std::string section = parse_sectiondef(class_name, cur_element);
			sections.push_back(section);
		}
		else if (tag_name == "briefdescription")
		{
			brief = get_formatted(cur_element);
		}
		else if (tag_name == "detaileddescription")
		{
			detail = get_formatted(cur_element);
			// extracting the ClanGroup information
			int m1 = detail.find("!hide!");
			if (m1 >= 0)
			{
				ishide = true; int m2 = m1+6;
				detail = std::string() + detail.substr(0,m1) + detail.substr(m2,detail.size()-m2);
			}
			int i1 = detail.find("!group=");
			int i3 = i1 + 7;
			int i2 = detail.find('!',i1+1);
			if (i1+2 >= i2 || i1 < 0)
			{
				if (!ishide) { CL_Log::log("spam", "No Groupinformation at '%1", class_name); }
			}
			else
			{
				clan_group = detail.substr(i3,i2-i3);
				std::string s1 = detail.substr(0,i1);
				std::string s2 = detail.substr(i2+1,detail.size()-i2);
				detail = std::string() + detail.substr(0,i1) + detail.substr(i2+1,detail.size()-i2);
				printf("DETAIL %d %d \"%s\" \"%s\" \"%s\"",i1,i2,detail.c_str(),s1.c_str(),s2.c_str());
			}
		}
	}

	// sometimes you have to hack to make something look nice
	RefClass refcl = RefClass(class_name,clan_group);
	clan_group = refcl.groupName;
	clan_section = refcl.subGroupName;

	std::ofstream class_file(CL_String::format("/home/mbn/public_html/temp/reftest/%1.html", class_name).c_str(), std::ofstream::out);
	class_file << "<!-- clanlib header begin -->" << std::endl;
	class_file << "<HTML>" << std::endl;
	class_file << "<HEAD>" << std::endl;
	class_file << "<TITLE>" << class_name.c_str() << " - ClanLib Game SDK</TITLE>" << std::endl;
	class_file << "<STYLE TYPE=\"text/css\"><!--" << std::endl;
	class_file << "HTML BODY" << std::endl;
	class_file << "{" << std::endl;
	class_file << "        font-family: verdana, helvetica, sans-serif;" << std::endl;
	class_file << "        font-size: 12px;" << std::endl;
	class_file << "}" << std::endl;
	class_file << "H1 { font-size: 22px; }" << std::endl;
	class_file << "H2 { font-size: 18px; }" << std::endl;
	class_file << "H3 { font-size: 16px; }" << std::endl;
	class_file << "H4 { font-size: 14px; }" << std::endl;
	class_file << "P { font-size: 12px; }" << std::endl;
	class_file << "LI { font-size: 12px; }" << std::endl;
	class_file << ".reflink:link { text-decoration: none; font-weight: bold; color: black; }" << std::endl;
	class_file << ".reflink:visited { text-decoration: none; font-weight: bold; color: black; }" << std::endl;
	class_file << ".reflink:active { text-decoration: none; font-weight: bold; color: black; }" << std::endl;
	class_file << ".reflink:hover { text-decoration: underline; font-weight: bold; color: black; }" << std::endl;
	class_file << "--></STYLE>" << std::endl;
	class_file << "</HEAD>" << std::endl;
	class_file << "<body bgcolor=white text=black link=blue vlink=#800080>" << std::endl;
	class_file << "<center>" << std::endl;
	class_file << "<img src=\"http://clanlib.org/gfx/clanlib.png\">" << std::endl;
	class_file << "</center>" << std::endl;
	class_file << "<!-- clanlib header end -->" << std::endl;
	class_file << "<center>" << std::endl;
	class_file << "<p>" << std::endl;
	class_file << "<a href=\"http://clanlib.org/docs.html\">Home</a> |" << std::endl;
	class_file << "<a href=\"classes.html\">All Classes</a> |" << std::endl;
	class_file << "<a href=\"modules.html\">Grouped Classes</a> |" << std::endl;
	class_file << "<a href=\"index.html\">Index</a> |" << std::endl;
	class_file << "<a href=\"search.html\">Search</a>" << std::endl;
	class_file << "</p>" << std::endl;
	class_file << "</center>" << std::endl;

	class_file << "<h1>Class " << class_name.c_str() << "</h1>" << std::endl;
	class_file << "<p>" << brief.c_str();
	if (!detail.empty()) class_file << " <a href=\"#description\">More...</a>";
	class_file << "</p>" << std::endl;
	class_file << "<table>" << std::endl;
	class_file << "<tr><td valign=top nowrap><p>Derived from:</p></td><td valign=top><p>" << std::endl;
	if (!base_classes.empty())
	{
		for (int i=0; i<base_classes.size()-1; i++)
			class_file << base_classes[i].c_str() << ", ";
		
		class_file << base_classes[base_classes.size()-1].c_str();
	}
	else
	{
		class_file << "<i>None</i>";
	}
	class_file << "</p></td></tr>" << std::endl;
	class_file << "<tr><td valign=top><p>Derived by:</p></td><td valign=top><p>" << std::endl;
	if (!derived_classes.empty())
	{
		for (int i=0; i<derived_classes.size()-1; i++)
			class_file << derived_classes[i].c_str() << ", ";
		
		class_file << derived_classes[derived_classes.size()-1].c_str();
	}
	else
	{
		class_file << "<i>None</i>";
	}
	class_file << "</p></td></tr>" << std::endl;
	class_file << "<tr><td><p>Group:</p></td><td><p>" << clan_group.c_str() << " (" << clan_section.c_str() << ")</p></td></tr>" << std::endl;
	class_file << "</table>" << std::endl;
	class_file << "<p>#include &lt;ClanLib/" << clan_header.c_str() << "&gt;</p>" << std::endl;
	
	for (int section_index = 0; section_index < sections.size(); section_index++)
	{
		class_file << sections[section_index] << std::endl;
	}
	
	if (!detail.empty())
	{
		class_file << "<a name=\"#description\"><p><b>Detailed description:</b></p></a>" << std::endl;
		class_file << detail.c_str() << std::endl;
	}


	// now we need to get the function etc. data
	refcl.sections.swap(sections);
	// adding the class to the reference list
	if (!ishide)
	{
		rlist.push_back(refcl);
	}

	class_file << "<!-- clanlib footer begin -->" << std::endl;
	class_file << "<center><br><br><font color=\"#a0a0a0\">" << std::endl;
	class_file << "Questions or comments, write to the <a href=\"http://clanlib.org/contact.html\">ClanLib mailing list</a>." << std::endl;
	class_file << "</font></center>" << std::endl;
	class_file << "<!-- clanlib footer end -->" << std::endl;
	class_file << "</body>" << std::endl;
	class_file << "</html>" << std::endl;
}

std::string ReferenceDocs::parse_sectiondef(const std::string &class_name, CL_DomElement sectiondef)
{
	std::string text;
	
	std::string section_name;
	std::vector<std::string> members;
	
	std::map<std::string, int> member_map;
	
	CL_DomNode cur_node;
	for (cur_node = sectiondef.get_first_child(); !cur_node.is_null(); cur_node = cur_node.get_next_sibling())
	{
		CL_DomElement cur_element = cur_node.to_element();
		if (cur_element.is_null()) continue;
		
		std::string tag_name = cur_element.get_tag_name();
		
		if (tag_name == "header")
		{
			section_name = cur_element.get_text();
			if (section_name == "Implementation")
			{
				// skip implementation group.
				return std::string();
			}
		}
		else if (tag_name == "memberdef")
		{
			std::string member = parse_memberdef(class_name, section_name, member_map, cur_element);
			if (!member.empty()) members.push_back(member);
		}
	}

	text += CL_String::format("<p><b>%1:</b></p>", section_name);
	text += "<table border=0 width=100%>";
	for (int i=0; i<members.size(); i++)
	{
		text += members[i];
	}
	text += "</table>";
	
	return text;
}

std::string ReferenceDocs::parse_memberdef(const std::string &class_name, const std::string &section_name, std::map<std::string, int> &member_map, CL_DomElement memberdef)
{
	std::string text;

	std::string kind = memberdef.get_attribute("kind");
	bool member_static = (memberdef.get_attribute("static") == "yes");
	bool member_const = (memberdef.get_attribute("const") == "yes");
	bool member_explicit = (memberdef.get_attribute("explicit") == "yes");
	bool member_inline = (memberdef.get_attribute("inline") == "yes");
	bool member_virtual = (memberdef.get_attribute("virtual") == "virtual");
	std::string prot = memberdef.get_attribute("prot", "public");
	
	if (prot == "private") return text;
	
	std::string type, name, definition, argsstring, brief, detail;
	std::string params_description;
	std::vector<std::string> params;
	
	CL_DomNode cur_node;
	for (cur_node = memberdef.get_first_child(); !cur_node.is_null(); cur_node = cur_node.get_next_sibling())
	{
		CL_DomElement cur_element = cur_node.to_element();
		if (cur_element.is_null()) continue;

		std::string tag_name = cur_element.get_tag_name();
		
		if (tag_name == "type")
		{
			type = cur_element.get_text();
		}
		else if (tag_name == "definition")
		{
			definition = cur_element.get_text();
		}
		else if (tag_name == "argsstring")
		{
			argsstring = cur_element.get_text();
		}
		else if (tag_name == "name")
		{
			name = cur_element.get_text();
			
			// Skip destructors.
			if (!name.empty() && name[0] == '~') return std::string();
		}
		else if (tag_name == "param")
		{
			std::string param_type = get_formatted(cur_element.named_item("type").to_element());
			std::string param_name = cur_element.named_item("declname").to_element().get_text();
			std::string param_defval = cur_element.named_item("defval").to_element().get_text();
			
			std::string param_text = param_type + " " + param_name;
			if (!param_defval.empty()) param_text += std::string(" = ") + param_defval;
			params.push_back(param_text);
		}
		else if (tag_name == "briefdescription")
		{
			brief = get_formatted(cur_element);
		}
		else if (tag_name == "detaileddescription")
		{
			detail = get_formatted(cur_element);
			params_description = get_params_list(cur_element);
		}
		else if (tag_name == "location")
		{
		}
	}

	// Group functions:
	std::map<std::string, int>::iterator it = member_map.find(name);
	if (it != member_map.end())
	{
		if (brief.empty()) return std::string();
		it->second++;
	}
	else
	{
		member_map[name] = 0;
	}

	// Generate output:
	std::string filename = CL_String::format("%1_func%2_%3.html", class_name, CL_String::from_int(member_map[name]), name);
	std::string hint;
	if (member_static) hint += " static";
	if (member_const) hint += " const";
	if (member_explicit) hint += " explicit";
	if (member_inline) hint += " inline";
	if (member_virtual) hint += " virtual";
	if (prot == "protected") hint += " protected";
	
	text += CL_String::format(
		"<tr>"
		"<td width=30% valign=top><p><a href=\"%1\">%2</a><i>%3</i></p></td>"
		"<td valign=top><p>%4</p></td>"
		"</tr>",
		filename,
		name,
		hint,
		brief);


	std::ofstream function_file(CL_String::format("/home/mbn/public_html/temp/reftest/%1", filename).c_str(), std::ofstream::out);
	function_file << "<!-- clanlib header begin -->" << std::endl;
	function_file << "<HTML>" << std::endl;
	function_file << "<HEAD>" << std::endl;
	function_file << "<TITLE>" << name.c_str() << " - ClanLib Game SDK</TITLE>" << std::endl;
	function_file << "<STYLE TYPE=\"text/css\"><!--" << std::endl;
	function_file << "HTML BODY" << std::endl;
	function_file << "{" << std::endl;
	function_file << "        font-family: verdana, helvetica, sans-serif;" << std::endl;
	function_file << "        font-size: 12px;" << std::endl;
	function_file << "}" << std::endl;
	function_file << "H1 { font-size: 22px; }" << std::endl;
	function_file << "H2 { font-size: 18px; }" << std::endl;
	function_file << "H3 { font-size: 16px; }" << std::endl;
	function_file << "H4 { font-size: 14px; }" << std::endl;
	function_file << "P { font-size: 12px; }" << std::endl;
	function_file << "LI { font-size: 12px; }" << std::endl;
	function_file << ".reflink:link { text-decoration: none; font-weight: bold; color: black; }" << std::endl;
	function_file << ".reflink:visited { text-decoration: none; font-weight: bold; color: black; }" << std::endl;
	function_file << ".reflink:active { text-decoration: none; font-weight: bold; color: black; }" << std::endl;
	function_file << ".reflink:hover { text-decoration: underline; font-weight: bold; color: black; }" << std::endl;
	function_file << "--></STYLE>" << std::endl;
	function_file << "</HEAD>" << std::endl;
	function_file << "<body bgcolor=white text=black link=blue vlink=#800080>" << std::endl;
	function_file << "<center>" << std::endl;
	function_file << "<img src=\"http://clanlib.org/gfx/clanlib.png\">" << std::endl;
	function_file << "</center>" << std::endl;
	function_file << "<!-- clanlib header end -->" << std::endl;
	function_file << "<center>" << std::endl;
	function_file << "<p>" << std::endl;
	function_file << "<a href=\"http://clanlib.org/docs.html\">Home</a> |" << std::endl;
	function_file << "<a href=\"classes.html\">All Classes</a> |" << std::endl;
	function_file << "<a href=\"modules.html\">Grouped Classes</a> |" << std::endl;
	function_file << "<a href=\"index.html\">Index</a> |" << std::endl;
	function_file << "<a href=\"search.html\">Search</a>" << std::endl;
	function_file << "</p>" << std::endl;
	function_file << "</center>" << std::endl;

	function_file << "<h1>Function " << name.c_str() << "</h1>" << std::endl;
	function_file << "<p>" << brief.c_str() << "</p>" << std::endl;
	function_file << "<pre>" << std::endl;
	function_file << "\t" << definition.c_str() << "("; // << argsstring.c_str() << std::endl;
	for (int index_params = 0; index_params < params.size(); index_params++)
	{
		if (index_params > 0) function_file << ",";
		function_file << std::endl << "\t\t" << params[index_params];
	}
	function_file << ");" << std::endl;
	function_file << "</pre>" << std::endl;
	
	if (!params_description.empty())
	{
		function_file << "<p><b>Parameters:</b></p>" << std::endl;
		function_file << params_description.c_str() << std::endl;
	}
/*
        print FILE "<p><b>Return value:</b></p>\n";
        print FILE "<p>";
        print_description(&addCrossRef($funcretval{$filename}));
        print FILE "</p>\n";
*/

	if (!detail.empty())
	{
		function_file << "<p><b>Detailed description:</b></p>" << std::endl;
		function_file << "<p>" << detail.c_str() << "</p>" << std::endl;
        }

	function_file << "<p><b>See also:</b></p>" << std::endl;
	function_file << "<p><a href=\"" << class_name << ".html\">" << class_name << "</a></p>" << std::endl;
/*
      print FILE "<p><b>See also:</b></p>\n";
      $see_also{$classname} = "<a href=\"$classfilename.html\">$classname</a>";

      my $str;
      my $cur_also;
      foreach $cur_also (keys %see_also)
      {
        $str .= " | " if ($str ne "");
        $str .= $see_also{$cur_also};
      }

      print FILE "<p>$str</p>\n";
*/

	function_file << "<!-- clanlib footer begin -->" << std::endl;
	function_file << "<center><br><br><font color=\"#a0a0a0\">" << std::endl;
	function_file << "Questions or comments, write to the <a href=\"http://clanlib.org/contact.html\">ClanLib mailing list</a>." << std::endl;
	function_file << "</font></center>" << std::endl;
	function_file << "<!-- clanlib footer end -->" << std::endl;
	function_file << "</body>" << std::endl;
	function_file << "</html>" << std::endl;

	return text;
}

std::string ReferenceDocs::get_params_list(const CL_DomElement &element)
{
	std::string text;

	CL_DomNode cur_node;
	for (cur_node = element.get_first_child(); !cur_node.is_null(); cur_node = cur_node.get_next_sibling())
	{
		if (!cur_node.is_element()) continue;
		
		CL_DomElement cur_element = cur_node.to_element();
		if (cur_element.get_tag_name() == "parameterlist")
		{
			std::string kind = cur_element.get_attribute("kind");
			if (kind != "param") continue;

			text += "<dl>";
			
			std::string param_name, param_description;
			CL_DomNode param_node;
			for (param_node = cur_element.get_first_child(); !param_node.is_null(); param_node = param_node.get_next_sibling())
			{
				if (!param_node.is_element()) continue;
				CL_DomElement param_element = param_node.to_element();
				
				if (param_element.get_tag_name() == "parametername")
				{
					param_name = param_element.get_text();
				}
				else if (param_element.get_tag_name() == "parameterdescription")
				{
					param_description = get_formatted(param_element);
					text += CL_String::format("<dt><i>%1</i></dt><dd>%2</dd>", param_name, param_description);
				}
			}
			
			text += "</dl>";
		}
		else
		{
			text += get_params_list(cur_element);
		}
	}
	
	return text;
}

std::string ReferenceDocs::get_formatted(const CL_DomElement &element)
{
	std::string text;

	CL_DomNode cur_node;
	for (cur_node = element.get_first_child(); !cur_node.is_null(); cur_node = cur_node.get_next_sibling())
	{
		if (cur_node.is_text())
		{
			CL_DomText cur_text = cur_node.to_text();
			text += cur_node.get_node_value();
		}
		else if (cur_node.is_element())
		{
			CL_DomElement cur_element = cur_node.to_element();
			if (cur_element.get_tag_name() == "sp")
			{
				text += " ";
			}
			else if (cur_element.get_tag_name() == "para")
			{
				if (text.empty()) text = get_formatted(cur_element);
				else text += "<p>" + get_formatted(cur_element) + "</p>";
			}
			else if (cur_element.get_tag_name() == "ref")
			{
				std::string link = "null";
				text += CL_String::format(
					" <a href=\"%1\" class=\"reflink\">%2</a> ",
					link,
					get_formatted(cur_element));
			}
		}
	}
	
	return text;
}

void ReferenceDocs::parse_groupedclasses()
{
	const int tabrows = 4;
	// sorting the list
	rlist.sort(groupsort);
	// calculating the lines
	int lines = 0;

	std::list<RefClass>::iterator it1 = rlist.begin();

	if (it1 == rlist.end())
	{
		CL_Log::log("error", "Empty class list, can't create reference");
		return;
	}

	std::list<RefClass>::iterator it2 = it1;
	lines += 3; // group and subgroup of the first element and the first element itself
	++it1;
	while (it1 != rlist.end())
	{
		if ((*it1).groupString == (*it2).groupString)
		{
			// only a new element
			lines += 1;
		}
		else if ((*it1).groupName == (*it2).groupName)
		{
			// space, new subgroup, the element
			lines += 3;
		}
		else
		{
			// space, new group, new subgroup, the element
			lines += 4;
		}
		it2 = it1; ++it1;
	}

	int newtabevery = (lines/tabrows)+1;


	std::ofstream function_file("/home/mbn/public_html/temp/reftest/modules.html", std::ofstream::out);
	function_file << "<!-- clanlib header begin -->" << std::endl;
	function_file << "<HTML>" << std::endl;
	function_file << "<HEAD>" << std::endl;
	function_file << "<TITLE>" << "Grouped Classes" << " - ClanLib Game SDK</TITLE>" << std::endl;
	function_file << "<STYLE TYPE=\"text/css\"><!--" << std::endl;
	function_file << "HTML BODY" << std::endl;
	function_file << "{" << std::endl;
	function_file << "        font-family: verdana, helvetica, sans-serif;" << std::endl;
	function_file << "        font-size: 12px;" << std::endl;
	function_file << "}" << std::endl;
	function_file << "H1 { font-size: 22px; }" << std::endl;
	function_file << "H2 { font-size: 18px; }" << std::endl;
	function_file << "H3 { font-size: 16px; }" << std::endl;
	function_file << "H4 { font-size: 14px; }" << std::endl;
	function_file << "P { font-size: 12px; }" << std::endl;
	function_file << "LI { font-size: 12px; }" << std::endl;
	function_file << ".reflink:link { text-decoration: none; font-weight: bold; color: black; }" << std::endl;
	function_file << ".reflink:visited { text-decoration: none; font-weight: bold; color: black; }" << std::endl;
	function_file << ".reflink:active { text-decoration: none; font-weight: bold; color: black; }" << std::endl;
	function_file << ".reflink:hover { text-decoration: underline; font-weight: bold; color: black; }" << std::endl;
	function_file << "--></STYLE>" << std::endl;
	function_file << "</HEAD>" << std::endl;
	function_file << "<body bgcolor=white text=black link=blue vlink=#800080>" << std::endl;
	function_file << "<center>" << std::endl;
	function_file << "<img src=\"http://clanlib.org/gfx/clanlib.png\">" << std::endl;
	function_file << "</center>" << std::endl;
	function_file << "<!-- clanlib header end -->" << std::endl;
	function_file << "<center>" << std::endl;
	function_file << "<p>" << std::endl;
	function_file << "<a href=\"http://clanlib.org/docs.html\">Home</a> |" << std::endl;
	function_file << "<a href=\"classes.html\">All Classes</a> |" << std::endl;
	function_file << "<a href=\"modules.html\">Grouped Classes</a> |" << std::endl;
	function_file << "<a href=\"index.html\">Index</a> |" << std::endl;
	function_file << "<a href=\"search.html\">Search</a>" << std::endl;
	function_file << "</p>" << std::endl;
	function_file << "</center>" << std::endl;


	function_file << "<h1>Grouped Classes</h1>" << std::endl;
	function_file << "<table width=100%>" << std::endl;
	function_file << "<tr><td valign=top><p>" << std::endl;
	int intabline = 0;
	int sumintablines = 0;
	it1 = rlist.begin();
	it2 = rlist.begin(); bool b1 = true; // b1 is true at the first time
	while (it1 != rlist.end())
	{
		bool needspace = false;
		bool needgroup = false;
		bool needsubgroup = false;
		if (b1)
		{
			//necessary because it2 is not valid
			intabline = 3; needgroup = true; needsubgroup = true;
			b1 = false;
		}
		else
		{
			if ((*it1).groupString == (*it2).groupString)
			{
				// only a new element
				intabline += 1;
			}
			else if ((*it1).groupName == (*it2).groupName)
			{
				// space, new subgroup, the element
				intabline += 3;
				needsubgroup = true; needspace = true;
			}
			else
			{
				// space, new group, new subgroup, the element
				intabline += 4;
				needgroup = true; needsubgroup = true; needspace = true;
			}
		}


		if (intabline > newtabevery)
		{
			function_file << "</p></td>" << std::endl;
			function_file << "<td valign=top><p>" << std::endl;
			sumintablines += intabline;
			intabline = 0; needspace = false;
			if (needgroup) { intabline += 1; }
			if (needsubgroup) { intabline += 1; }
			intabline += 1; // for the next object
		}


		if (needspace)
		{
			function_file << "<br>";
		}
		if (needgroup)
		{
			function_file << "<b>" << (*it1).groupName << "</b><br>" << std::endl;
		}
		if (needsubgroup)
		{
			function_file << "<i>" << (*it1).subGroupName << "</i><br>" << std::endl;
		}
		function_file << "<a href=\"" << (*it1).className << ".html\">" << (*it1).className << "</a><br>" << std::endl;

		it2 = it1;
		++it1;
	}
	sumintablines += intabline;
	printf("\n exspected lines %d real lines %d\n",lines,sumintablines);

	function_file << "</p></td></tr>" << std::endl;
	function_file << "</table>" << std::endl;


	function_file << "<!-- clanlib footer begin -->" << std::endl;
	function_file << "<center><br><br><font color=\"#a0a0a0\">" << std::endl;
	function_file << "Questions or comments, write to the <a href=\"http://clanlib.org/contact.html\">ClanLib mailing list</a>." << std::endl;
	function_file << "</font></center>" << std::endl;
	function_file << "<!-- clanlib footer end -->" << std::endl;
	function_file << "</body>" << std::endl;
	function_file << "</html>" << std::endl;

}

void ReferenceDocs::parse_allclasses()
{
	const int tabrows = 4;
	// sorting the list
	rlist.sort(indexsort);

	std::list<RefClass>::iterator it1 = rlist.begin();

	if (it1 == rlist.end())
	{
		CL_Log::log("error", "Empty class list, can't create reference");
		return;
	}

	// determin the carracter which needs to be indexed
	int indexchar = -1;
	std::list<RefClass>::iterator it2 = it1;
	++it1;
	if (it1 == rlist.end()) { indexchar = 0; }
	else
	{
		int i2 = (*it2).className.size();
		if ((*it1).className.size() < i2) { i2 = (*it1).className.size(); }
		int i1 = 0; while (i1 < i2) { if ((*it1).className[i1] != (*it2).className[i1]) { break; } ++i1; }
		indexchar = i1; // ok we have our begining difference (it could only get smaller)
		it2 = it1;
		while (it1 != rlist.end())
		{
			if ((*it1).className.size() < indexchar) { indexchar = (*it1).className.size(); }
			i1 = 0; while (i1 < indexchar) {if ((*it1).className[i1] != (*it2).className[i1]) { indexchar = i1; break; } ++i1; }
			it2 = it1;
			++it1;
		}
	}
	// ok we have our character
	// calculating the lines
	int lines = 0;
	it1 = rlist.begin();
	it2 = it1; // reset
	lines += 2; // The "A" and the first class
	++it1;
	while (it1 != rlist.end())
	{
		if ((*it1).className[indexchar] == (*it2).className[indexchar])
		{
			// only a new element
			lines += 1;
		}
		else
		{
			// space, the charackter, the element
			lines += 3;
		}
		it2 = it1; ++it1;
	}

	int newtabevery = (lines/tabrows)+1;


	std::ofstream function_file("/home/mbn/public_html/temp/reftest/classes.html", std::ofstream::out);
	function_file << "<!-- clanlib header begin -->" << std::endl;
	function_file << "<HTML>" << std::endl;
	function_file << "<HEAD>" << std::endl;
	function_file << "<TITLE>" << "All Classes" << " - ClanLib Game SDK</TITLE>" << std::endl;
	function_file << "<STYLE TYPE=\"text/css\"><!--" << std::endl;
	function_file << "HTML BODY" << std::endl;
	function_file << "{" << std::endl;
	function_file << "        font-family: verdana, helvetica, sans-serif;" << std::endl;
	function_file << "        font-size: 12px;" << std::endl;
	function_file << "}" << std::endl;
	function_file << "H1 { font-size: 22px; }" << std::endl;
	function_file << "H2 { font-size: 18px; }" << std::endl;
	function_file << "H3 { font-size: 16px; }" << std::endl;
	function_file << "H4 { font-size: 14px; }" << std::endl;
	function_file << "P { font-size: 12px; }" << std::endl;
	function_file << "LI { font-size: 12px; }" << std::endl;
	function_file << ".reflink:link { text-decoration: none; font-weight: bold; color: black; }" << std::endl;
	function_file << ".reflink:visited { text-decoration: none; font-weight: bold; color: black; }" << std::endl;
	function_file << ".reflink:active { text-decoration: none; font-weight: bold; color: black; }" << std::endl;
	function_file << ".reflink:hover { text-decoration: underline; font-weight: bold; color: black; }" << std::endl;
	function_file << "--></STYLE>" << std::endl;
	function_file << "</HEAD>" << std::endl;
	function_file << "<body bgcolor=white text=black link=blue vlink=#800080>" << std::endl;
	function_file << "<center>" << std::endl;
	function_file << "<img src=\"http://clanlib.org/gfx/clanlib.png\">" << std::endl;
	function_file << "</center>" << std::endl;
	function_file << "<!-- clanlib header end -->" << std::endl;
	function_file << "<center>" << std::endl;
	function_file << "<p>" << std::endl;
	function_file << "<a href=\"http://clanlib.org/docs.html\">Home</a> |" << std::endl;
	function_file << "<a href=\"classes.html\">All Classes</a> |" << std::endl;
	function_file << "<a href=\"modules.html\">Grouped Classes</a> |" << std::endl;
	function_file << "<a href=\"index.html\">Index</a> |" << std::endl;
	function_file << "<a href=\"search.html\">Search</a>" << std::endl;
	function_file << "</p>" << std::endl;
	function_file << "</center>" << std::endl;


	function_file << "<h1>All Classes</h1>" << std::endl;
	function_file << "<table width=100%>" << std::endl;
	function_file << "<tr><td valign=top><p>" << std::endl;
	int intabline = 0;
	int sumintablines = 0;
	it1 = rlist.begin();
	it2 = rlist.begin(); bool b1 = true; // b1 is true at the first time
	while (it1 != rlist.end())
	{
		bool needspace = false;
		bool needchar = false;
		if (b1)
		{
			//necessary because it2 is not valid
			intabline = 2; needchar = true;
			b1 = false;
		}
		else
		{
			if ((*it1).className[indexchar] == (*it2).className[indexchar])
			{
				// only a new element
				intabline += 1;
			}
			else
			{
				// space, the charackter, the element
				intabline += 3; needspace = true; needchar = true;
			}
		}


		if (intabline > newtabevery)
		{
			function_file << "</p></td>" << std::endl;
			function_file << "<td valign=top><p>" << std::endl;
			sumintablines += intabline;
			intabline = 0; needspace = false;
			if (needchar) { intabline += 1; }
			intabline += 1; // for the next object
		}


		if (needspace)
		{
			function_file << "<br>";
		}
		if (needchar)
		{
			// there is no need for makeuppercharackter because sorting is case sensetive and Classes should be C lass ;-)
			function_file << "<b>" << (*it1).className[indexchar] << "</b><br>" << std::endl;
		}
		function_file << "<a href=\"" << (*it1).className << ".html\">" << (*it1).className << "</a><br>" << std::endl;

		it2 = it1;
		++it1;
	}
	sumintablines += intabline;
	printf("\n exspected lines %d real lines %d\n",lines,sumintablines);

	function_file << "</p></td></tr>" << std::endl;
	function_file << "</table>" << std::endl;


	function_file << "<!-- clanlib footer begin -->" << std::endl;
	function_file << "<center><br><br><font color=\"#a0a0a0\">" << std::endl;
	function_file << "Questions or comments, write to the <a href=\"http://clanlib.org/contact.html\">ClanLib mailing list</a>." << std::endl;
	function_file << "</font></center>" << std::endl;
	function_file << "<!-- clanlib footer end -->" << std::endl;
	function_file << "</body>" << std::endl;
	function_file << "</html>" << std::endl;

}


void ReferenceDocs::parse_indexedclasses()
{
	std::ofstream function_file("/home/mbn/public_html/temp/reftest/index.html", std::ofstream::out);
	function_file << "<!-- clanlib header begin -->" << std::endl;
	function_file << "<HTML>" << std::endl;
	function_file << "<HEAD>" << std::endl;
	function_file << "<TITLE>" << "Index" << " - ClanLib Game SDK</TITLE>" << std::endl;
	function_file << "<STYLE TYPE=\"text/css\"><!--" << std::endl;
	function_file << "HTML BODY" << std::endl;
	function_file << "{" << std::endl;
	function_file << "        font-family: verdana, helvetica, sans-serif;" << std::endl;
	function_file << "        font-size: 12px;" << std::endl;
	function_file << "}" << std::endl;
	function_file << "H1 { font-size: 22px; }" << std::endl;
	function_file << "H2 { font-size: 18px; }" << std::endl;
	function_file << "H3 { font-size: 16px; }" << std::endl;
	function_file << "H4 { font-size: 14px; }" << std::endl;
	function_file << "P { font-size: 12px; }" << std::endl;
	function_file << "LI { font-size: 12px; }" << std::endl;
	function_file << ".reflink:link { text-decoration: none; font-weight: bold; color: black; }" << std::endl;
	function_file << ".reflink:visited { text-decoration: none; font-weight: bold; color: black; }" << std::endl;
	function_file << ".reflink:active { text-decoration: none; font-weight: bold; color: black; }" << std::endl;
	function_file << ".reflink:hover { text-decoration: underline; font-weight: bold; color: black; }" << std::endl;
	function_file << "--></STYLE>" << std::endl;
	function_file << "</HEAD>" << std::endl;
	function_file << "<body bgcolor=white text=black link=blue vlink=#800080>" << std::endl;
	function_file << "<center>" << std::endl;
	function_file << "<img src=\"http://clanlib.org/gfx/clanlib.png\">" << std::endl;
	function_file << "</center>" << std::endl;
	function_file << "<!-- clanlib header end -->" << std::endl;
	function_file << "<center>" << std::endl;
	function_file << "<p>" << std::endl;
	function_file << "<a href=\"http://clanlib.org/docs.html\">Home</a> |" << std::endl;
	function_file << "<a href=\"classes.html\">All Classes</a> |" << std::endl;
	function_file << "<a href=\"modules.html\">Grouped Classes</a> |" << std::endl;
	function_file << "<a href=\"index.html\">Index</a> |" << std::endl;
	function_file << "<a href=\"search.html\">Search</a>" << std::endl;
	function_file << "</p>" << std::endl;
	function_file << "</center>" << std::endl;


	function_file << "<h1>Index</h1>" << std::endl;


	rlist.sort(indexsort);
	std::list<RefClass>::iterator it1 = rlist.begin();
	while (it1 != rlist.end())
	{
		function_file << "<p><dt>" << (*it1).className << "</dt>" << std::endl;
		function_file << "<dd><a href=\"" << (*it1).className << ".html\">" << (*it1).className << "</a></dd>" << std::endl;
		std::vector<std::string>::iterator it3 = (*it1).sections.begin();
		while (it3 != (*it1).sections.end())
		{
			function_file << "<dd>" << (*it3) << "</dd>" << std::endl;
			++it3;
		}
		function_file << "</p>" << std::endl;
	++it1;
	}

	function_file << "<!-- clanlib footer begin -->" << std::endl;
	function_file << "<center><br><br><font color=\"#a0a0a0\">" << std::endl;
	function_file << "Questions or comments, write to the <a href=\"http://clanlib.org/contact.html\">ClanLib mailing list</a>." << std::endl;
	function_file << "</font></center>" << std::endl;
	function_file << "<!-- clanlib footer end -->" << std::endl;
	function_file << "</body>" << std::endl;
	function_file << "</html>" << std::endl;

}


void ReferenceDocs::on_log_message(const std::string &channel, int level, const std::string &text)
{
	if (channel == "spam") return;
	std::cout << "[" << channel.c_str() << "] " << text.c_str() << std::endl;
}
