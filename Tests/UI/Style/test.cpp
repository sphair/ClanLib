
#include "precomp.h"

using namespace clan;

int main(int, char **)
{
	auto style = std::make_shared<Style>();

	style->set("color: rgb(255,128,64)");
	style->set("border: 1px solid yellow; border-top: none");
	style->set("margin: 1em auto");
	style->set("padding: 10mm 5pt 42px 2.1in");
	style->set("font: 15px/24px 'Segoe UI', sans-serif");
	style->set("background: url('foobar.png'), linear-gradient(to bottom, red, green 25%, blue 100px, rgba(220,220,220,0.25)), red");

	Colorf c = style->computed_value("color").color;
	Console::write_line("color: rgba(%1, %2, %3, %4)", (int)(c.r * 255), (int)(c.g * 255), (int)(c.b * 255), c.a);

	Console::write_line("border left style: %1", style->computed_value("border-left-style").text);
	Console::write_line("border top style: %1", style->computed_value("border-top-style").text);
	Console::write_line("border right style: %1", style->computed_value("border-right-style").text);
	Console::write_line("border bottom style: %1", style->computed_value("border-bottom-style").text);

	Console::write_line("margin left: %1", style->computed_value("margin-left").text);
	Console::write_line("margin top: %1", style->computed_value("margin-top").number);
	Console::write_line("margin right: %1", style->computed_value("margin-right").text);
	Console::write_line("margin bottom: %1", style->computed_value("margin-bottom").number);

	Console::write_line("padding left: %1", style->computed_value("padding-left").number);
	Console::write_line("padding top: %1", style->computed_value("padding-top").number);
	Console::write_line("padding right: %1", style->computed_value("padding-right").number);
	Console::write_line("padding bottom: %1", style->computed_value("padding-bottom").number);

	Console::write_line("font-size: %1", style->computed_value("font-size").number);
	Console::write_line("line-height: %1", style->computed_value("line-height").number);

	Console::write_line("background image: %1", style->computed_value("background-image").text);
	Console::write_line("background image[0]: %1", style->computed_value("background-image[0]").text);
	Console::write_line("background image[1]: %1", style->computed_value("background-image[1]").text);
	Console::write_line("background image[2]: %1", style->computed_value("background-image[2]").text);

	Colorf bg = style->computed_value("background-color").color;
	Console::write_line("background color: rgba(%1, %2, %3, %4)", (int)(bg.r * 255), (int)(bg.g * 255), (int)(bg.b * 255), bg.a);

	return 0;
}
