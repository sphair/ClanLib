
#include <ClanLib/core.h>

void evaluate(const CL_String &xpath, const CL_DomDocument &document)
{
	CL_Console::write_line(L"Evaluating XPath '%1'", xpath);

	CL_XPathEvaluator evaluator;
	CL_XPathObject result = evaluator.evaluate(xpath, document);
	switch (result.get_type())
	{
	case CL_XPathObject::type_null:
		CL_Console::write_line("Result: null");
		break;
	case CL_XPathObject::type_node_set:
		{
			std::vector<CL_DomNode> nodes = result.get_node_set();
			CL_Console::write_line("Result: node-set (%1)", (int)nodes.size());
			for (std::vector<CL_DomNode>::size_type i = 0; i < nodes.size(); i++)
			{
				CL_String text = nodes[i].get_node_value();
				if (nodes[i].is_element())
					text = nodes[i].to_element().get_text();
				CL_Console::write_line("#%1: [%2] %3", (int)i, nodes[i].get_node_name(), text);
			}
		}
		break;
	case CL_XPathObject::type_number:
		CL_Console::write_line("Result: number (%1)", (int) result.get_number());
		break;
	case CL_XPathObject::type_string:
		CL_Console::write_line("Result: string (%1)", result.get_string());
		break;
	case CL_XPathObject::type_boolean:
		CL_Console::write_line("Result: boolean (%1)", result.get_boolean() ? "true" : "false");
		break;
	}
	CL_Console::write_line("");
}

int main(int, char**)
{
	CL_SetupCore setup_core;
	try
	{
		CL_File file("test.xml", CL_File::open_existing, CL_File::access_read);
// 		CL_File file("test2.xml", CL_File::open_existing, CL_File::access_read);
		CL_DomDocument document;
		document.load(file);

// 		evaluate("root/child/child[2]/following::*", document);
// 		evaluate("root/child/child[2]/childchild[2]/preceding::*", document);
		evaluate("//child/attribute::type", document);

// 		evaluate("6 mod 4", document);
// 		evaluate("/root/child/childchild", document);
// 		evaluate("/child::root/child::child/child::childchild", document);
// 		evaluate("child::root/child::child/child::childchild", document);
// 		evaluate("child::root/child::child[@foo]/child::childchild", document);
// 		evaluate("child::root/child::child[child::foobar]/child::childchild", document);
// 		evaluate("child::root/child::child[2]/child::childchild", document);
// 		evaluate("root//childchild", document);
// 		evaluate("root/child[@foo]/childchild", document);
// 		evaluate("root/child[@foo=\"barism\"]/childchild", document);
// 		evaluate("root/child[childchild=\"Test6\"]/foobar", document);
// 		evaluate("root/child[@age!=10]/foobar", document);
// 		evaluate("root/child[@age>27]/foobar", document);
// 		evaluate("root/com:child/foobar", document);
// 		evaluate("root/child[last()-3]/foobar", document);
// 		evaluate("root/child[1]/childchild[2]", document);
// 		evaluate("translate('bare', 'abr', 'AB')", document);
// 		evaluate("substring-before('1999/04/01','/')", document);
// 		evaluate("substring-after('1999/04/01','/')", document);
// 		evaluate("substring('12345', 2)", document);
// 		evaluate("substring('12345', 2, 3)", document);
// 		evaluate("count(root/child[position() mod 2 = 0])", document);
// 		evaluate("namespace-uri(root/com:child)", document);
// 		evaluate("local-name(root/com:child)", document);
// 		evaluate("count(root/child::*[local-name()='child'])", document);
// 		evaluate("normalize-space('\tchild    \tname\n  \t  thingie\n')", document);
// 		evaluate("string-length(root/*[local-name()='child'][position()=last()-2]/foobar)", document);
// 		evaluate("root/*[local-name()='child' and (@age=10 or namespace-uri()='fisk')]/foobar", document);
// 		evaluate("root/child[last()]/foobar", document);
// 		evaluate("root/child[not(@foo) and not(@age)]/foobar", document);
// 		evaluate("root/child[last()]/foobar | root/child[not(@foo) and not(@age)]/foobar", document);
// 		evaluate("root/*[local-name()='child'][last()]/foobar | root/child[not(@foo) and not(@age)]/foobar", document);
// 		evaluate("sum(root/child[@type='numbers']/number)", document);
// 		evaluate("/root/child[1]/childchild[1]/text()", document);
// 		evaluate("(root/*[local-name()='child'])[1]", document);
// 		evaluate("root/*[local-name()='child'][1]", document);
// 		evaluate("namespace-uri(root/*[local-name()='child'][1])", document);
// 		evaluate("local-name(/root/child[1])", document);
// 		evaluate("id(/root/child[1]/childchild[1])", document);
// 		evaluate("//childchild[1]", document);
	}
	catch(CL_Exception &error)
	{
		CL_Console::write_line("Exception caught:");
		CL_Console::write_line(error.message);
		return -1;
	}

	return 0;
}
