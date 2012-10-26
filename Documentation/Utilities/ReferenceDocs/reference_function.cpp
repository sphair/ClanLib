
#include "reference_function.h"
#include "reference_docs.h"

void ReferenceFunction::load(CL_String member_name, CL_DomElement class_element)
{
	if (class_element.get_tag_name() != "compounddef")
		throw CL_Exception("Expected compounddef element");

	name = member_name;

	CL_DomNode cur_node;
	for (cur_node = class_element.get_first_child(); !cur_node.is_null(); cur_node = cur_node.get_next_sibling())
	{
		CL_DomElement cur_element = cur_node.to_element();
		if (cur_element.is_null()) continue;

		CL_String tag_name = cur_element.get_tag_name();
		if ( (tag_name == "sectiondef") && (
			(cur_element.get_attribute("kind") == "user-defined") ||
			(cur_element.get_attribute("kind") == "public-func") ||
			(cur_element.get_attribute("kind") == "public-static-func") ) )
		{
			parse_sectiondef(cur_element);
		}
	}
}

void ReferenceFunction::save(const CL_StringRef &filename)
{
	CL_File function_file(filename, CL_File::create_always, CL_File::access_write);


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
//		"<a href=\"http://clanlib.org/docs.html\">Home</a> |"
		"<a href=\"classes.html\">All Classes</a> |"
		"<a href=\"modules.html\">Grouped Classes</a> |"
		"<a href=\"index.html\">Index</a>"
//		"<a href=\"search.html\">Search</a>"
		"</p>"
		"</center>",
		name);
	function_file.write(html.data(), html.length());

	html = cl_format(
		"<h1>Function %1</h1>"
		"<p>%2</p>"
		"<pre>\n"
		"%3\n"
		"</pre>",
		name,
		brief_description,
		declaration);
	function_file.write(html.data(), html.length());
	
	if (!parameters.empty())
	{
		html = "<p><b>Parameters:</b></p>";
		function_file.write(html.data(), html.length());

		for (unsigned int index = 0; index < parameters.size(); index++)
		{
			html = cl_format("<dt><i>%1</i></dt><dd>%2</dd>", parameters[index].name, parameters[index].description);
			function_file.write(html.data(), html.length());
		}
	}
/*
        print FILE "<p><b>Return value:</b></p>\n";
        print FILE "<p>";
        print_description(&addCrossRef($funcretval{$filename}));
        print FILE "</p>\n";
*/

	if (!CL_StringHelp::trim(detailed_description).empty())
	{
		html = cl_format(
			"<p><b>Detailed description:</b></p>"
			"<p>%1</p>",
			detailed_description);
		function_file.write(html.data(), html.length());
        }
/*
	html = cl_format(
		"<p><b>See also:</b></p>"
		"<p><a href=\"%1.html\">%2</a></p>",
		class_name,
		to_filename(class_name));
	function_file.write(html.data(), html.length());
*/
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
//		"<center><br><br><font color=\"#a0a0a0\">"
//		"Questions or comments, write to the <a href=\"http://clanlib.org/contact.html\">ClanLib mailing list</a>."
//		"</font></center>"
		"<!-- clanlib footer end -->"
		"</body>"
		"</html>";
	function_file.write(html.data(), html.length());
}

void ReferenceFunction::parse_sectiondef(CL_DomElement sectiondef)
{
	CL_DomNode cur_node;
	for (cur_node = sectiondef.get_first_child(); !cur_node.is_null(); cur_node = cur_node.get_next_sibling())
	{
		CL_DomElement cur_element = cur_node.to_element();
		if (cur_element.is_null()) continue;
		
		CL_String tag_name = cur_element.get_tag_name();
		if (tag_name == "memberdef")
		{
			parse_memberdef(cur_element);
		}
	}
}

void ReferenceFunction::parse_memberdef(CL_DomElement memberdef)
{
	CL_DomElement element_name = memberdef.named_item("name").to_element();
	if (element_name.is_null() || element_name.get_text() != name)
		return;

	CL_String type, type_formatted, definition, argsstring;
	std::vector<CL_String> cur_params;

	CL_DomNode cur_node;
	for (cur_node = memberdef.get_first_child(); !cur_node.is_null(); cur_node = cur_node.get_next_sibling())
	{
		CL_DomElement cur_element = cur_node.to_element();
		if (cur_element.is_null()) continue;

		CL_String tag_name = cur_element.get_tag_name();
		
		if (tag_name == "type")
		{
			type = cur_element.get_text();
			type_formatted = ReferenceDocs::get_formatted(cur_element);
		}
		else if (tag_name == "definition")
		{
			definition = CL_StringHelp::trim(ReferenceDocs::escape_code(cur_element.get_text()));
		}
		else if (tag_name == "argsstring")
		{
			argsstring = cur_element.get_text();
		}
		else if (tag_name == "param")
		{
			CL_String param_type = ReferenceDocs::get_formatted(cur_element.named_item("type").to_element());
			CL_String param_name = cur_element.named_item("declname").to_element().get_text();
			CL_String param_defval = ReferenceDocs::get_formatted(cur_element.named_item("defval").to_element());
			
			CL_String param_text = param_type + " " + param_name;
			if (!param_defval.empty())
				param_text += CL_String(" = ") + param_defval;
			cur_params.push_back(param_text);
		}
		else if (tag_name == "briefdescription")
		{
			brief_description = ReferenceDocs::get_formatted(cur_element);
		}
		else if (tag_name == "detaileddescription")
		{
			detailed_description = ReferenceDocs::get_formatted(cur_element);
			get_params_list(cur_element);
		}
	}

	CL_String cur_declaration;
	cur_declaration += cl_format("\t%1(", definition);
	for (unsigned int index_params = 0; index_params < cur_params.size(); index_params++)
	{
		if (index_params > 0)
			cur_declaration += ",";
		cur_declaration += cl_format("\n\t\t%1", cur_params[index_params]);
	}
	cur_declaration += ");\n";

	if (!declaration.empty())
		declaration += "\n";
	declaration += cur_declaration;
}

void ReferenceFunction::get_params_list(const CL_DomElement &element)
{
	CL_DomNode cur_node;
	for (cur_node = element.get_first_child(); !cur_node.is_null(); cur_node = cur_node.get_next_sibling())
	{
		if (!cur_node.is_element()) continue;
		
		CL_DomElement cur_element = cur_node.to_element();
		if (cur_element.get_tag_name() == "parameterlist")
		{
			CL_String kind = cur_element.get_attribute("kind");
			if (kind != "param") continue;

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
					param_description = ReferenceDocs::get_formatted(param_element);
				}
			}
			
			if (!param_name.empty())
			{
				ReferenceFunctionParameter parameter;
				parameter.name = param_name;
				parameter.description = param_description;
				parameters.push_back(parameter);
			}
		}
		else
		{
			get_params_list(cur_element);
		}
	}
}
