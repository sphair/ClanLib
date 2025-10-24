
#include <ClanLib/core.h>

int main(int, char**)
{
	CL_SetupCore setup_core;
	try
	{
		CL_File file("test.xml", CL_File::open_existing, CL_File::access_read);
		CL_DomDocument document;
		document.load(file);
		CL_XPathEvaluator evaluator;
//		CL_XPathObject result = evaluator.evaluate("6 mod 4", document);
//		CL_XPathObject result = evaluator.evaluate("/root/child/childchild", document);
//		CL_XPathObject result = evaluator.evaluate("/child::root/child::child/child::childchild", document);
//		CL_XPathObject result = evaluator.evaluate("child::root/child::child/child::childchild", document);
//		CL_XPathObject result = evaluator.evaluate("child::root/child::child[@foo]/child::childchild", document);
//		CL_XPathObject result = evaluator.evaluate("child::root/child::child[child::foobar]/child::childchild", document);
//		CL_XPathObject result = evaluator.evaluate("child::root/child::child[2]/child::childchild", document);
//		CL_XPathObject result = evaluator.evaluate("root//childchild", document);
//		CL_XPathObject result = evaluator.evaluate("root/child[@foo]/childchild", document);
//		CL_XPathObject result = evaluator.evaluate("root/child[@foo=\"barism\"]/childchild", document);
//		CL_XPathObject result = evaluator.evaluate("root/child[childchild=\"Test6\"]/foobar", document);
//		CL_XPathObject result = evaluator.evaluate("root/child[@age!=10]/foobar", document);
//		CL_XPathObject result = evaluator.evaluate("root/child[@age>27]/foobar", document);
		CL_XPathObject result = evaluator.evaluate("root/com:child/foobar", document);
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
					CL_Console::write_line("#%1: %2 %3", (int)i, nodes[i].get_node_name(), nodes[i].to_element().get_text());
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
	}
	catch(CL_Exception error)
	{
		CL_Console::write_line("Exception caught:");
		CL_Console::write_line(error.message);
		return -1;
	}

	return 0;
}

