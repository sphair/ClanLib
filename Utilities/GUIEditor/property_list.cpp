
#include "precomp.h"
#include "property_list.h"

/////////////////////////////////////////////////////////////////////////////
// PropertyList construction:

PropertyList::PropertyList(const CL_Rect &pos, CL_Component *parent)
: CL_Component(pos, parent)
{
	font = new CL_Font("ListBox/font", get_style_manager()->get_resources());

	properties.push_back(Property("name", "label_title"));
	properties.push_back(Property("x", "20"));
	properties.push_back(Property("y", "20"));
	properties.push_back(Property("width", "500"));
	properties.push_back(Property("height", "17"));
	properties.push_back(Property("text", "Welcome to the CTalk IRC Client."));

	slots.connect(sig_paint(), this, &PropertyList::on_paint);
}

PropertyList::~PropertyList()
{
	delete font;
}

/////////////////////////////////////////////////////////////////////////////
// PropertyList attributes:


/////////////////////////////////////////////////////////////////////////////
// PropertyList operations:


/////////////////////////////////////////////////////////////////////////////
// PropertyList implementation:

void PropertyList::on_paint()
{
	int width = get_width();
	int height = get_height();

	CL_Display::fill_rect(CL_Rect(0, 0, width, height), CL_Color::white);
	CL_Display::draw_rect(CL_Rect(0, 0, width, height), CL_Color(128, 142, 159));

	CL_Display::draw_line(width/2, 0, width/2, height, CL_Color(128, 142, 159));

	for (std::vector<Property>::size_type y=0; y<properties.size(); y++)
	{
		font->draw(5, y*20+5, properties[y].first);
		font->draw(width/2+5, y*20+5, properties[y].second);
		CL_Display::draw_line(0, (y+1)*20, width, (y+1)*20, CL_Color(128, 142, 159));
	}
}
