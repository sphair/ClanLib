
#include <ClanLib/core.h>
using namespace clan;

void evaluate(const std::string &xpath, const DomDocument &document)
{
	Console::write_line("Evaluating XPath '%1'", xpath);

	XPathEvaluator evaluator;
	XPathObject result = evaluator.evaluate(xpath, document);
	switch (result.get_type())
	{
	case XPathObject::type_null:
		Console::write_line("Result: null");
		break;
	case XPathObject::type_node_set:
		{
			std::vector<DomNode> nodes = result.get_node_set();
			Console::write_line("Result: node-set (%1)", (int)nodes.size());
			for (std::vector<DomNode>::size_type i = 0; i < nodes.size(); i++)
			{
				std::string text = nodes[i].get_node_value();
				if (nodes[i].is_element())
					text = nodes[i].to_element().get_text();
				Console::write_line("#%1: [%2] %3", (int)i, nodes[i].get_node_name(), text);
			}
		}
		break;
	case XPathObject::type_number:
		Console::write_line("Result: number (%1)", (int) result.get_number());
		break;
	case XPathObject::type_string:
		Console::write_line("Result: string (%1)", result.get_string());
		break;
	case XPathObject::type_boolean:
		Console::write_line("Result: boolean (%1)", result.get_boolean() ? "true" : "false");
		break;
	}
	Console::write_line("");
}

int main(int, char**)
{
	try
	{
		File file("test.xml", File::open_existing, File::access_read);
// 		File file("test2.xml", File::open_existing, File::access_read);
		DomDocument document;
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
	catch(Exception &error)
	{
		Console::write_line("Exception caught:");
		Console::write_line(error.message);
		return -1;
	}

	return 0;
}
