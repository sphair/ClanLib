
#include "precomp.h"

using namespace clan;

int main(int, char **)
{
	auto style = std::make_shared<Style>();

	style->set("color", "rgb(255,128,64)");

	Colorf c = style->color("color");
	Console::write_line("color: rgba(%1,%2,%3,%4)", (int)(c.r * 255), (int)(c.g * 255), (int)(c.b * 255), (int)(c.a * 255));

	return 0;
}
