
#include "precomp.h"

using namespace clan;

int main(int, char **)
{
	auto style = std::make_shared<Style>();

	style->set("color", "rgb(255,128,64)");
	style->set("border", "1px solid yellow");
	style->set("border-top", "none");
	style->set("margin", "10px auto");
	style->set("padding", "10mm 5pt");
	style->set("font", "15px/24px 'Segoe UI', sans-serif");

	Colorf c = style->color("color");
	Console::write_line("color: rgba(%1,%2,%3,%4)", (int)(c.r * 255), (int)(c.g * 255), (int)(c.b * 255), (int)(c.a * 255));

	Console::write_line("border left style: %1", style->keyword("border-left-style"));
	Console::write_line("border top style: %1", style->keyword("border-top-style"));
	Console::write_line("border right style: %1", style->keyword("border-right-style"));
	Console::write_line("border bottom style: %1", style->keyword("border-bottom-style"));

	Console::write_line("margin left: %1", style->keyword("margin-left"));
	Console::write_line("margin top: %1", style->length("margin-top"));
	Console::write_line("margin right: %1", style->keyword("margin-right"));
	Console::write_line("margin bottom: %1", style->length("margin-bottom"));

	Console::write_line("padding left: %1", style->length("padding-left"));
	Console::write_line("padding top: %1", style->length("padding-top"));
	Console::write_line("padding right: %1", style->length("padding-right"));
	Console::write_line("padding bottom: %1", style->length("padding-bottom"));

	Console::write_line("font-size: %1", style->length("font-size"));
	Console::write_line("line-height: %1", style->length("line-height"));

	return 0;
}
