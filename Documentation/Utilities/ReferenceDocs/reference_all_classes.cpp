
#include "reference_all_classes.h"
#include "reference_docs.h"

void ReferenceAllClasses::load()
{
}

void ReferenceAllClasses::save(const CL_StringRef &filename)
{
	CL_File classes_file(filename, CL_File::create_always, CL_File::access_write);
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
//		"<a href=\"http://clanlib.org/docs.html\">Home</a> |"
		"<a href=\"classes.html\">All Classes</a> |"
		"<a href=\"modules.html\">Grouped Classes</a> |"
		"<a href=\"index.html\">Index</a>"
//		"<a href=\"search.html\">Search</a>"
		"</p>"
		"</center>"
		"<h1>All Classes</h1>"
		"<table width=100%>"
		"<tr><td valign=top><p>";
	classes_file.write(html.data(), html.length());


	const int tabrows = 4;
	ReferenceDocs::class_list.sort(ReferenceDocs::indexsort);

	CL_String::char_type current_letter = 0;

	std::vector<CL_String> cells;
	for (std::list<ReferenceClass>::iterator it = ReferenceDocs::class_list.begin(); it != ReferenceDocs::class_list.end(); ++it)
	{
		CL_String::char_type cell_letter = 0;
		if (it->name.substr(0, 3) == "CL_")
			cell_letter = it->name[3];
		else
			cell_letter = it->name[0];

		if (current_letter != cell_letter)
		{
			if (!cells.empty())
				cells.push_back("");
			cells.push_back(cl_format("<p><b>%1</b></p>", CL_String(1, cell_letter)));
			current_letter = cell_letter;
		}

		cells.push_back(cl_format("<p><a href=\"%1.html\">%2</a></p>", it->name, it->name));
	}

	std::vector<CL_String>::size_type num_rows = (cells.size() + tabrows-1) / tabrows;
	std::vector<CL_String>::size_type missing_cells = num_rows * tabrows - cells.size();
	for (std::vector<CL_String>::size_type i = 0; i < missing_cells; i++)
		cells.push_back("");

	for (unsigned int i = 0; i < num_rows; i++)
	{
		CL_String html = "<tr>";
		for (int j = 0; j < tabrows; j++)
			html += cl_format("<td>%1</td>", cells[i+j*num_rows]);
		html += "</tr>";
		classes_file.write(html.data(), html.length());
	}


	html =
		"</p></td></tr>"
		"</table>"
		"<!-- clanlib footer begin -->"
//		"<center><br><br><font color=\"#a0a0a0\">"
//		"Questions or comments, write to the <a href=\"http://clanlib.org/contact.html\">ClanLib mailing list</a>."
//		"</font></center>"
		"<!-- clanlib footer end -->"
		"</body>"
		"</html>";
	classes_file.write(html.data(), html.length());
}
