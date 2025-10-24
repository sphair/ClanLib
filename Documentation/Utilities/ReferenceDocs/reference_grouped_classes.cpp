
#include "reference_grouped_classes.h"
#include "reference_docs.h"
#include <algorithm>

void ReferenceGroupedClasses::load()
{
}

	class SortByGroupAndSection
	{
	public:
		bool operator()(ReferenceClass *a, ReferenceClass *b)
		{
			if (a->clan_section != b->clan_section)
				return a->clan_section < b->clan_section;
			else
				return a->clan_group < b->clan_group;
		}
	};

void ReferenceGroupedClasses::save(const CL_StringRef &filename)
{
	CL_File modules_file(filename, CL_File::create_always, CL_File::access_write);
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
//		"<a href=\"http://clanlib.org/docs.html\">Home</a> |"
		"<a href=\"classes.html\">All Classes</a> |"
		"<a href=\"modules.html\">Grouped Classes</a> |"
		"<a href=\"index.html\">Index</a>"
//		"<a href=\"search.html\">Search</a>"
		"</p>"
		"</center>"
		"<h1>Grouped Classes</h1>"
		"<table width=100%>"
		"<tr><td valign=top><p>";
	modules_file.write(html.data(), html.length());


	std::vector<CL_String> lines;
	
	std::vector<ReferenceClass *> sorted_classes;
	for (std::list<ReferenceClass>::iterator it = ReferenceDocs::class_list.begin(); it != ReferenceDocs::class_list.end(); ++it)
		sorted_classes.push_back(&(*it));
		
	
	std::sort(sorted_classes.begin(), sorted_classes.end(), SortByGroupAndSection());
	
	CL_String cur_group, cur_section;
	for (std::vector<ReferenceClass *>::iterator it = sorted_classes.begin(); it != sorted_classes.end(); ++it)
	{
		if (cur_section != (*it)->clan_section)
		{
			if (!cur_section.empty())
				lines.push_back(CL_String());
			cur_group.clear();
			cur_section = (*it)->clan_section;
			html = cl_format("<p><b>%1</b></p>", cur_section);
			lines.push_back(html);
		}
		if (cur_group != (*it)->clan_group)
		{
			if (!cur_group.empty())
				lines.push_back(CL_String());
			cur_group = (*it)->clan_group;
			html = cl_format("<p><i>%1</i></p>", cur_group);
			lines.push_back(html);
		}
		html = cl_format("<p><a href=\"%1.html\">%2</a></p>", (*it)->name, (*it)->name);
		lines.push_back(html);
	}
	
	int num_cols = 5;
	int num_rows = (lines.size()+num_cols-1)/num_cols;
	for (int row = 0; row < num_rows; row++)
	{
		html = "<tr>";
		modules_file.write(html.data(), html.length());
		for (int col = 0; col < num_cols; col++)
		{
			int line = row+col*num_rows;
			html = "<td>";
			modules_file.write(html.data(), html.length());
			
			if (line < lines.size())
				modules_file.write(lines[line].data(), lines[line].length());
			
			html = "</td>";
			modules_file.write(html.data(), html.length());
		}
		html = "</tr>";
		modules_file.write(html.data(), html.length());
	}

#ifdef oldcode


	const int tabrows = 4;
	// sorting the list
	ReferenceDocs::class_list.sort(ReferenceDocs::groupsort);
	// calculating the lines
	int lines = 0;

	std::list<ReferenceClass>::iterator it1 = ReferenceDocs::class_list.begin();

	if (it1 == ReferenceDocs::class_list.end())
	{
		cl_log_event("error", "Empty class list, can't create reference");
		return;
	}

	std::list<ReferenceClass>::iterator it2 = it1;
	lines += 3; // group and subgroup of the first element and the first element itself
	++it1;
	while (it1 != ReferenceDocs::class_list.end())
	{
		if ((*it1).clan_group == (*it2).clan_group && (*it1).clan_section == (*it2).clan_section)
		{
			// only a new element
			lines += 1;
		}
		else if ((*it1).clan_section == (*it2).clan_section)
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
	int intabline = 0;
	int sumintablines = 0;
	it1 = ReferenceDocs::class_list.begin();
	it2 = ReferenceDocs::class_list.begin();
	bool b1 = true; // b1 is true at the first time
	while (it1 != ReferenceDocs::class_list.end())
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
			if ((*it1).clan_group == (*it2).clan_group && (*it1).clan_section == (*it2).clan_section)
			{
				// only a new element
				intabline += 1;
			}
			else if ((*it1).clan_section == (*it2).clan_section)
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
			html = cl_format("<b>%1</b><br>", (*it1).clan_section);
			modules_file.write(html.data(), html.length());
		}
		if (needsubgroup)
		{
			html = cl_format("<i>%1</i><br>", (*it1).clan_group);
			modules_file.write(html.data(), html.length());
		}
		html = cl_format("<a href=\"%1.html\">%2</a><br>", (*it1).name, (*it1).name);
		modules_file.write(html.data(), html.length());

		it2 = it1;
		++it1;
	}
	sumintablines += intabline;
#endif



	html = "</p></td></tr>"
		"</table>";
	modules_file.write(html.data(), html.length());

	html = "<!-- clanlib footer begin -->"
//		"<center><br><br><font color=\"#a0a0a0\">"
//		"Questions or comments, write to the <a href=\"http://clanlib.org/contact.html\">ClanLib mailing list</a>."
//		"</font></center>"
		"<!-- clanlib footer end -->"
		"</body>"
		"</html>";
	modules_file.write(html.data(), html.length());

}
