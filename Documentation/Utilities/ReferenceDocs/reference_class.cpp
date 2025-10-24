
#include "reference_class.h"
#include "reference_docs.h"
#include "reference_function.h"

ReferenceClass::ReferenceClass()
: hidden(false)
{
}

void ReferenceClass::load(CL_DomElement element)
{
	if (element.get_tag_name() != "compounddef")
		throw CL_Exception("Expected compounddef element");
	parse_compounddef(element);
}

void ReferenceClass::save(const CL_StringRef &filename)
{
	CL_File class_file(filename, CL_File::create_always, CL_File::access_write);
	CL_String html = cl_format(
		"<!-- clanlib header begin -->\n"
		"<HTML>\n"
		"<HEAD>\n"
		"<TITLE>%1 - ClanLib Game SDK</TITLE>\n"
		"<STYLE TYPE=\"text/css\"><!--\n"
		"HTML BODY"
		"{"
		"        font-family: verdana, helvetica, sans-serif;"
		"        font-size: 12px;"
		"}"
		"H1 { font-size: 22px; }"
		"H2 { font-size: 18px; }"
		"H3 { font-size: 16px; }"
		"H4 { font-size: 14px; }"
		"P { font-size: 12px; }"
		"LI { font-size: 12px; }"
		".reflink:link { text-decoration: none; font-weight: bold; color: black; }"
		".reflink:visited { text-decoration: none; font-weight: bold; color: black; }"
		".reflink:active { text-decoration: none; font-weight: bold; color: black; }"
		".reflink:hover { text-decoration: underline; font-weight: bold; color: black; }"
		"--></STYLE>"
		"</HEAD>"
		"<body bgcolor=white text=black link=blue vlink=#800080>"
		"<center>"
		"<img src=\"http://clanlib.org/gfx/clanlib.png\">"
		"</center>"
		"<!-- clanlib header end -->"
		"<center>"
		"<p>"
//		"<a href=\"http://clanlib.org/docs.html\">Home</a> |"
		"<a href=\"classes.html\">All Classes</a> |"
		"<a href=\"modules.html\">Grouped Classes</a> |"
		"<a href=\"index.html\">Index</a>"
//		"<a href=\"search.html\">Search</a>"
		"</p>"
		"</center>",
		name);
	class_file.write(html.data(), html.length());

	html = cl_format("<h1>Class %1</h1><p>%2%3</p>", name, brief_description, detailed_description.empty() ? "" : " <a href=\"#description\">More...</a>");
	class_file.write(html.data(), html.length());

	html = "<table><tr><td valign=top nowrap><p>Derived from:</p></td><td valign=top><p>";
	class_file.write(html.data(), html.length());

	if (!base_classes.empty())
	{
		for (unsigned int i=0; i<base_classes.size()-1; i++)
		{
			html = cl_format("%1, ", base_classes[i]);
			class_file.write(html.data(), html.length());
		}
		
		html = base_classes[base_classes.size()-1];
		class_file.write(html.data(), html.length());
	}
	else
	{
		html = "<i>None</i>";
		class_file.write(html.data(), html.length());
	}
	
	html = "</p></td></tr>"
		"<tr><td valign=top><p>Derived by:</p></td><td valign=top><p>";
	class_file.write(html.data(), html.length());
	
	if (!derived_classes.empty())
	{
		for (unsigned int i=0; i<derived_classes.size()-1; i++)
		{
			html = cl_format("%1, ", derived_classes[i]);
			class_file.write(html.data(), html.length());
		}
		
		html = derived_classes[derived_classes.size()-1];
		class_file.write(html.data(), html.length());
	}
	else
	{
		html = "<i>None</i>";
		class_file.write(html.data(), html.length());
	}
	
	html = cl_format(
		"</p></td></tr>"
		"<tr><td><p>Group:</p></td><td><p>%1 (%2)</p></td></tr>"
		"</table>"
		"<p>#include &lt;ClanLib/%3&gt;</p>",
		clan_group,
		clan_section,
		api_include);
	class_file.write(html.data(), html.length());
	
	if (!groups.empty())
	{
		html = "<p><b>Class Members:</b></p>";
		class_file.write(html.data(), html.length());
	}

	for (unsigned int section_index = 0; section_index < groups.size(); section_index++)
	{
		if (groups[section_index].name == "Implementation")
			continue;
		html = get_group_html(groups[section_index], section_index == 0);
		class_file.write(html.data(), html.length());
	}
	
	if (!CL_StringHelp::trim(detailed_description).empty())
	{
		html = cl_format(
			"<a name=\"#description\"><p><b>Detailed description:</b></p></a>%1",
			detailed_description);
		class_file.write(html.data(), html.length());
	}

	html = "<!-- clanlib footer begin -->"
//		"<center><br><br><font color=\"#a0a0a0\">"
//		"Questions or comments, write to the <a href=\"http://clanlib.org/contact.html\">ClanLib mailing list</a>."
//		"</font></center>"
		"<!-- clanlib footer end -->"
		"</body>"
		"</html>";
	class_file.write(html.data(), html.length());
}

void ReferenceClass::generate_members(CL_DomElement class_element)
{
	for (unsigned int section_index = 0; section_index < groups.size(); section_index++)
	{
		ReferenceClassGroup &group = groups[section_index];
		for (unsigned int i=0; i<group.members.size(); i++)
		{
			const ReferenceClassMember &member = group.members[i];
			if (member.name.substr(0, 1) == "~" || member.name == "operator=")
				continue;

			ReferenceFunction ref_function;
			ref_function.load(member.name, class_element);
			ref_function.save("reftest/" + ReferenceDocs::to_filename(cl_format("%1.%2.html", name, member.name)));
		}
	}
}

CL_String ReferenceClass::get_group_html(const ReferenceClassGroup &group, bool first_group)
{
	CL_String text;
	text += "<table border=0 width=100%>";
	if (!group.name.empty())
		if (!first_group)
			text += cl_format("<tr><td cellspan=2><p><br>%1:</p></td></tr>", group.name);
		else
			text += cl_format("<tr><td cellspan=2><p>%1:</p></td></tr>", group.name);

	int num_functions = 0;
	for (unsigned int i=0; i<group.members.size(); i++)
	{
		const ReferenceClassMember &member = group.members[i];
		if (member.name.substr(0, 1) == "~" || member.name == "operator=")
			continue;
		CL_String member_text = cl_format(
			"<tr>"
			"<td width=30% valign=top><p><a href=\"%1\">%2</a><i>%3</i></p></td>"
			"<td valign=top><p>%4</p></td>"
			"</tr>",
			ReferenceDocs::to_filename(cl_format("%1.%2.html", name, member.name)),
			member.name,
			member.hint,
			member.brief_description);

		text += member_text;
		num_functions++;
	}
	text += "</table>";
	if (num_functions == 0)
		return CL_String();
	return text;
}

void ReferenceClass::parse_compounddef(CL_DomElement compounddef)
{
	if (compounddef.get_attribute("kind") != "class")
		throw CL_Exception("Expected compounddef to be a class");

	CL_DomNode cur_node;
	for (cur_node = compounddef.get_first_child(); !cur_node.is_null(); cur_node = cur_node.get_next_sibling())
	{
		CL_DomElement cur_element = cur_node.to_element();
		if (cur_element.is_null()) continue;
		
		CL_String tag_name = cur_element.get_tag_name();

		if (tag_name == "compoundname")
		{
			name = cur_element.get_text();
		}
		else if (tag_name == "basecompoundref")
		{
			CL_String flags;
			if (cur_element.has_attribute("prot")) flags += cur_element.get_attribute("prot")+" ";
			if (cur_element.get_attribute("virt") == "virtual") flags += "virtual ";
			CL_String str = cl_format("%1<a href=\"%2\" class=\"reflink\">%3</a>", flags, ReferenceDocs::to_filename(cur_element.get_text()+".html"), cur_element.get_text());
			base_classes.push_back(str);
		}
		else if (tag_name == "derivedcompoundref")
		{
			CL_String str = cl_format("<a href=\"%1\" class=\"reflink\">%2</a>", ReferenceDocs::to_filename(cur_element.get_text()+".html"), cur_element.get_text());
			derived_classes.push_back(str);
		}
		else if ( (tag_name == "sectiondef") && (
			(cur_element.get_attribute("kind") == "user-defined") ||
			(cur_element.get_attribute("kind") == "public-func") ||
			(cur_element.get_attribute("kind") == "public-static-func") ) )
		{
			ReferenceClassGroup group = parse_sectiondef(cur_element);
			groups.push_back(group);
		}
		else if (tag_name == "briefdescription")
		{
			brief_description = ReferenceDocs::get_formatted(cur_element);
		}
		else if (tag_name == "detaileddescription")
		{
			detailed_description = ReferenceDocs::get_formatted(cur_element);
			extract_clangroup();
		}
	}
}

void ReferenceClass::extract_clangroup()
{
	// extracting the ClanGroup information
	CL_String group_string;
	int m1 = detailed_description.find("!hide!");
	if (m1 >= 0)
	{
		hidden = true; int m2 = m1+6;
		detailed_description = CL_String() + detailed_description.substr(0,m1) + detailed_description.substr(m2,detailed_description.size()-m2);
	}
	int i1 = detailed_description.find("!group=");
	int i3 = i1 + 7;
	int i2 = detailed_description.find('!',i1+1);
	if (i1+2 >= i2 || i1 < 0)
	{
		// if (!hidden) { cl_log_event("spam", "No Groupinformation at '%1", class_name); }
	}
	else
	{
		group_string = detailed_description.substr(i3,i2-i3);
		CL_String s1 = detailed_description.substr(0,i1);
		CL_String s2 = detailed_description.substr(i2+1,detailed_description.size()-i2);
		detailed_description = CL_String() + detailed_description.substr(0,i1) + detailed_description.substr(i2+1,detailed_description.size()-i2);
		// printf("DETAIL %d %d \"%s\" \"%s\" \"%s\"",i1,i2,detailed_description.c_str(),s1.c_str(),s2.c_str());
	}
	i1 = detailed_description.find("!header=");
	i3 = i1 + 8;
	i2 = detailed_description.find('!',i1+1);
	if (!(i1+2 >= i2 || i1 < 0))
	{
		api_include = detailed_description.substr(i3,i2-i3);
		detailed_description = CL_String() + detailed_description.substr(0,i1) + detailed_description.substr(i2+1,detailed_description.size()-i2);
	}


	i1 = group_string.find('/');
	if (i1 < 0)
	{
		if (group_string.size() > 0)
		{
			// just no subgroup
			i1 = group_string.size();
		}
		else
		{
			group_string = "Incorrect/no group";
			clan_section = "no group";
			clan_group = "Incorrect";
			return;
		}
	}
	clan_section = group_string.substr(0,i1);
	if (i1+1 >= (int) group_string.size())
	{
		clan_group="";
	}
	else
	{
		clan_group = group_string.substr(i1+1,group_string.size()-i1);
	}
}

ReferenceClassGroup ReferenceClass::parse_sectiondef(CL_DomElement sectiondef)
{
	ReferenceClassGroup group;

	std::map<CL_String, int> member_map;
	
	CL_DomNode cur_node;
	for (cur_node = sectiondef.get_first_child(); !cur_node.is_null(); cur_node = cur_node.get_next_sibling())
	{
		CL_DomElement cur_element = cur_node.to_element();
		if (cur_element.is_null()) continue;
		
		CL_String tag_name = cur_element.get_tag_name();
		
		if (tag_name == "header")
		{
			group.name = cur_element.get_text();
		}
		else if (tag_name == "memberdef")
		{
			ReferenceClassMember member = parse_memberdef(group.name, member_map, cur_element);
			if (!member.name.empty())
				group.members.push_back(member);
		}
	}

	return group;
}

ReferenceClassMember ReferenceClass::parse_memberdef(const CL_String &section_name, std::map<CL_String, int> &member_map, CL_DomElement memberdef)
{
	ReferenceClassMember member;

	CL_String kind = memberdef.get_attribute("kind");
	CL_DomNode cur_node;
	for (cur_node = memberdef.get_first_child(); !cur_node.is_null(); cur_node = cur_node.get_next_sibling())
	{
		CL_DomElement cur_element = cur_node.to_element();
		if (cur_element.is_null()) continue;

		CL_String tag_name = cur_element.get_tag_name();
		
		if (tag_name == "name")
		{
			member.name = cur_element.get_text();
			if (member_map.find(member.name) == member_map.end())
			{
				member_map[member.name] = 1;
			}
			else
			{
				member_map[member.name]++;
				return ReferenceClassMember();
			}
		}
		else if (tag_name == "briefdescription")
		{
			member.brief_description = ReferenceDocs::get_formatted(cur_element);
		}
	}

	return member;
}
