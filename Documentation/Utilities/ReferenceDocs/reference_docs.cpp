
#include "reference_docs.h"
#include "reference_class.h"
#include "reference_all_classes.h"
#include "reference_grouped_classes.h"
#include "reference_index.h"

int main(int, char **)
{
	CL_SetupCore setup_core;
	CL_ConsoleLogger console_logger;
	console_logger.enable();

	try
	{
		ReferenceDocs::find_references();
		ReferenceDocs::generate_classes();
		ReferenceDocs::generate_grouped_classes();
		ReferenceDocs::generate_all_classes();
		ReferenceDocs::generate_index();
	}
	catch (CL_Exception err)
	{
		cl_log_event("error", "%1", err.message);
	}

	return 0;
}

std::map<CL_String, CL_String> ReferenceDocs::refid_to_href;

std::list<ReferenceClass> ReferenceDocs::class_list;

CL_String ReferenceDocs::to_filename(const CL_StringRef &name)
{
	CL_String filtered = name;

	CL_String::size_type pos;
	for (pos = filtered.find("::"); pos != CL_String::npos; pos = filtered.find('&', pos+1))
	{
		filtered.replace(pos, 2, ".");
	}

	for (CL_String::size_type i = 0; i < filtered.length(); i++)
	{
		if (
			(filtered[i] >= 'a' && filtered[i] <= 'z') ||
			(filtered[i] >= 'A' && filtered[i] <= 'Z') ||
			(filtered[i] >= '0' && filtered[i] <= '9') ||
			filtered[i] == '_' ||
			filtered[i] == '.')
		{
			continue;
		}
		filtered[i] = '_';
	}
	return filtered;
}

CL_String ReferenceDocs::escape_code(const CL_String &code)
{
	CL_String s = code;
	CL_String::size_type pos;
	for (pos = s.find('&'); pos != CL_String::npos; pos = s.find('&', pos+1))
	{
		s.replace(pos, 1, "&amp;");
	}
	for (pos = s.find('<'); pos != CL_String::npos; pos = s.find('<', pos+1))
	{
		s.replace(pos, 1, "&lt;");
	}
	for (pos = s.find('>'); pos != CL_String::npos; pos = s.find('>', pos+1))
	{
		s.replace(pos, 1, "&gt");
	}
	return s;
}

CL_String ReferenceDocs::get_formatted(const CL_DomElement &element)
{
	CL_String text;

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
				CL_String link = refid_to_href[cur_element.get_attribute("refid")];
				if (!text.empty())
				{
					switch (text[text.length()-1])
					{
					case ' ':
					case '\t':
					case '\n':
						break;
					default:
						text += " ";
					}
				}
				if (link.empty())
				{
					text += get_formatted(cur_element);
				}
				else
				{
					text += cl_format(
						"<a href=\"%1\" class=\"reflink\">%2</a>",
						link,
						get_formatted(cur_element));
				}
				if (!text.empty())
				{
					switch (text[text.length()-1])
					{
					case ' ':
					case '\t':
					case '\n':
						break;
					default:
						text += " ";
					}
				}
			}
		}
	}
	
	return text;
}

void ReferenceDocs::find_references()
{
	CL_DirectoryScanner scanner;
	bool result = scanner.scan("../../Documentation/Reference/doxyoutput/xml", "*.xml");
	if (!result)
		return;

	while (scanner.next())
	{
		CL_String filename = scanner.get_pathname();
		CL_File file(filename);
		CL_DomDocument doc(file, true);
		CL_DomElement doc_element = doc.get_document_element();
		if (doc_element.get_tag_name() != "doxygen")
			continue;
		
		CL_DomNode cur_node = doc_element.get_first_child();
		while (!cur_node.is_null())
		{
			if (cur_node.is_element())
			{
				CL_DomElement cur_element = cur_node.to_element();
				if (cur_element.get_tag_name() == "compounddef")
				{
					find_compounddef_references(cur_element);
				}
			}
		
			cur_node = cur_node.get_next_sibling();
		}
	}
}

void ReferenceDocs::find_compounddef_references(CL_DomElement compound_element, std::vector<CL_String> owners)
{
	CL_String compound_name = compound_element.named_item("compoundname").to_element().get_text();
	refid_to_href[compound_element.get_attribute("id")] = to_filename(compound_name + ".html");

	CL_DomElement members = compound_element.named_item("listofallmembers").to_element();
	if (members.is_null())
		return;
	CL_DomNode cur = members.get_first_child();
	while (!cur.is_null())
	{
		if (cur.is_element() && cur.get_node_name() == "member")
		{
			CL_DomElement member = cur.to_element();
			CL_String refid = member.get_attribute("refid");
			CL_String name = member.named_item("name").to_element().get_text();
			CL_String href = to_filename(compound_name + "__" + name + ".html");
			refid_to_href[refid] = href;
		}
		cur = cur.get_next_sibling();
	}
}

void ReferenceDocs::generate_classes()
{
	CL_DirectoryScanner scanner;
	bool result = scanner.scan("../../Documentation/Reference/doxyoutput/xml", "*.xml");
	if (!result)
		return;

	while (scanner.next())
	{
		CL_String filename = scanner.get_pathname();
		CL_File file(filename);
		CL_DomDocument doc(file, true);
		CL_DomElement doc_element = doc.get_document_element();
		if (doc_element.get_tag_name() != "doxygen")
			continue;
		
		CL_DomNode cur_node = doc_element.get_first_child();
		while (!cur_node.is_null())
		{
			if (cur_node.is_element())
			{
				CL_DomElement cur_element = cur_node.to_element();
				if (cur_element.get_tag_name() == "compounddef")
				{
					CL_String kind = cur_element.get_attribute("kind");

					if (kind == "class")
					{
						ReferenceClass ref_class;
						ref_class.load(cur_element);
						ref_class.save(cl_format("reftest/%1.html", to_filename(ref_class.name)));
						ref_class.generate_members(cur_element);
						cl_log_event("class", ref_class.name);

						if (!ref_class.hidden)
							class_list.push_back(ref_class);
					}
/*					else if (kind == "enum")
					{
						ReferenceEnum ref_enum;
						ref_enum.load(cur_element);
						ref_enum.save(cl_format("reftest/%1.html", to_filename(ref_class.name)));
						cl_log_event("enum", ref_enum.name);
					} */
				}
			}
		
			cur_node = cur_node.get_next_sibling();
		}
	}
}

void ReferenceDocs::generate_grouped_classes()
{
	ReferenceGroupedClasses ref_grouped_classes;
	ref_grouped_classes.load();
	ref_grouped_classes.save("reftest/modules.html");
}

void ReferenceDocs::generate_all_classes()
{
	ReferenceAllClasses ref_all_classes;
	ref_all_classes.load();
	ref_all_classes.save("reftest/classes.html");
}

void ReferenceDocs::generate_index()
{
	ReferenceIndex ref_index;
	ref_index.load();
	ref_index.save("reftest/index.html");
}

bool ReferenceDocs::groupsort(ReferenceClass &u1, ReferenceClass &u2)
{
	if (u1.clan_section == u2.clan_section)
		return u1.clan_group < u2.clan_group;
	else
		return u1.clan_section < u2.clan_section;
}

bool ReferenceDocs::indexsort(ReferenceClass &u1, ReferenceClass &u2)
{
	CL_StringRef n1 = u1.name;
	CL_StringRef n2 = u2.name;
	if (n1.substr(0, 3) == "CL_")
		n1 = n1.substr(3);
	if (n2.substr(0, 3) == "CL_")
		n2 = n2.substr(3);
	return n1 < n2;
}



















#ifdef use_old_version

/////////////////////////////////////////////////////////////////////////////
// RefClass

void RefClass::setGroupName(const CL_String &group_string)
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
	// printf("dba3 information %d %d",i1, group_string.size());
	if (i1+1 >= group_string.size()) { subGroupName=""; }
	else { subGroupName = group_string.substr(i1+1,group_string.size()-i1); }
	groupString = group_string;
}

RefClass::RefClass(const CL_String &class_name,const CL_String &group_string)
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
	CL_SetupCore setup_core;
	CL_ConsoleLogger console_logger;
	console_logger.enable();

	try
	{
		find_references();
		generate_classes();
		generate_groupedclasses();
		generate_allclasses();
		generate_indexedclasses();
	}
	catch (CL_Exception err)
	{
		cl_log_event("error", "%1", err.message);
	}

	return 0;
}

void ReferenceDocs::find_references()
{
	CL_DirectoryScanner scanner;
	bool result = scanner.scan("../../Documentation/Reference/doxyoutput/xml", "*.xml");
	if (!result)
		return;

	while (scanner.next())
	{
		CL_String filename = scanner.get_pathname();
		CL_File file(filename);
		CL_DomDocument doc(file, true);
		CL_DomElement doc_element = doc.get_document_element();
		if (doc_element.get_tag_name() != "doxygen")
			continue;
		
		CL_DomNode cur_node = doc_element.get_first_child();
		while (!cur_node.is_null())
		{
			if (cur_node.is_element())
			{
				CL_DomElement cur_element = cur_node.to_element();
				if (cur_element.get_tag_name() == "compounddef")
				{
					find_compounddef_references(cur_element);
				}
			}
		
			cur_node = cur_node.get_next_sibling();
		}
	}
}

void ReferenceDocs::find_compounddef_references(CL_DomElement compound_element, std::vector<CL_String> owners)
{
	CL_String compound_name = compound_element.named_item("compoundname").to_element().get_text();
	refid_to_href[compound_element.get_attribute("id")] = to_filename(compound_name + ".html");

	CL_DomElement members = compound_element.named_item("listofallmembers").to_element();
	if (members.is_null())
		return;
	CL_DomNode cur = members.get_first_child();
	while (!cur.is_null())
	{
		if (cur.is_element() && cur.get_node_name() == "member")
		{
			CL_DomElement member = cur.to_element();
			CL_String refid = member.get_attribute("refid");
			CL_String name = member.named_item("name").to_element().get_text();
			CL_String href = to_filename(compound_name + "__" + name + ".html");
			refid_to_href[refid] = href;
		}
		cur = cur.get_next_sibling();
	}
}

CL_String ReferenceDocs::to_filename(const CL_StringRef &name)
{
	CL_String filtered = name;
	for (CL_String::size_type i = 0; i < filtered.length(); i++)
	{
		if (
			(filtered[i] >= 'a' && filtered[i] <= 'z') ||
			(filtered[i] >= 'A' && filtered[i] <= 'Z') ||
			(filtered[i] >= '0' && filtered[i] <= '9') ||
			filtered[i] == '_' ||
			filtered[i] == '.')
		{
			continue;
		}
		filtered[i] = '_';
	}
	return filtered;
}

void ReferenceDocs::generate_classes()
{
	CL_DirectoryScanner scanner;
	bool result = scanner.scan("../../Documentation/Reference/doxyoutput/xml", "*.xml");
	if (!result)
		return;

	while (scanner.next())
	{
		CL_String filename = scanner.get_pathname();
		CL_File file(filename);
		CL_DomDocument doc(file, true);
		CL_DomElement doc_element = doc.get_document_element();
		if (doc_element.get_tag_name() != "doxygen")
			continue;
		
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
}

void ReferenceDocs::parse_compounddef(CL_DomElement compounddef)
{
	// Right now we only deal with classes.
	if (compounddef.get_attribute("kind") != "class")
		return;

	CL_String class_name = "unknown";
	std::vector<CL_String> base_classes, derived_classes, sections;
	CL_String brief, detail, clan_group, clan_section, clan_header;
	bool ishide = false;

	CL_DomNode cur_node;
	for (cur_node = compounddef.get_first_child(); !cur_node.is_null(); cur_node = cur_node.get_next_sibling())
	{
		CL_DomElement cur_element = cur_node.to_element();
		if (cur_element.is_null()) continue;
		
		CL_String tag_name = cur_element.get_tag_name();

		if (tag_name == "compoundname")
		{
			class_name = cur_element.get_text();
		}
		else if (tag_name == "basecompoundref")
		{
			CL_String flags;
			if (cur_element.has_attribute("prot")) flags += cur_element.get_attribute("prot")+" ";
			if (cur_element.get_attribute("virt") == "virtual") flags += "virtual ";
			CL_String str = cl_format("%1<a href=\"%2\" class=\"reflink\">%3</a>", flags, to_filename(cur_element.get_text()+".html"), cur_element.get_text());
			base_classes.push_back(str);
		}
		else if (tag_name == "derivedcompoundref")
		{
			CL_String str = cl_format("<a href=\"%1\" class=\"reflink\">%2</a>", to_filename(cur_element.get_text()+".html"), cur_element.get_text());
			derived_classes.push_back(str);
		}
		else if ( (tag_name == "sectiondef") && (
			(cur_element.get_attribute("kind") == "user-defined") ||
			(cur_element.get_attribute("kind") == "public-func") ||
			(cur_element.get_attribute("kind") == "public-static-func") ) )
		{
			CL_String section = parse_sectiondef(class_name, cur_element);
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
				detail = CL_String() + detail.substr(0,m1) + detail.substr(m2,detail.size()-m2);
			}
			int i1 = detail.find("!group=");
			int i3 = i1 + 7;
			int i2 = detail.find('!',i1+1);
			if (i1+2 >= i2 || i1 < 0)
			{
				// if (!ishide) { cl_log_event("spam", "No Groupinformation at '%1", class_name); }
			}
			else
			{
				clan_group = detail.substr(i3,i2-i3);
				CL_String s1 = detail.substr(0,i1);
				CL_String s2 = detail.substr(i2+1,detail.size()-i2);
				detail = CL_String() + detail.substr(0,i1) + detail.substr(i2+1,detail.size()-i2);
				// printf("DETAIL %d %d \"%s\" \"%s\" \"%s\"",i1,i2,detail.c_str(),s1.c_str(),s2.c_str());
			}
			i1 = detail.find("!header=");
			i3 = i1 + 8;
			i2 = detail.find('!',i1+1);
			if (i1+2 >= i2 || i1 < 0)
			{
			}
			else
			{
				clan_header = detail.substr(i3,i2-i3);
				detail = CL_String() + detail.substr(0,i1) + detail.substr(i2+1,detail.size()-i2);
			}
		}
	}

	// sometimes you have to hack to make something look nice
	RefClass refcl = RefClass(class_name,clan_group);
	clan_group = refcl.groupName;
	clan_section = refcl.subGroupName;

	CL_File class_file(cl_format("reftest/%1.html", to_filename(class_name)), CL_File::create_always, CL_File::access_write);
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
		"<a href=\"http://clanlib.org/docs.html\">Home</a> |"
		"<a href=\"classes.html\">All Classes</a> |"
		"<a href=\"modules.html\">Grouped Classes</a> |"
		"<a href=\"index.html\">Index</a> |"
		"<a href=\"search.html\">Search</a>"
		"</p>"
		"</center>",
		class_name);
	class_file.write(html.data(), html.length());

	html = cl_format("<h1>Class %1</h1><p>%2%3</p>", class_name, brief, detail.empty() ? "" : " <a href=\"#description\">More...</a>");
	class_file.write(html.data(), html.length());

	html = "<table><tr><td valign=top nowrap><p>Derived from:</p></td><td valign=top><p>";
	class_file.write(html.data(), html.length());

	if (!base_classes.empty())
	{
		for (int i=0; i<base_classes.size()-1; i++)
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
		for (int i=0; i<derived_classes.size()-1; i++)
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
		clan_header);
	class_file.write(html.data(), html.length());
	
	for (int section_index = 0; section_index < sections.size(); section_index++)
	{
		html = sections[section_index];
		class_file.write(html.data(), html.length());
	}
	
	if (!CL_StringHelp::trim(detail).empty())
	{
		html = cl_format(
			"<a name=\"#description\"><p><b>Detailed description:</b></p></a>%1",
			detail);
		class_file.write(html.data(), html.length());
	}


	// now we need to get the function etc. data
	refcl.sections.swap(sections);
	// adding the class to the reference list
	if (!ishide)
	{
		rlist.push_back(refcl);
	}

	html = "<!-- clanlib footer begin -->"
		"<center><br><br><font color=\"#a0a0a0\">"
		"Questions or comments, write to the <a href=\"http://clanlib.org/contact.html\">ClanLib mailing list</a>."
		"</font></center>"
		"<!-- clanlib footer end -->"
		"</body>"
		"</html>";
	class_file.write(html.data(), html.length());
}

CL_String ReferenceDocs::parse_sectiondef(const CL_String &class_name, CL_DomElement sectiondef)
{
	CL_String text;
	
	CL_String section_name;
	std::vector<CL_String> members;
	
	std::map<CL_String, int> member_map;
	
	CL_DomNode cur_node;
	for (cur_node = sectiondef.get_first_child(); !cur_node.is_null(); cur_node = cur_node.get_next_sibling())
	{
		CL_DomElement cur_element = cur_node.to_element();
		if (cur_element.is_null()) continue;
		
		CL_String tag_name = cur_element.get_tag_name();
		
		if (tag_name == "header")
		{
			section_name = cur_element.get_text();
			if (section_name == "Implementation")
			{
				// skip implementation group.
				return CL_String();
			}
		}
		else if (tag_name == "memberdef")
		{
			CL_String member = parse_memberdef(class_name, section_name, member_map, cur_element);
			if (!member.empty())
				members.push_back(member);
		}
	}

	text += cl_format("<p><b>%1:</b></p>", section_name);
	text += "<table border=0 width=100%>";
	for (int i=0; i<members.size(); i++)
	{
		text += members[i];
	}
	text += "</table>";
	
	return text;
}

CL_String ReferenceDocs::parse_memberdef(const CL_String &class_name, const CL_String &section_name, std::map<CL_String, int> &member_map, CL_DomElement memberdef)
{
	CL_String kind = memberdef.get_attribute("kind");
	if (kind == "enum")
	{
		return parse_memberdef_enum(class_name, section_name, member_map, memberdef);
	}
	else
	{
		return parse_memberdef_func(class_name, section_name, member_map, memberdef);
	}
}

CL_String ReferenceDocs::escape_code(const CL_String &code)
{
	CL_String s = code;
	CL_String::size_type pos;
	for (pos = s.find('&'); pos != CL_String::npos; pos = s.find('&', pos+1))
	{
		s.replace(pos, 1, "&amp;");
	}
	for (pos = s.find('<'); pos != CL_String::npos; pos = s.find('<', pos+1))
	{
		s.replace(pos, 1, "&lt;");
	}
	for (pos = s.find('>'); pos != CL_String::npos; pos = s.find('>', pos+1))
	{
		s.replace(pos, 1, "&gt;");
	}
	return s;
}

CL_String ReferenceDocs::parse_memberdef_func(const CL_String &class_name, const CL_String &section_name, std::map<CL_String, int> &member_map, CL_DomElement memberdef)
{
	CL_String text;

	CL_String kind = memberdef.get_attribute("kind");
	bool member_static = (memberdef.get_attribute("static") == "yes");
	bool member_const = (memberdef.get_attribute("const") == "yes");
	bool member_explicit = (memberdef.get_attribute("explicit") == "yes");
	bool member_inline = (memberdef.get_attribute("inline") == "yes");
	bool member_virtual = (memberdef.get_attribute("virtual") == "virtual");
	CL_String prot = memberdef.get_attribute("prot", "public");
	
	if (prot == "private") return text;
	
	CL_String type, type_formatted, name, definition, argsstring, brief, detail;
	CL_String params_description;
	std::vector<CL_String> params;
	
	CL_DomNode cur_node;
	for (cur_node = memberdef.get_first_child(); !cur_node.is_null(); cur_node = cur_node.get_next_sibling())
	{
		CL_DomElement cur_element = cur_node.to_element();
		if (cur_element.is_null()) continue;

		CL_String tag_name = cur_element.get_tag_name();
		
		if (tag_name == "type")
		{
			type = cur_element.get_text();
			type_formatted = get_formatted(cur_element);
		}
		else if (tag_name == "definition")
		{
			definition = CL_StringHelp::trim(escape_code(cur_element.get_text()));
		}
		else if (tag_name == "argsstring")
		{
			argsstring = cur_element.get_text();
		}
		else if (tag_name == "name")
		{
			name = cur_element.get_text();
			
			// Skip destructors.
			if (!name.empty() && name[0] == '~') return CL_String();
		}
		else if (tag_name == "param")
		{
			CL_String param_type = get_formatted(cur_element.named_item("type").to_element());
			CL_String param_name = cur_element.named_item("declname").to_element().get_text();
			CL_String param_defval = get_formatted(cur_element.named_item("defval").to_element());
			
			CL_String param_text = param_type + " " + param_name;
			if (!param_defval.empty())
				param_text += CL_String(" = ") + param_defval;
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
	
	if (definition.substr(0, type.length()) == type)
	{
		definition = CL_StringHelp::trim(type_formatted + definition.substr(type.length()));
	}

	// Group functions:
	std::map<CL_String, int>::iterator it = member_map.find(name);
	if (it != member_map.end())
	{
		if (brief.empty()) return CL_String();
		it->second++;
	}
	else
	{
		member_map[name] = 0;
	}

	// Generate output:
	CL_String filename = to_filename(class_name + "__" + name + ".html"); // cl_format("%1_func%2_%3.html", class_name, CL_StringHelp::int_to_text(member_map[name]), name);
	CL_String hint;
	if (member_static) hint += " static";
	if (member_const) hint += " const";
	if (member_explicit) hint += " explicit";
	if (member_inline) hint += " inline";
	if (member_virtual) hint += " virtual";
	if (prot == "protected") hint += " protected";
	
	text += cl_format(
		"<tr>"
		"<td width=30% valign=top><p><a href=\"%1\">%2</a><i>%3</i></p></td>"
		"<td valign=top><p>%4</p></td>"
		"</tr>",
		filename,
		name,
		hint,
		brief);


	CL_File function_file(cl_format("reftest/%1", filename), CL_File::create_always, CL_File::access_write);
	CL_String html = cl_format(
		"<!-- clanlib header begin -->"
		"<HTML>"
		"<HEAD>"
		"<TITLE>%1 - ClanLib Game SDK</TITLE>"
		"<STYLE TYPE=\"text/css\"><!--"
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
		"<a href=\"http://clanlib.org/docs.html\">Home</a> |"
		"<a href=\"classes.html\">All Classes</a> |"
		"<a href=\"modules.html\">Grouped Classes</a> |"
		"<a href=\"index.html\">Index</a> |"
		"<a href=\"search.html\">Search</a>"
		"</p>"
		"</center>",
		name);
	function_file.write(html.data(), html.length());

	html = cl_format(
		"<h1>Function %1</h1>"
		"<p>%2</p>"
		"<pre>\n"
		"\t%3(", // << argsstring.c_str()
		name,
		brief,
		definition);
	function_file.write(html.data(), html.length());
	
	for (int index_params = 0; index_params < params.size(); index_params++)
	{
		if (index_params > 0)
			function_file.write(",", 1);
		html = cl_format("\n\t\t%1", params[index_params]);
		function_file.write(html.data(), html.length());
	}
	
	html = ");\n"
		"</pre>";
	function_file.write(html.data(), html.length());
	
	if (!params_description.empty())
	{
		html = cl_format(
			"<p><b>Parameters:</b></p>%1",
			params_description);
		function_file.write(html.data(), html.length());
	}
/*
        print FILE "<p><b>Return value:</b></p>\n";
        print FILE "<p>";
        print_description(&addCrossRef($funcretval{$filename}));
        print FILE "</p>\n";
*/

	if (!CL_StringHelp::trim(detail).empty())
	{
		html = cl_format(
			"<p><b>Detailed description:</b></p>"
			"<p>%1</p>",
			detail);
		function_file.write(html.data(), html.length());
        }

	html = cl_format(
		"<p><b>See also:</b></p>"
		"<p><a href=\"%1.html\">%2</a></p>",
		class_name,
		to_filename(class_name));
	function_file.write(html.data(), html.length());
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

	html = 
		"<!-- clanlib footer begin -->"
		"<center><br><br><font color=\"#a0a0a0\">"
		"Questions or comments, write to the <a href=\"http://clanlib.org/contact.html\">ClanLib mailing list</a>."
		"</font></center>"
		"<!-- clanlib footer end -->"
		"</body>"
		"</html>";
	function_file.write(html.data(), html.length());

	return text;
}

CL_String ReferenceDocs::parse_memberdef_enum(const CL_String &class_name, const CL_String &section_name, std::map<CL_String, int> &member_map, CL_DomElement memberdef)
{
	CL_String text;

	CL_String kind = memberdef.get_attribute("kind");
	bool member_static = (memberdef.get_attribute("static") == "yes");
	bool member_const = (memberdef.get_attribute("const") == "yes");
	bool member_explicit = (memberdef.get_attribute("explicit") == "yes");
	bool member_inline = (memberdef.get_attribute("inline") == "yes");
	bool member_virtual = (memberdef.get_attribute("virtual") == "virtual");
	CL_String prot = memberdef.get_attribute("prot", "public");
	
	if (prot == "private") return text;
	
	CL_String name = memberdef.named_item("name").to_element().get_text();
	CL_String brief = get_formatted(memberdef.named_item("briefdescription").to_element());
	CL_String detail = get_formatted(memberdef.named_item("detaileddescription").to_element());
	
	CL_String definition = cl_format("\tenum %1::%2<br>\t{", class_name, name);
	std::vector<CL_String> value_names;
	std::vector<CL_String> value_initializers;
	std::vector<CL_String> value_descriptions;
	
	bool first_value = true;
	CL_DomNode cur_node;
	for (cur_node = memberdef.get_first_child(); !cur_node.is_null(); cur_node = cur_node.get_next_sibling())
	{
		CL_DomElement cur_element = cur_node.to_element();
		if (cur_element.is_null())
			continue;
		if (cur_element.get_tag_name() != "enumvalue")
			continue;
			
		CL_String name = cur_element.named_item("name").to_element().get_text();
		CL_String brief = get_formatted(cur_element.named_item("briefdescription").to_element());
		CL_String detail = get_formatted(cur_element.named_item("detaileddescription").to_element());
		CL_String initializer = cur_element.named_item("initializer").to_element().get_text();

		
		if (!first_value)
			definition += ",";
		first_value = false;

		if (initializer.empty())
			definition += cl_format("<br>\t\t%1", name);
		else
			definition += cl_format("<br>\t\t%1 = %2", name, initializer);

		if (brief.empty() && detail.empty())
			continue;

		value_names.push_back(name);
		value_initializers.push_back(initializer);
		value_descriptions.push_back(brief + detail);
	}
	definition += "<br>\t};";

	// Group functions:
	std::map<CL_String, int>::iterator it = member_map.find(name);
	if (it != member_map.end())
	{
		if (brief.empty()) return CL_String();
		it->second++;
	}
	else
	{
		member_map[name] = 0;
	}

	// Generate output:
	CL_String filename = to_filename(class_name + "__" + name + ".html"); // cl_format("%1_func%2_%3.html", class_name, CL_StringHelp::int_to_text(member_map[name]), name);
	CL_String hint = " enum";
	if (member_static) hint += " static";
	if (member_const) hint += " const";
	if (member_explicit) hint += " explicit";
	if (member_inline) hint += " inline";
	if (member_virtual) hint += " virtual";
	if (prot == "protected") hint += " protected";
	
	text += cl_format(
		"<tr>"
		"<td width=30% valign=top><p><a href=\"%1\">%2</a><i>%3</i></p></td>"
		"<td valign=top><p>%4</p></td>"
		"</tr>",
		filename,
		name,
		hint,
		brief);


	CL_File function_file(cl_format("reftest/%1", filename), CL_File::create_always, CL_File::access_write);
	CL_String html = cl_format(
		"<!-- clanlib header begin -->"
		"<HTML>"
		"<HEAD>"
		"<TITLE>%1 - ClanLib Game SDK</TITLE>"
		"<STYLE TYPE=\"text/css\"><!--"
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
		"<a href=\"http://clanlib.org/docs.html\">Home</a> |"
		"<a href=\"classes.html\">All Classes</a> |"
		"<a href=\"modules.html\">Grouped Classes</a> |"
		"<a href=\"index.html\">Index</a> |"
		"<a href=\"search.html\">Search</a>"
		"</p>"
		"</center>",
		name);
	function_file.write(html.data(), html.length());

	html = cl_format(
		"<h1>Enum %1</h1>"
		"<p>%2</p>"
		"<pre>\n"
		"%3</pre>",
		name,
		brief,
		definition);
	function_file.write(html.data(), html.length());

	if (!value_names.empty())
	{
		html = "<p><b>Values:</b></p><dl>";
		function_file.write(html.data(), html.length());
		
		for (unsigned int i = 0; i < value_names.size(); i++)
		{
			html = cl_format("<dt><i>%1</i></dt><dd>%2</dd>", value_names[i], value_descriptions[i]);
			function_file.write(html.data(), html.length());
		}
		
		html = "</dl>";
		function_file.write(html.data(), html.length());
	}

	if (!CL_StringHelp::trim(detail).empty())
	{
		html = cl_format(
			"<p><b>Detailed description:</b></p>"
			"<p>%1</p>",
			detail);
		function_file.write(html.data(), html.length());
        }

	html = cl_format(
		"<p><b>See also:</b></p>"
		"<p><a href=\"%1.html\">%2</a></p>",
		class_name,
		to_filename(class_name));
	function_file.write(html.data(), html.length());

	html = 
		"<!-- clanlib footer begin -->"
		"<center><br><br><font color=\"#a0a0a0\">"
		"Questions or comments, write to the <a href=\"http://clanlib.org/contact.html\">ClanLib mailing list</a>."
		"</font></center>"
		"<!-- clanlib footer end -->"
		"</body>"
		"</html>";
	function_file.write(html.data(), html.length());

	return text;
}

CL_String ReferenceDocs::get_params_list(const CL_DomElement &element)
{
	CL_String text;

	CL_DomNode cur_node;
	for (cur_node = element.get_first_child(); !cur_node.is_null(); cur_node = cur_node.get_next_sibling())
	{
		if (!cur_node.is_element()) continue;
		
		CL_DomElement cur_element = cur_node.to_element();
		if (cur_element.get_tag_name() == "parameterlist")
		{
			CL_String kind = cur_element.get_attribute("kind");
			if (kind != "param") continue;

			text += "<dl>";
			
			CL_String param_name, param_description;
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
					text += cl_format("<dt><i>%1</i></dt><dd>%2</dd>", param_name, param_description);
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

CL_String ReferenceDocs::get_formatted(const CL_DomElement &element)
{
	CL_String text;

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
				CL_String link = refid_to_href[cur_element.get_attribute("refid")];
				if (!text.empty())
				{
					switch (text[text.length()-1])
					{
					case ' ':
					case '\t':
					case '\n':
						break;
					default:
						text += " ";
					}
				}
				if (link.empty())
				{
					text += get_formatted(cur_element);
				}
				else
				{
					text += cl_format(
						"<a href=\"%1\" class=\"reflink\">%2</a>",
						link,
						get_formatted(cur_element));
				}
				if (!text.empty())
				{
					switch (text[text.length()-1])
					{
					case ' ':
					case '\t':
					case '\n':
						break;
					default:
						text += " ";
					}
				}
			}
		}
	}
	
	return text;
}

void ReferenceDocs::generate_groupedclasses()
{
	const int tabrows = 4;
	// sorting the list
	rlist.sort(groupsort);
	// calculating the lines
	int lines = 0;

	std::list<RefClass>::iterator it1 = rlist.begin();

	if (it1 == rlist.end())
	{
		cl_log_event("error", "Empty class list, can't create reference");
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


	CL_File modules_file("reftest/modules.html", CL_File::create_always, CL_File::access_write);
	CL_String html =
		"<!-- clanlib header begin -->"
		"<HTML>"
		"<HEAD>"
		"<TITLE>Grouped Classes - ClanLib Game SDK</TITLE>"
		"<STYLE TYPE=\"text/css\"><!--"
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
		"<a href=\"http://clanlib.org/docs.html\">Home</a> |"
		"<a href=\"classes.html\">All Classes</a> |"
		"<a href=\"modules.html\">Grouped Classes</a> |"
		"<a href=\"index.html\">Index</a> |"
		"<a href=\"search.html\">Search</a>"
		"</p>"
		"</center>"
		"<h1>Grouped Classes</h1>"
		"<table width=100%>"
		"<tr><td valign=top><p>";
	modules_file.write(html.data(), html.length());

	int intabline = 0;
	int sumintablines = 0;
	it1 = rlist.begin();
	it2 = rlist.begin();
	bool b1 = true; // b1 is true at the first time
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
			html = "</p></td>"
				"<td valign=top><p>";
			modules_file.write(html.data(), html.length());
			sumintablines += intabline;
			intabline = 0; needspace = false;
			if (needgroup) { intabline += 1; }
			if (needsubgroup) { intabline += 1; }
			intabline += 1; // for the next object
		}


		if (needspace)
		{
			modules_file.write("<br>", 4);
		}
		if (needgroup)
		{
			html = cl_format("<b>%1</b><br>", (*it1).groupName);
			modules_file.write(html.data(), html.length());
		}
		if (needsubgroup)
		{
			html = cl_format("<i>%1</i><br>", (*it1).subGroupName);
			modules_file.write(html.data(), html.length());
		}
		html = cl_format("<a href=\"%1.html\">%2</a><br>", (*it1).className, (*it1).className);
		modules_file.write(html.data(), html.length());

		it2 = it1;
		++it1;
	}
	sumintablines += intabline;

	html = "</p></td></tr>"
		"</table>";
	modules_file.write(html.data(), html.length());

	html = "<!-- clanlib footer begin -->"
		"<center><br><br><font color=\"#a0a0a0\">"
		"Questions or comments, write to the <a href=\"http://clanlib.org/contact.html\">ClanLib mailing list</a>."
		"</font></center>"
		"<!-- clanlib footer end -->"
		"</body>"
		"</html>";
	modules_file.write(html.data(), html.length());
}

void ReferenceDocs::generate_allclasses()
{
	const int tabrows = 4;
	// sorting the list
	rlist.sort(indexsort);

	std::list<RefClass>::iterator it1 = rlist.begin();

	if (it1 == rlist.end())
	{
		cl_log_event("error", "Empty class list, can't create reference");
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


	CL_File classes_file("reftest/classes.html", CL_File::create_always, CL_File::access_write);
	CL_String html =
		"<!-- clanlib header begin -->"
		"<HTML>"
		"<HEAD>"
		"<TITLE>All Classes - ClanLib Game SDK</TITLE>"
		"<STYLE TYPE=\"text/css\"><!--"
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
		"<a href=\"http://clanlib.org/docs.html\">Home</a> |"
		"<a href=\"classes.html\">All Classes</a> |"
		"<a href=\"modules.html\">Grouped Classes</a> |"
		"<a href=\"index.html\">Index</a> |"
		"<a href=\"search.html\">Search</a>"
		"</p>"
		"</center>"
		"<h1>All Classes</h1>"
		"<table width=100%>"
		"<tr><td valign=top><p>";
	classes_file.write(html.data(), html.length());
	
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
			html = "</p></td>"
				"<td valign=top><p>";
			classes_file.write(html.data(), html.length());
			sumintablines += intabline;
			intabline = 0; needspace = false;
			if (needchar) { intabline += 1; }
			intabline += 1; // for the next object
		}


		if (needspace)
		{
			classes_file.write("<br>", 4);
		}
		if (needchar)
		{
			// there is no need for makeuppercharackter because sorting is case sensetive and Classes should be C lass ;-)
			char name[2];
			name[0] = (*it1).className[indexchar];
			name[1] = 0;
			html = cl_format("<b>%1</b><br>", name);
			classes_file.write(html.data(), html.length());
		}
		
		html = cl_format("<a href=\"%1.html\">%2</a><br>", (*it1).className, (*it1).className);
		classes_file.write(html.data(), html.length());

		it2 = it1;
		++it1;
	}
	sumintablines += intabline;

	html =
		"</p></td></tr>"
		"</table>"
		"<!-- clanlib footer begin -->"
		"<center><br><br><font color=\"#a0a0a0\">"
		"Questions or comments, write to the <a href=\"http://clanlib.org/contact.html\">ClanLib mailing list</a>."
		"</font></center>"
		"<!-- clanlib footer end -->"
		"</body>"
		"</html>";
	classes_file.write(html.data(), html.length());
}


void ReferenceDocs::generate_indexedclasses()
{
	CL_File index_file("reftest/index.html", CL_File::create_always, CL_File::access_write);
	CL_String html =
		"<!-- clanlib header begin -->"
		"<HTML>"
		"<HEAD>"
		"<TITLE>Index - ClanLib Game SDK</TITLE>"
		"<STYLE TYPE=\"text/css\"><!--"
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
		"<a href=\"http://clanlib.org/docs.html\">Home</a> |"
		"<a href=\"classes.html\">All Classes</a> |"
		"<a href=\"modules.html\">Grouped Classes</a> |"
		"<a href=\"index.html\">Index</a> |"
		"<a href=\"search.html\">Search</a>"
		"</p>"
		"</center>"
		"<h1>Index</h1>";
	index_file.write(html.data(), html.length());

	rlist.sort(indexsort);
	std::list<RefClass>::iterator it1 = rlist.begin();
	while (it1 != rlist.end())
	{
		html = cl_format("<p><dt>%1</dt>", (*it1).className);
		index_file.write(html.data(), html.length());
		
		html = cl_format("<dd><a href=\"%1.html\">%2</a></dd>", (*it1).className, (*it1).className);
		index_file.write(html.data(), html.length());
		
		std::vector<CL_String>::iterator it3 = (*it1).sections.begin();
		while (it3 != (*it1).sections.end())
		{
			html = cl_format("<dd>%1</dd>", *it3);
			index_file.write(html.data(), html.length());
			++it3;
		}
		index_file.write("</p>", 4);
		++it1;
	}

	html =
		"<!-- clanlib footer begin -->"
		"<center><br><br><font color=\"#a0a0a0\">"
		"Questions or comments, write to the <a href=\"http://clanlib.org/contact.html\">ClanLib mailing list</a>."
		"</font></center>"
		"<!-- clanlib footer end -->"
		"</body>"
		"</html>";
	index_file.write(html.data(), html.length());
}

#endif
