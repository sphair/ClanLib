
#include "reference_index.h"

void ReferenceIndex::load()
{
}

void ReferenceIndex::save(const CL_StringRef &filename)
{
	CL_File index_file(filename, CL_File::create_always, CL_File::access_write);
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
//		"<a href=\"http://clanlib.org/docs.html\">Home</a> |"
		"<a href=\"classes.html\">All Classes</a> |"
		"<a href=\"modules.html\">Grouped Classes</a> |"
		"<a href=\"index.html\">Index</a>"
//		"<a href=\"search.html\">Search</a>"
		"</p>"
		"</center>"
		"<h1>Index</h1>";
	index_file.write(html.data(), html.length());

	html =
		"<!-- clanlib footer begin -->"
//		"<center><br><br><font color=\"#a0a0a0\">"
//		"Questions or comments, write to the <a href=\"http://clanlib.org/contact.html\">ClanLib mailing list</a>."
//		"</font></center>"
		"<!-- clanlib footer end -->"
		"</body>"
		"</html>";
	index_file.write(html.data(), html.length());
}
